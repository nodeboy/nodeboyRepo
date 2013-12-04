#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <error.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>

#include "wtd.h"


//#define WTD_DEBUG
#undef DEBUG
#ifdef WTD_DEBUG
#define DEBUG(fmt...) printf(fmt)
#else
#define DEBUG(arg, fmt...)
#endif

struct thread{
    pthread_t thread_id;
    struct thread *next;
    struct thread *prev;

};

struct wtd_mng{
    pid_t pid;  /*process id, work at this process*/
    int fd0;    /*use socketpair to send to kick watchdog*/
    int sockpair_fd[2]; /*use socketpair to control the threads in this process*/
    int kick_timeout;   /*unit us*/
    struct thread *thread_list_head;
    struct thread *thread_list_tail;
    struct wtd_msg *msg;
};

static struct wtd_mng *wtd;

static void *wtd_kick(void *arg)
{
    struct wtd_mng *wtd;
    struct wtd_msg *msg;
    fd_set  rd_fd_set;
    struct timeval timeout;
    int kick_timeout_ms;
    int select_ret;
    int kick_fd;

    if(!arg) {
        printf("wtd_kick arg is NULL\r\n");
        return NULL;
    }

    wtd = (struct wtd_mng*)arg;
    wtd->msg = (struct wtd_msg *)malloc(sizeof(struct wtd_msg));
    msg = wtd->msg;
    msg->pid = wtd->pid;
    msg->msg_type = WTD_ADD_SELFCHECK_TASK;
    send(wtd->fd0, (void *)msg, sizeof(struct wtd_msg), 0);

    kick_fd = wtd->sockpair_fd[1];
    kick_timeout_ms = wtd->kick_timeout * 1000;
    DEBUG("kick timeout:%d sec\r\n", wtd->kick_timeout);
    while(1) {
        timeout.tv_sec = kick_timeout_ms / 1000;
        timeout.tv_usec = (kick_timeout_ms % 1000) * 1000;

        FD_ZERO(&rd_fd_set);
        FD_SET(kick_fd, &rd_fd_set);

        select_ret = select(kick_fd + 1, &rd_fd_set, NULL, NULL, &timeout);
        if(select_ret == 0) {
            DEBUG("wtd_kick:timeout\r\n");
            msg->msg_type = WTD_TASK_SELFCHCEK_RESULT;
            *(int *)(void *)(msg->msg_buf) = TASK_SELFCHECK_OK;
            send(wtd->fd0, (void *)msg, sizeof(struct wtd_msg), 0);
            continue;
        }
        else if( select_ret > 0){
            DEBUG("receive...\r\n");
        }
        else {
            perror("select error");
        }
    }
    free(wtd->msg);
    free(wtd);
    pthread_exit(0);
    return NULL;
}

static int wtd_kick_add_thread(pthread_t thread_id, struct wtd_mng *mng)
{
    struct thread *thread_list_head;
    struct thread *thread_list_tail;
    struct thread *new_thread;
    
    if(!mng) {
        return -1;
    }

    thread_list_head = mng->thread_list_head;
    thread_list_tail = mng->thread_list_tail;
    new_thread = malloc(sizeof(struct thread));
    if(!new_thread) {
        printf("new_thread malloc fail\n");
        return -1;
    }

    memset(new_thread , 0 ,sizeof(struct thread));
    new_thread->thread_id = thread_id;
    
    if(thread_list_head == NULL && thread_list_tail == NULL) {
        thread_list_head = thread_list_tail = new_thread;
    }
    else{
        thread_list_tail->next = new_thread;
        thread_list_tail = new_thread;
    }

    return 0;
}

static int wtd_kick_del_thread(pthread_t thread_id, struct wtd_mng *mng)
{
   // struct thread *thread_list_head;
   // struct thread *thread_list_tail;
    struct thread *del_thread;
    
    if(!mng) {
        return -1;
    }

    del_thread = mng->thread_list_head;
    while(del_thread != mng->thread_list_tail) {
        if(del_thread->thread_id == thread_id) {
            del_thread->prev->next = del_thread->next;
            del_thread->next->prev = del_thread->prev;
            free(del_thread);
            break;
        }
        del_thread = del_thread->next;
    }
    return 0;




}

int wtd_kick_init(int fd0)
{
    pthread_t wtd_kick_thread;

    if(fd0 < 0) {
        printf("param error\r\n");
        return -1;
    }
    wtd = (struct wtd_mng*)malloc(sizeof(struct wtd_mng));
    if(!wtd) {
        return -1;
    }
    memset(wtd, 0, sizeof(struct wtd_mng));

    wtd->fd0 = fd0;
    if(socketpair(AF_UNIX, SOCK_DGRAM, 0, wtd->sockpair_fd) < 0){
        perror("socketpair error:");
        return -1;
    }
    wtd->pid = getpid();    
    wtd->kick_timeout = DEFAULT_KICK_TIMEOUT;
    if(pthread_create(&wtd_kick_thread, NULL, wtd_kick, (void *)wtd) < 0){
        printf("pthread_create fial\r\n");
        free(wtd);
        return -1;
    }
    return 0;
}

int wtd_kick_add_task(int fd0)
{
    pid_t pid;
    struct wtd_msg msg;
    if(fd0 < 0) {
        printf("param error\r\n");
        return -1;
    }

    pid = getpid();
    msg.pid = pid;
    msg.msg_type = WTD_ADD_TASK;
    return send(fd0, (void *)&msg, sizeof(struct wtd_msg), 0);
}

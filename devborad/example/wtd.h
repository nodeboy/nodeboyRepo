#ifndef _WTD_H
#define _WTD_H

#define WTD_ADD_TASK				1
#define	WTD_ADD_SELFCHECK_TASK		2
#define WTD_TASK_SELFCHCEK_RESULT	3
#define TASK_SELFCHECK_FAIL 0
#define TASK_SELFCHECK_OK 1

#define DEFAULT_HEATBEAT 7
#define DEFAULT_KICK_TIMEOUT 5	/*must less than DEFAULT_HEATBEAT*/
struct wtd_msg{
    pid_t   pid;
    int     msg_type;
    char    msg_buf[64];
};
int wtd_kick_add_task(int fd0);
int wtd_kick_init(int fd0);
int wtd_init(void);
#endif

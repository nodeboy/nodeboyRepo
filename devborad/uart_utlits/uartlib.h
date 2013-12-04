#ifndef GPS_H
#define GPS_H


#define FALSE -1
#define TRUE 0
#ifdef __cplusplus
extern "C" {
extern  int set_Parity(int fd,int databits,int stopbits,int parity,int mode));
extern int OpenDev(char *Dev);
extern void set_speed(int fd, int speed);
extern void read_n(int fd, void *buf, int len);
extern void read_n(int fd, void *buf, int len);
}
#endif
#endif

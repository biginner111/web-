//通信模块头文件
#ifndef __SOCKET_H_
#define __SOCKET_H_
//初始化套接字
int initSocket(short port);
//接受客户端的连接请求
int acceptClient(void);
//接受http请求
char* recvRequest(int conn);
//发送响应头
int sendHead(int conn,const char* head);
//发送响应体
int sendBody(int conn,const char* path);
//终结化套接字
void deinitSocket(void);
#endif //__SOCKET_H_

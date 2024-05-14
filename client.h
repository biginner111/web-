//线程模块头文件
#ifndef __CLIENT_H_
#define __CLIENT_H_
//线程需要的参数
typedef struct clientArgs{
    const char* home;//资源在本地的存储路径
    int conn;//通信套接字
}CA;


//线程过程函数,负责和客户端通信
void* client(void* arg);

#endif //__CLIENT_H_

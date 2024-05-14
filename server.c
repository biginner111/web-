//服务器模块实现
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"socket.h"
#include"signals.h"
#include"client.h"
#include"server.h"

//初始化服务器
int initServer(short port){
    //初始化信号
    if(initSignals() == -1){
        return -1;
    }
    //初始化套接字
    if(initSocket(port) == -1){
        return -1;
    }
    return 0;
}
//运行服务器
int runServer(const char* home){
    for(;;){
        //接连接请求
        int conn = acceptClient();
        if(conn == -1){
            return -1;
        }
        //开线程
        pthread_t tid;//用来输出线程的ID
        pthread_attr_t attr;//线程属性
        pthread_attr_init(&attr);//初始化
        pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
        
        CA* ca = malloc(sizeof(CA));
        ca->conn = conn;
        ca->home = home;
        int error = pthread_create(&tid,&attr,client,ca);
        if(error){
            fprintf(stderr,"pthread_create:%s\n",strerror(error));
            return -1;
        }
    }
    return 0;
}
//终结化服务器
void deinitServer(void){
    deinitSocket();
}







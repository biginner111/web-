//通信模块实现
#include<unistd.h>
#include<fcntl.h>
#include<sys/syscall.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"socket.h"

static int s_sock = -1;//侦听套接字

int initSocket(short port){
    printf("%d.%ld > 创建套接字\n",getpid(),syscall(SYS_gettid));
    s_sock = socket(AF_INET,SOCK_STREAM,0);
    if(s_sock == -1){
        perror("socket");
        return -1;
    }
    //端口复用,避免服务器重启无法绑定端口
    printf("%d.%ld > 设置套接字\n",getpid(),syscall(SYS_gettid));
    int on = 1;
    if(setsockopt(s_sock,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)) == -1){
        perror("setsockopt");
        return -1;
    }
    printf("%d.%ld > 组织地址结构\n",getpid(),syscall(SYS_gettid));
    struct sockaddr_in ser;
    ser.sin_family = AF_INET;
    ser.sin_port = htons(port);
    ser.sin_addr.s_addr = INADDR_ANY;
    printf("%d.%ld > 绑定端口\n",getpid(),syscall(SYS_gettid));
    if(bind(s_sock,(struct sockaddr*)&ser,sizeof(ser)) == -1){
        perror("bind");
        return -1;
    }
    printf("%d.%ld > 启动侦听\n",getpid(),syscall(SYS_gettid));
    if(listen(s_sock,1024) == -1){
        perror("listen");
        return -1;
    }
    return 0;
}

//接受客户端的连接请求
int acceptClient(void){
    printf("%d.%ld > 等待客户机的连接\n",getpid(),syscall(SYS_gettid));
    struct sockaddr_in cli;//用来输出客户端的地址结构
    socklen_t len = sizeof(cli);//输出地址结构大小
    int conn = accept(s_sock,(struct sockaddr*)&cli,&len);
    if(conn == -1){
        perror("accept");
        return -1;
    }
    printf("%d.%ld > 已经接受到%s.%hu的连接\n",getpid(),syscall(SYS_gettid),
                    inet_ntoa(cli.sin_addr),ntohs(cli.sin_port));
    return conn;
}
//发送响应头
int sendHead(int conn,const char* head){
    if(send(conn,head,strlen(head),0) == -1){
        perror("send");
        return -1;
    }
    return 0;
}
//发送响应体
int sendBody(int conn,const char* path){
    //打开文件
    int fd = open(path,O_RDONLY);
    if(fd == -1){
        perror("open");
        return -1;
    }
    //读取并发送
    char buf[1024];
    ssize_t len;
    while((len = read(fd,buf,sizeof(buf))) > 0){
        if(send(conn,buf,len,0) == -1){
            perror("send");
            return -1;
        }
    }
    //循环结束条件 len = 0  len = -1
    if(len == -1){
        perror("len");
        close(fd);
        return -1;
    }

    //关闭文件
    close(fd);
    return 0;
}
//接受客户端的http请求
char* recvRequest(int conn){
    char* req = NULL;//记录存储区的首地址
    size_t len = 0;//记录已经接收的总字节数
    for(;;){
        char buf[1024] = {};
        ssize_t byte = recv(conn,buf,sizeof(buf)-1,0);
        if(byte == -1){
            perror("recv");
            free(req);
            return NULL;
        }
        if(byte == 0){
            printf("%d.%ld > 客户机关闭连接\n",getpid(),syscall(SYS_gettid));
            free(req);
            return NULL;
        }
        //扩大存储区
        req = realloc(req,len + byte + 1);
        //将此次接受到的数据,拷贝到扩大出来的空间
        memcpy(req+len,buf,byte+1);
        //总长度累加
        len += byte;

        //看所接受的数据中,是否有连续的\r\n
        if(strstr(req,"\r\n\r\n")){
            break;
        }
    }
    return req;
}
//终结化套接字
void deinitSocket(void){
    close(s_sock);
}












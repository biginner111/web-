//http模块实现
#include<unistd.h>
#include<sys/syscall.h>
#include<stdio.h>
#define __USE_GNU//为了使用case版本的函数
#include<string.h>
#include<time.h>
#include"http.h"

//解析请求
int parseRequest(const char* req,HTTP_REQUEST* hreq){
    /*GET /common/startup_scripts.js HTTP/1.1\r\n
    Host: 192.168.222.128\r\n
    User-Agent: Mozilla/5.0\r\n
    Accept: text/html\r\n
    Connection: keep-alive\r\n
    Referer: http://192.168.222.128/index.html\r\n\r\n*/

    sscanf(req,"%s%s%s",hreq->method,hreq->path,hreq->protocol);
    char* con = strcasestr(req,"connection");
    if(con){
        sscanf(con,"%*s%s",hreq->connection);
    }
    printf("%d.%ld > [%s][%s][%s][%s]\n",getpid(),syscall(SYS_gettid),
                    hreq->method,hreq->path,hreq->protocol,hreq->connection);
    //判断是否为get方法
    if(strcasecmp(hreq->method,"get")){
        printf("%d.%ld > 无效的方法\n",getpid(),syscall(SYS_gettid));
        return -1;
    }
    //判断协议版本
    if(strcasecmp(hreq->protocol,"http/1.1")  && strcasecmp(hreq->protocol,"http/1.0")){
        printf("%d.%ld > 无效的版本\n",getpid(),syscall(SYS_gettid));
        return -1;
    }
    return 0;
}
//构造响应头
int constructHead(const HTTP_RESPOND* hres,char* head){
    /*HTTP/1.1 200 OK\r\n
    Server: LaozhangWebserver 4.0\r\n
    Date: Mon 20 Mar 2023 11:40:41\r\n
    Content-Type: application/x-javascript\r\n
    Content-Length: 12311\r\n
    Connection: keep-alive\r\n\r\n*/
    char dateTime[32];
    time_t now = time(NULL);
    strftime(dateTime,sizeof(dateTime),"%a %d %b %Y %T",gmtime(&now));
    sprintf(head,"%s %d %s\r\n"
                 "Server: LAOZHANGZHENSHUI\r\n"
                 "Date: %s\r\n"
                 "Content-Type: %s\r\n"
                 "Content-Length: %ld\r\n"
                 "Connection: %s\r\n\r\n",
                 hres->protocol,hres->status,hres->desc,dateTime,
                 hres->type,hres->length,hres->connection);
    return 0;
}






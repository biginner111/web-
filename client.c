//线程模块实现
#include<unistd.h>
#include<sys/syscall.h>
#include<sys/stat.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"http.h"
#include"socket.h"
#include"resource.h"
#include"client.h"

//线程过程函数
void* client(void* arg){
    CA* ca = (CA*)arg;
    printf("%d.%ld > 客户机线程处理开始\n",getpid(),syscall(SYS_gettid));
    for(;;){
        printf("%d.%ld > 接受请求\n",getpid(),syscall(SYS_gettid));
        char* req = recvRequest(ca->conn);
        if(req == NULL){
            break;
        }
        printf("%d.%ld > 请求电文:\n%s\n",getpid(),syscall(SYS_gettid),req);

        printf("%d.%ld > 解析请求\n",getpid(),syscall(SYS_gettid));
        HTTP_REQUEST hreq = {};//输出解析的关键数据
        if(parseRequest(req,&hreq) == -1){
            free(req);//释放存储区
            break;
        }
        free(req);//后续操作,使用hreq即可,存储区释放

        //处理路径
        //资源路径 /home/tarena/2212/project/home
        char root[PATH_MAX + 1];
        //真实路径 /home/tarena/2212/project/home/c/c.html
        char path[PATH_MAX + 1];
        strcpy(root,ca->home);
        if(root[strlen(root) - 1] == '/'){
            root[strlen(root) - 1] = '\0';
        }
        strcpy(path,root);
        strcat(path,hreq.path);
        //如果请求路径是 / ,则应将首页文件响应给对方
        if(strcmp(hreq.path,"/") == 0){
            strcat(path,"index.html");
        }
        
        //构造响应所需的结构
        HTTP_RESPOND hres = {"HTTP/1.1",200,"OK","text/html"};
        //搜索资源
        if(searchResource(path) == -1){
            // /home/tarena/2212/project/home/404.html
            hres.status = 404;
            strcpy(hres.desc,"NOT FOUND");
            strcpy(path,root);
            strcat(path,"/404.html");
        }else if(identifyType(path,hres.type) == -1){
            hres.status = 404;
            strcpy(hres.desc,"NOT FOUND");
            strcpy(path,root);
            strcat(path,"/404.html");
        }
        //文件长度
        struct stat st;//用来输出文件的元数据
        if(stat(path,&st) == -1){
            break;
        }
        hres.length = st.st_size;

        //连接状态
        if(strlen(hreq.connection)){
            strcpy(hres.connection,hreq.connection);
        }else if(strcasecmp(hreq.protocol,"http/1.0") == 0){
            strcpy(hres.connection,"close");
        }else{
            strcpy(hres.connection,"keep-alive");
        }
        
        printf("%d.%ld > 构造响应\n",getpid(),syscall(SYS_gettid));
        char head[1024];//存储构造好的响应头
        if(constructHead(&hres,head) == -1){
            break;
        }
        printf("%d.%ld > 响应电文:\n%s\n",getpid(),syscall(SYS_gettid),head);

        printf("%d.%ld > 发送响应头\n",getpid(),syscall(SYS_gettid));
        if(sendHead(ca->conn,head) == -1){
            break;
        }
        printf("%d.%ld > 发送响应体\n",getpid(),syscall(SYS_gettid));
        if(sendBody(ca->conn,path) == -1){
            break;
        }

        //如果连接状态是close
        if(strcasecmp(hres.connection,"close") == 0){
            break;
        }
    }
    close(ca->conn);
    free(ca);
    printf("%d.%ld > 客户机线程处理结束\n",getpid(),syscall(SYS_gettid));
    return NULL;
}






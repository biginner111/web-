//资源管理模块实现
#include<unistd.h>
#include<sys/syscall.h>
#include<stdio.h>
#include<string.h>
#include"resource.h"
#include"mime.h"

//搜索资源
int searchResource(const char* path){
    return access(path,R_OK);
}

//判断类型
int identifyType(const char* path,char* type){
    // /c/hello.html
    //先找.
    char* dot = strrchr(path,'.');
    if(dot == NULL){
        printf("%d.%ld > 无法获取文件拓展名\n",getpid(),syscall(SYS_gettid));
        return -1;
    }

    for(int i = 0;i < sizeof(s_mime) / sizeof(s_mime[0]);i++){
        if(strcasecmp(dot,s_mime[i].suffix) == 0){
            strcpy(type,s_mime[i].type);
            return 0;
        }
    }
    printf("%d.%ld > 不可识别的拓展名类型\n",getpid(),syscall(SYS_gettid));
    return -1;
}

//信号模块实现
#include<unistd.h>
#include<signal.h>
#include<stdio.h>
#include"signals.h"

//忽略大部分信号
int initSignals(void){
    printf("%d > 忽略大部分信号\n",getpid());
    for(int signum = SIGHUP;signum <= SIGRTMAX;signum++){
        if(signum != SIGINT && signum != SIGTERM){
            signal(signum,SIG_IGN);
        }
    }
    return 0;
}

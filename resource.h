#ifndef _RESOURCE_H
#define _RESOURCE_H
#include <stdio.h>//printf
#include <string.h>//strcasecmp,strcpy
#include <unistd.h>//getpid
#include <sys/syscall.h>//syscall
#include "mime.h"

int findtype(const char* path,char* type);
int processread(const char* path);

#endif

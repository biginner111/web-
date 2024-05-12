//根据指定路径判断文件是否可读和查找资源对应类型
#include "resource.h"

//根据指定后缀区查询资源类型
int findtype(const char* path,char *type)
{
 char* buf=strrchr(path,'.');
 if(buf==NULL)
 {
  printf("%d:%ld无法获取文件扩展名\n",getpid(),syscall(SYS_gettid));
  return -1;
 }
 //查找对应类型
 int i;
 for(i=0;i<(sizeof(s_mime)/sizeof(s_mime[1]));i++)
 {
 if(strcasecmp(buf,s_mime[i].suffix)==0)
   {
     strcpy(type,s_mime[i].type);
     return 0;
   }
 }
 printf("%d:%ld:找不到对应类型\n",getpid(),syscall(SYS_gettid));
}



//判断当前进程对找到的文件是否可读
int processread(const char* path)
{
 return access(path,R_OK);
}

#include <stdio.h>    
#include <string.h>                                                            
#include <sys/types.h>    
#include <dirent.h>    
#include <unistd.h>    
#include <errno.h>    
#include <stdlib.h>    
#include <sys/stat.h>    
    
#define MAX_PATH 1024    
void enterdir(char *dir, void(*fuc)(char *))    
{    
   char name[MAX_PATH];    
   struct dirent *dp;    
   DIR *dfd = opendir(dir);    
   if(dfd == NULL)    
   {    
     fprintf(stderr, "enterdir can't open %s\n", dir);    
     return;    
   }    
   while((dp = readdir(dfd)) != NULL)    
   {    
     if(strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)    
     {
       continue;          
     }                 
     if(strlen(dir) + strlen(dp->d_name) > sizeof(name))
     {                                                                         
       fprintf(stderr, "enterdir:name %s %s too long\n", dir, dp->d_name);
     }                   
     else
     {                   
       sprintf(name, "%s/%s", dir, dp->d_name); 
       (*fuc)(name);
     }                     
   }                     
}                              
void showfile(char name[])
{       
   struct stat dir_buf;                                   
   //用stat函数将文件的inode属性存储在dir_buf中,并获取
   //文件描述符
   int fd = stat(name, &dir_buf);        
   if(fd == -1)
   {                                                                      
     fprintf(stderr, "showfile:can't access %s\n", name);
     return;              
   }                   
   if((dir_buf.st_mode & S_IFMT) == S_IFDIR)            
   {                                                                           
     enterdir(name, showfile);                                            
   }                     
   printf("%8ld %s\n", dir_buf.st_size, name);
}                        
int main(int argc, char *argv[])                
{                   
    //通过目录操作实现Linux tree命令
   int i = 0;            
   if(argc == 1)               
   {                      
     showfile(".");
   }                                                      
   else                                               
   {           
     while(--argc > 0)                   
     {         
       showfile(argv[++i]);                                               
     }        
   }                      
   return 0;                                    
}               	   
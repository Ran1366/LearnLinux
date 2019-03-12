#include <stdio.h>                                                                                                                   
#include <unistd.h>    
#include <stdlib.h>    
#include <sys/types.h>    
#include <sys/stat.h>    
#include <errno.h>    
                      
void showrwx(struct stat *buf, char rwx[])    
{    
   if(buf->st_mode & S_IRUSR )    
   {                                
     rwx[1] = 'r';    
   }    
   else                  
   {             
     rwx[1] = '-';                 
   }               
   if(buf->st_mode & S_IWUSR )    
   {                              
     rwx[2] = 'w';    
   }    
   else                          
   {            
     rwx[2] = '-';    
   }
   if(buf->st_mode & S_IXUSR )
   {                      
     rwx[3] = 'x';       
   }                                                                                                                                 
   else               
   {                                          
     rwx[3] = '-';
   }                              
   if(buf->st_mode & S_IRGRP)       
   {                  
     rwx[4] = 'r';
   }                     
   else          
   {                               
     rwx[4] = '-'; 
   }                              
   if(buf->st_mode & S_IWGRP)     
   {
     rwx[5] = 'w';
   }                             
   else         
   {                  
     rwx[5] = '-';
   }                          
   if(buf->st_mode & S_IXGRP)
   {                     
     rwx[6] = 'x';                                                                                                                   
   }                  
   else                                       
   {              
     rwx[6] = '-';                
   }                                
   if(buf->st_mode & S_IROTH)
   {              
     rwx[7] = 'r';       
   }             
   else                            
   {               
     rwx[7] = '-';                
   }                              
   if(buf->st_mode & S_IWOTH)
   {              
     rwx[8] = 'w';               
   }            
   else               
   {   
     rwx[8] = '-';            
   }                         
   if(buf->st_mode & S_IXOTH)
   {                                                                                                                                 
     rwx[9] = 'x';    
   }                                          
   else           
   {                              
     rwx[9] = '-';                  
   }                         
}                        
int main(int argc, char *argv[])
{                                  
   //使用stat函数获取inode信息用来查看文件权限位
   struct stat s_buf;             
   int i = 0;                     
   char rwx[11] = "----------"; 
   if(argc < 2)   
   {                             
      printf("./myls file\n");
      exit(1);        
   } 
   i = stat(argv[1], &s_buf); 
   if(i < 0)                 
   {                         
      perror("stat\n");                                                                                                              
      exit(1);        
   }                                          
   showrwx(&s_buf, rwx);
   printf("%s\n", rwx);           
}                  
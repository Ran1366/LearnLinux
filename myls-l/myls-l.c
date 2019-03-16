#include <stdio.h>
#include <time.h>
#include <grp.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <pwd.h>
struct finfo
{
   char rwx[11];
   unsigned int linknum;
   char *usr;
   char *group;
   off_t file_size;
   char *ftime;
   char *fname;
};
typedef struct finfo F_INFO;
F_INFO fileinfo = {
  .rwx = "----------"
};

void rwx(F_INFO *fileinfo, struct stat *s_buf)
{
   if(s_buf->st_mode & S_IRUSR )
   {
     fileinfo->rwx[1] = 'r';
   }
   else
   {
     fileinfo->rwx[1] = '-';
   }
   if(s_buf->st_mode & S_IWUSR )
   {
     fileinfo->rwx[2] = 'w';
   }
   else
   {
     fileinfo->rwx[2] = '-';
   }
   if(s_buf->st_mode & S_IXUSR )
   {
     fileinfo->rwx[3] = 'x';
   }
   else
   {
     fileinfo->rwx[3] = '-';
   }
   if(s_buf->st_mode & S_IRGRP)
   {
     fileinfo->rwx[4] = 'r';
   }
   else
   {
     fileinfo->rwx[4] = '-';
   }
   if(s_buf->st_mode & S_IWGRP)
   {
     fileinfo->rwx[5] = 'w';
   }
   else
   {
     fileinfo->rwx[5] = '-';
   }
   if(s_buf->st_mode & S_IXGRP)
   {
     fileinfo->rwx[6] = 'x';
   }
   else
   {
     fileinfo->rwx[6] = '-';
   }
   if(s_buf->st_mode & S_IROTH)
   {
     fileinfo->rwx[7] = 'r';
   }
   else
   {
     fileinfo->rwx[7] = '-';
   }
   if(s_buf->st_mode & S_IWOTH)
   {
     fileinfo->rwx[8] = 'w';
   }
   else
   {
     fileinfo->rwx[8] = '-';
   }
   if(s_buf->st_mode & S_IXOTH)
   {
     fileinfo->rwx[9] = 'x';
   }
   else
   {
     fileinfo->rwx[9] = '-';
   }
}
void linknum(F_INFO *fileinfo, struct stat *s_buf)
{
   fileinfo->linknum = s_buf->st_nlink; 
}
void usr_group(F_INFO *fileinfo, struct stat *s_buf)
{
   struct passwd *p_info;
   struct group *g_info;
   p_info = getpwuid(s_buf->st_uid);
   g_info = getgrgid(s_buf->st_gid);
   fileinfo->usr = p_info->pw_name;
   fileinfo->group = g_info->gr_name;
}
void filesize(F_INFO *fileinfo, struct stat *s_buf)
{
  fileinfo->file_size = s_buf->st_size;
}
void ftime(F_INFO *fileinfo, struct stat *s_buf)
{
   fileinfo->ftime = ctime(&(s_buf->st_atime));
}
void fname(F_INFO *fileinfo, char *argv[])
{
   fileinfo->fname = argv[1]; 
}
void showallinfo(F_INFO *fileinfo)
{
  printf("%s %u %s %s %ld %s %s\n",
      fileinfo->rwx,
      fileinfo->linknum,
      fileinfo->usr,
      fileinfo->group,
      fileinfo->file_size,
      fileinfo->fname,
      fileinfo->ftime
      );
}
int main(int argc, char *argv[])
{
   //使用stat函数获取inode信息用来查看文件权限位
   struct stat s_buf;
   int i = 0;
   if(argc < 2)
   {
      printf("./myls file\n");
      exit(1);
   }
   i = lstat(argv[1], &s_buf);
   if(i < 0)
   {
      perror("stat\n");
      exit(1);
   }
   rwx(&fileinfo, &s_buf);   
   linknum(&fileinfo, &s_buf);
   usr_group(&fileinfo, &s_buf);
   filesize(&fileinfo, &s_buf);
   ftime(&fileinfo, &s_buf);
   fname(&fileinfo, argv);
   showallinfo(&fileinfo);
   return 0;
}

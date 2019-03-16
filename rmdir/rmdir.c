#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>

void enterdir(char *dir, void(*fuc)(char *))
{
  DIR *dfd;
  struct dirent *d_info; 
  char name[1024];
  if((dfd = opendir(dir)) == NULL)
  {
    fprintf(stderr, "enterdir can't open %s\n", dir);
    return;
  }
  while((d_info = readdir(dfd)) != NULL) 
  {
    if(strcmp(d_info->d_name, ".") == 0 ||
        strcmp(d_info->d_name, "..") == 0)
    {
      continue;
    }
    else 
    {
      //这里使用sprintf将路径联系起来,才能正确调用函数
      //直接传递文件名,会找不到该文件
      sprintf(name,"%s/%s",dir, d_info->d_name);
      (*fuc)(name);
    }
  }
  rmdir(dir);
}
void rmfile(char *file)
{
  struct stat s_buf;
  int reslt = stat(file, &s_buf);
  if(reslt == -1)
  {
    fprintf(stderr, "rmfile can't access %s\n", file);
    return;
  }
  if((s_buf.st_mode & S_IFMT) == S_IFDIR)
  {
    enterdir(file, rmfile);
  }
  unlink(file);
}
int main(int argc, char *argv[])
{
  //打开用户指定的目录,把除了. 和 .. 以外的普通文件都删除
  //如果该目录下有子目录,就进入该子目录,重复上述过程
  //直到所有文件包括用户指定的目录全部删除
  int i = 0;
  if(argc < 2)
  {
    printf("./rmdir dir");
  }
  else 
  {
    while(--argc > 0)
    {
      rmfile(argv[++i]);
    }
  }
  return 0;
}

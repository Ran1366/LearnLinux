#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int cmp ( const void *a , const void *b  )
{
  return *(int *)a - *(int *)b;  //升序排序
}
int main()
{
  int buf[1000] = {0};
  //fd_src获取src的文件描述符
  int fd_src = open("src", O_CREAT | O_RDONLY, 0666);
  //fd_in获取标准输入文件描述符的副本
  int fd_in = dup(STDIN_FILENO);
  //将标准输入的文件描述符释放,使文件描述符0其指向fd_src
  //指向的文件
  dup2(fd_src, STDIN_FILENO);
  int i = 0;
  //从文件src读取1000个随机数并升序排序
  //scanf默认从文件描述符为0指向的地方获得数据
  while(scanf("%d", buf + i++) != EOF);
  qsort(buf, 1000, sizeof(buf[0]), cmp); 
  //获得des的文件描述符
  int fd_des = open("des", O_CREAT | O_WRONLY, 0666);
  //取得标准输出文件描述符的备份
  int fd_out = dup(STDOUT_FILENO);
  //释放标准输出文件描述符,使文件描述符1指向fd_des指向
  //的文件
  dup2(fd_des, STDOUT_FILENO);
  for(i = 0; i < 1000; ++i)
  {
    //printf默认向文件描述符1指向的地方打印数据
    printf("%d ", buf[i]);
  }
  printf("\n");
  return 0;
}

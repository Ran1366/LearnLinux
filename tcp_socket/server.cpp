#include "tcp_socket.hpp"
#include <iostream>
#include <pthread.h>

bool ConverseStr(std::string &buf, Data *data)
{
  std::string num1;
  std::string num2;
  int i;
  //暂时只支持正数的运算,用户可以自由输入
  //如果要支持正负数,从规定client端输入方式入手会很简单
  for(i = 0; i < (int)buf.size(); ++i)
  {
    if(buf[i] >= '0' && buf[i] <= '9')  
    {
      num1 += buf[i];
    }
    else if(buf[i] == '+' || buf[i] == '-' 
            || buf[i] == '*' || buf[i] == '/')
    {
      data->Operator = buf[i]; 
      break;
    }
  }
  for(; i < (int)buf.size(); ++i)
  {
    if(buf[i] >= '0' && buf[i] <= '9')  
    {
      num2 += buf[i];
    }
  }
  data->num1 = atoi(num1.c_str());
  data->num2 = atoi(num2.c_str());
  if(data->num2 == 0 && data->Operator == '/')
  {
    return false;
  }
  return true;
}

double Count(Data *data)
{
  if(data->Operator == '+')
  {
    return data->num1 + data->num2;
  }
  else if(data->Operator == '-')
  {
    return data->num1 - data->num2;
  }
  else if(data->Operator == '*')
  {
    return data->num1 * data->num2;
  }
  else 
  {
    if(data->num1 < 0 && data->num2 < 0)
    {
      data->num1 *= -1;
      data->num2 *= -1;
      return data->num1 / ((data->num2)*1.0);
    }
    else if(data->num1 < 0 && data->num2 > 0)
    {
      data->num1 *= -1;
      return -(data->num1 / ((data->num2)*1.0));
    }
    else if(data->num2 < 0 && data->num1 > 0)
    {
      data->num2 *= -1;
      return -(data->num1 / ((data->num2)*1.0));
    }
    else 
    {
      return data->num1 / ((data->num2)*1.0);
    }
  }
}

void *th_fuc(void * sock)
{
  tcp_socket *c_sock = (tcp_socket*)sock;
  std::string buf;
  while(1)
  {
    if(c_sock->Recv(buf) == false) 
    {
      return (void*)-1;
    }
    //解析字符串
    Data *data = new Data;
    if(ConverseStr(buf, data) == false)
    {
      buf = "输入错误!";
      c_sock->Send(buf);
    }
    else 
    {
      //计算并返回
      double ret = Count(data); 
      char tmp[20];
      sprintf(tmp, "%lf", ret);
      buf = tmp;
      c_sock->Send(buf);
    }
  }
  c_sock->Close();
}

int main(int argc, char *argv[])
{
  if(argc != 3)
  {
    perror("./server ip port");
    return -1;
  }
  
  std::string ip = argv[1];
  uint16_t port = atoi(argv[2]);
    
  tcp_socket sock;
  //创建套接字
  CHECK_RIGHT(sock.Socket()); 
  //绑定ip地址和端口号
  CHECK_RIGHT(sock.Bind(ip, port));
  //监听
  CHECK_RIGHT(sock.Listen());

  pthread_t tid;

  while(1)
  {
    tcp_socket *c_sock = new tcp_socket; 
    if(sock.Accept(*c_sock) != false)
    {
      //创建线程后设置其为分离态
      pthread_create(&tid, NULL, th_fuc, (void*)c_sock);
      pthread_detach(tid);
    }
  }
  sock.Close();
  return 0;
}

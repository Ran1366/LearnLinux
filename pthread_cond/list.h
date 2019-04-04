#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct Node
{
  int val;
  struct Node* Next;
}Node;

//头插
Node *frontInsert(Node *head, int v)
{
 Node *newNode = (Node *)malloc(sizeof(Node)); 
  if(head == NULL)
  {
    head = newNode;
    head->val = v;
    head->Next = head;
    return head;
  }
  newNode->val = v;
  newNode->Next = head;
  head = newNode;
  return head;
}
//头删
Node *frontDelete(Node *head)
{
  if(head == NULL)
  {
    return NULL;
  }
  Node *deleted = head;
  head = head->Next;
  deleted->Next = NULL;
  return deleted;
}
//尾插
Node *backInsert(Node *head, int v)
{
  Node *result = head;
  //如果链表为空,则把新结点作为第一个结点
  if(head == NULL)
  {
    head->Next = head;
    head->val = v;
  }
  //如果链表只有一个结点
  Node *newNode = (Node *)malloc(sizeof(Node)); 
  newNode->val = v;
  newNode->Next = NULL;
  if(head->Next == NULL)
  {
    head->Next = newNode;
  }
  Node *cur = head;
  //循环直到找到最后一个结点的位置
  while(cur->Next != NULL) 
  {
    cur = cur->Next;
  }
  cur->Next = newNode;
  return result; 
}

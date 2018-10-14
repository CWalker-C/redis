/* adlist.h - A generic doubly linked list implementation
 *
 * Copyright (c) 2006-2012, Salvatore Sanfilippo <antirez at gmail dot com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Redis nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __ADLIST_H__
#define __ADLIST_H__

/**
 * 以下是对链表结点、链表、迭代器的声明
 **/

// 链表结点
typedef struct listNode {
    struct listNode *prev;  // 指向前驱结点的指针
    struct listNode *next;  // 指向后继结点的指针
    void *value;    // 节点中存储的值，void* 为万能指针，可以通过类型转换，指向其他类型
} listNode;

// 迭代器的声明
typedef struct listIter {   
    listNode *next; // 迭代器指向的当前结点的指针
    int direction;  // 迭代器的方向，可以取以下两个值：AL_START_HEAD（从表头到表尾方向）和AL_START_TAIL（从表尾到表头的方向），下文可见
} listIter;

// 链表的声明
typedef struct list {
    listNode *head; // 指向首结点的指针，即头指针
    listNode *tail; // 指向尾结点的指针
    void *(*dup)(void *ptr);    // 用于复制结点的函数指针
    void (*free)(void *ptr);    // 用于释放结点的函数指针
    int (*match)(void *ptr, void *key); // 用于判断ptr 所指向的结点的值与key指向的值是否相等
    unsigned long len;  //链表长度
} list;


/**
 * 使用宏定义的函数
 **/
#define listLength(l) ((l)->len)    // 返回链表l 长度
#define listFirst(l) ((l)->head)    // 返回链表l 首结点的地址
#define listLast(l) ((l)->tail)     // 返回链表l 表尾结点的地址
#define listPrevNode(n) ((n)->prev) // 返回结点n 的前驱结点的地址
#define listNextNode(n) ((n)->next) // 返回结点n 的后继结点的地址
#define listNodeValue(n) ((n)->value)   // 返回结点n 值的地址

#define listSetDupMethod(l,m) ((l)->dup = (m))  // 为链表l 设置复制函数(即函数m)
#define listSetFreeMethod(l,m) ((l)->free = (m))    //为链表l 设置释放函数（即函数m）
#define listSetMatchMethod(l,m) ((l)->match = (m))  //为链表l 设置比较结点值与给定值是否相等的函数（即函数m） 

#define listGetDupMethod(l) ((l)->dup)  // 获取链表的复制函数
#define listGetFree(l) ((l)->free)  // 获取链表的释放函数
#define listGetMatchMethod(l) ((l)->match)  // 获取链表的比较函数

/**
 * 与链表有关函数声明
 **/
list *listCreate(void); // 创建空链表
void listRelease(list *list);   // 释放链表
void listEmpty(list *list); // 判断链表是否为空
list *listAddNodeHead(list *list, void *value); // 头插法向链表中插入结点
list *listAddNodeTail(list *list, void *value); // 尾插法向链表中插入结点
list *listInsertNode(list *list, listNode *old_node, void *value, int after);
void listDelNode(list *list, listNode *node);
listIter *listGetIterator(list *list, int direction);
listNode *listNext(listIter *iter);
void listReleaseIterator(listIter *iter);
list *listDup(list *orig);
listNode *listSearchKey(list *list, void *key);
listNode *listIndex(list *list, long index);
void listRewind(list *list, listIter *li);
void listRewindTail(list *list, listIter *li);
void listRotate(list *list);
void listJoin(list *l, list *o);

/**
 * 迭代器方向
 **/
#define AL_START_HEAD 0
#define AL_START_TAIL 1

#endif /* __ADLIST_H__ */

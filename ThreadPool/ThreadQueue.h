#ifndef H_THREDQUEUE
#define H_THREDQUEUE
#include"ThreadNode.h"
#include<stdio.h>
class ThreadQueue{
public:
    ThreadQueue();
    ~ThreadQueue();

public:
    bool isEmpty() {return mSize == 0;};
    ThreadNode *addThreadNodeToHead(ThreadNode *tn);
    ThreadNode *deleteThreadNodeFromTail();
    ThreadNode *deleteThreadNode(ThreadNode *tn);
    ThreadNode *findThreadNode(ThreadNode *tn);
private:
    void deleteAllThreadNode();

private:
    int mSize;
    ThreadNode *mHead;
    ThreadNode *mTail;
};
#endif

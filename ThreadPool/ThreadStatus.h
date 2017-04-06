#ifndef H_THREADSTATUS
#define H_THREADSTATUS
#include"ThreadNode.h"
class ThreadStatus{
public:
    virtual void BeginRun() = 0;
    virtual void RunEnd(ThreadNode *tn) = 0;
};
#endif

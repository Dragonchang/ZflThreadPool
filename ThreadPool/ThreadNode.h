#ifndef H_THREADNODE
#define H_THREADNODE
class NThread;
class Handler;
class Task;
class ThreadNode {
public: 
    ThreadNode();
    virtual ~ThreadNode();

public:
   void RunTask(Task *task);

public:
    NThread *mThread;
    Handler *mHandler;
    ThreadNode *mNext;
};
#endif

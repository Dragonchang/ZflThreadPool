#ifndef H_THREADNODE
#define H_THREADNODE
class NThread;
class Handler;
class Task;
class ThreadStatus;
//add tag for every node at threadPool manager
class ThreadNode {
public:
    ThreadNode();
    ThreadNode(ThreadStatus *threadCallBack);
    virtual ~ThreadNode();

public:
   void RunTask(Task *task);

private:
    NThread *mThread;
    Handler *mHandler;

public:
    ThreadNode *mNext;
    ThreadNode *mBefore;
};
#endif

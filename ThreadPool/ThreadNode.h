#ifndef H_THREADNODE
#define H_THREADNODE
class NThread;
class Handler;
class Task;
class ThreadStatus;
class ThreadNode {
public:
    ThreadNode() {};
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

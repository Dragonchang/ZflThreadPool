#ifndef H_THREDPOOLMANAGER
#define H_THREDPOOLMANAGER
#include "ThreadStatus.h"
class NThread;
class Handler;
class ThreadNode;
class ThreadQueue;
class Task;
//need more feature
//1.extend the idle queue when idle queue is empty
//2.create a delay message loop to check the idle queue if too large
class ThreadPoolManager: public ThreadStatus {
public:
    ThreadPoolManager();
    ThreadPoolManager(int size);
    ~ThreadPoolManager();

public:
    virtual void BeginRun();
    virtual void RunEnd(ThreadNode *tn);

public:
    void startThreadPool();
    void beginExecuteTask(Task *task);

    void handlerStartThreadPool();
    void handlerBeginExecuteTask(Task *task);
    void handlerRunEnd(ThreadNode *tn);
public:
    enum {
        STARTTHREADPOOL  = 0,
        BEGINRUNTASK     = 1,
        TASKRUNOVER      = 2
    };

private:
    ThreadNode *getThreadNodeFromIdleQueue();
    void setThreadNodeBusy(ThreadNode * tn);
    ThreadNode *getThreadNodeFromBusyQueue(ThreadNode *tn);
    void setThreadNodeIdle(ThreadNode * tn);
private:
    ThreadQueue *mBusyQueue;
    ThreadQueue *mIdleQueue;

    NThread *mThread;
    Handler *mHandler;
    int mDefaultSize;
};
#endif



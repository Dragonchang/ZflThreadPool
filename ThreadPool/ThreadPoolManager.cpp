#include"ThreadPoolManager.h"
#include "ThreadNode.h"
#include "ThreadQueue.h"
#include "ThreadStatus.h"
#include "../Handler/NThread.h"
#include "../Handler/Handler.h"
#define DEFAULTSIZE  10;
class MThreadPoolHandler: public Handler {

    public:
    MThreadPoolHandler(Looper* looper, ThreadPoolManager *threadPoolManager)
    :Handler(looper){
        mThreadPoolManager = threadPoolManager;
    }
    void handlerMessage(Message* message) {
        if (message != NULL) {
	    switch (message->what) {
		case ThreadPoolManager::STARTTHREADPOOL:
                    mThreadPoolManager->handlerStartThreadPool();
		    break;
		case ThreadPoolManager::BEGINRUNTASK:
                    mThreadPoolManager->handlerBeginExecuteTask(static_cast<Task *>(message->mObj));
		    break;
		case ThreadPoolManager::TASKRUNOVER:
                    mThreadPoolManager->handlerRunEnd(static_cast<ThreadNode *>(message->mObj));
		    break;
	    }
        }

    }
    private:
       ThreadPoolManager *mThreadPoolManager;
};

ThreadPoolManager::ThreadPoolManager() {
    mDefaultSize = DEFAULTSIZE;
    mBusyQueue = new ThreadQueue();
    mIdleQueue = new ThreadQueue();
    mThread = new NThread();
    mHandler = new MThreadPoolHandler(mThread->getLooper(), this);
}

ThreadPoolManager::ThreadPoolManager(int size) {
    mDefaultSize = size;
    mBusyQueue = new ThreadQueue();
    mIdleQueue = new ThreadQueue();
    mThread = new NThread();
    mHandler = new MThreadPoolHandler(mThread->getLooper(), this);
}

ThreadPoolManager::~ThreadPoolManager() {
    if (mHandler != NULL) {
        delete mHandler;
        mHandler = NULL;
    }
    //must stop the poolmanger thread before delete tn Queue
    //destruct function maybe call by user thread
    if (mThread != NULL) {
        delete mThread;
        mThread = NULL;
    }

    if (mBusyQueue != NULL) {
        delete mBusyQueue;
        mBusyQueue = NULL;
    }
    if (mIdleQueue != NULL) {
        delete mIdleQueue;
        mIdleQueue = NULL;
    }

}

//create thread node for busy and idle queue
void ThreadPoolManager::startThreadPool() {
    Message* message = Message::obtain(STARTTHREADPOOL);
    if (mHandler != NULL) {
        mHandler->sendMessage(message);
    }
}

//run user task
//call from user
void ThreadPoolManager::beginExecuteTask(Task *task) {
    Message* message = Message::obtain(BEGINRUNTASK, task);
    if (mHandler != NULL) {
        mHandler->sendMessage(message);
    }
}

void ThreadPoolManager::BeginRun() {

}

//user task run end
//call from threadPool thread
void ThreadPoolManager::RunEnd(ThreadNode *tn) {
    Message* message = Message::obtain(TASKRUNOVER, tn);
    if (mHandler != NULL) {
        mHandler->sendMessage(message);
    }
}

void ThreadPoolManager::handlerStartThreadPool() {
    for (int i = 0; i < mDefaultSize; i++) {
        if (mIdleQueue != NULL) {
            mIdleQueue->addThreadNodeToHead(new ThreadNode(this));
        }
    }
}

void ThreadPoolManager::handlerBeginExecuteTask(Task *task) {
    ThreadNode * tn = getThreadNodeFromIdleQueue();
    if (tn == NULL) {
        //FIX ME
        //Idle Queue without ThreadNode
        //return;
        tn = new ThreadNode(this);
    }
    setThreadNodeBusy(tn);
    tn->RunTask(task);
}

void ThreadPoolManager::handlerRunEnd(ThreadNode *tn) {
     if (getThreadNodeFromBusyQueue(tn) == NULL) {
         //FIX ME
         //ThreadNode was not in Busy queue
         return;
     }
     setThreadNodeIdle(tn);
}

ThreadNode *ThreadPoolManager::getThreadNodeFromIdleQueue() {
    ThreadNode *idleThread = NULL;
    if (mIdleQueue!= NULL) {
       idleThread = mIdleQueue->deleteThreadNodeFromTail();
    }
    return idleThread;
}

void ThreadPoolManager::setThreadNodeBusy(ThreadNode * tn) {
    if (mBusyQueue != NULL) {
        mBusyQueue->addThreadNodeToHead(tn);
    }
}

ThreadNode *ThreadPoolManager::getThreadNodeFromBusyQueue(ThreadNode *tn) {
    ThreadNode *busyThread = NULL;
    if (mBusyQueue!= NULL) {
       busyThread = mBusyQueue->deleteThreadNode(tn);
    }
    return busyThread;
}

void ThreadPoolManager::setThreadNodeIdle(ThreadNode * tn) {
    if (mIdleQueue != NULL) {
        mIdleQueue->addThreadNodeToHead(tn);
    }

}

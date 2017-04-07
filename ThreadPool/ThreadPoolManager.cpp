#include"ThreadPoolManager.h"
#include "ThreadNode.h"
#include "ThreadQueue.h"
#include "ThreadStatus.h"
#include "../Handler/NThread.h"
#include "../Handler/Handler.h"
class MyHandler: public Handler {

    public:
    MyHandler(Looper* looper, ThreadPoolManager *threadPoolManager)
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

}

ThreadPoolManager::~ThreadPoolManager() {

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

}

void ThreadPoolManager::handlerBeginExecuteTask(Task *task) {
    ThreadNode * tn = getThreadNodeFromIdleQueue();
    if (tn == NULL) {
        //FIX ME
        //Idle Queue without ThreadNode
        return;
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

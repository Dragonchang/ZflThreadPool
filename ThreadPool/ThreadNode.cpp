#include "ThreadNode.h"
#include "Task.h"
#include "ThreadStatus.h"
#include "../Handler/NThread.h"
#include "../Handler/Handler.h"
class MyHandler: public Handler {

    public:
    MyHandler(Looper* looper, ThreadStatus *threadCallBack, ThreadNode *tn)
    :Handler(looper){
        mThreadCallBack = threadCallBack;
        mThreadNode = tn;
    }
    void handlerMessage(Message* message) {
        if (message != NULL) {
            Task *myTask = (Task *)message->mObj;
            if (myTask != NULL) {
               myTask->run(); 
               if (mThreadCallBack!= NULL) {
                   mThreadCallBack->RunEnd(mThreadNode);
               }
            }
        }

    }
    private:
       ThreadStatus *mThreadCallBack;
       ThreadNode *mThreadNode;
};

ThreadNode::ThreadNode(ThreadStatus *threadCallBack) {
     mThread = new NThread();
     mHandler  = new MyHandler(mThread->getLooper(), threadCallBack, this);
}

ThreadNode::ThreadNode() {
     mThread = NULL;
     mHandler  = NULL;
}

ThreadNode::~ThreadNode() {
    if (mHandler != NULL) {
        delete mHandler;
        mHandler = NULL;
    }
    if (mThread != NULL) {
        delete mThread;
        mThread = NULL;
    }
}

void ThreadNode::RunTask(Task *task) {
    Message* message = Message::obtain(0, task);
    if (mHandler != NULL) {
        mHandler->sendMessage(message);
    }
}

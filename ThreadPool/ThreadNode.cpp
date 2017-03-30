#include "ThreadNode.h"
#include "Task.h"
#include "../Handler/NThread.h"
#include "../Handler/Handler.h"
class MyHandler: public Handler {

    public:
    MyHandler(Looper* looper)
    :Handler(looper){

    }
    void handlerMessage(Message* message) {
        if (message != NULL) {
            Task *myTask = (Task *)message->mObj;
            if (myTask != NULL) {
               myTask->run(); 
            }
        }

    }
};

ThreadNode::ThreadNode() {
    mThread = new NThread();
    mHandler  = new MyHandler(mThread->getLooper());
}

ThreadNode::~ThreadNode() {
    if (mHandler != NULL) {
        delete mHandler;
        mHandler = NULL;
    }
    if (mThread != NULL) {
        if (mThread->IsRun()) {
            if (mThread->getLooper() != NULL)
                mThread->getLooper()->quit(true);
        }
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

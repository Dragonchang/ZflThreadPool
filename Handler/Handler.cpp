#include"Handler.h"
#include "../Utils/Utils.h"
#define DEBUG 0
Handler::Handler(Looper* looper){
    mLooper = looper;
}

Handler::~Handler(){

}

void Handler::sendMessage(Message *message){
    if (DEBUG) printf("tid:%d Handler::sendMessage what =%d\n",(unsigned)pthread_self() ,message->what);
    if (message != NULL) {
        message->setTarget(this);
        message->when = getCurrentTime();
    } else {
        printf("Handler::sendMessage message is null\n");
        return;
    }
    if (mLooper != NULL) {
        mLooper->enqueueMessage(message);
    } else {
        printf("looper is null\n");
    }
}

void Handler::sendMessageDelayed(Message* message, long delayMillis) {
    if (DEBUG) printf("tid:%d Handler::sendMessage what =%d\n",(unsigned)pthread_self() ,message->what);
    if (message != NULL) {
        message->setTarget(this);
        message->when = getCurrentTime() + delayMillis;
    } else {
        printf("Handler::sendMessage message is null\n");
        return;
    }
    if (mLooper != NULL) {
        mLooper->enqueueMessage(message);
    } else {
        printf("looper is null\n");
    }
}

void Handler::handlerMessage(Message *message){
    if (DEBUG) printf("tid:%d Handler::handlerMessage what =%d\n",(unsigned)pthread_self() ,message->what);
    sleep(2);
}

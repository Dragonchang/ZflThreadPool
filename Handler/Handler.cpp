#include"Handler.h"
Handler::Handler(Looper* looper){
    mLooper = looper;
}

Handler::~Handler(){

}

void Handler::sendMessage(Message *message){
    printf("tid:%d Handler::sendMessage what =%d\n",(unsigned)pthread_self() ,message->what);
    message->setTarget(this);
    if (mLooper != NULL) mLooper->getMessageQueue()->enqueueMessage(message);
}

void Handler::handlerMessage(Message *message){
    printf("tid:%d Handler::handlerMessage what =%d\n",(unsigned)pthread_self() ,message->what);
    sleep(2);
}

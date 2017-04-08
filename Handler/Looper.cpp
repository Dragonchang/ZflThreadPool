#include "Looper.h"
#define DEBUG 0
static pthread_key_t gTLSKey = 0;
Looper::Looper() {
    mMessageQueue = new MessageQueue();
    int result = pthread_key_create(& gTLSKey, NULL);
}

Looper::~Looper() {
   quit(true);
   if (mMessageQueue != 0) {
      delete mMessageQueue;
      mMessageQueue = 0;
   }
}

void Looper::setForThread(Looper* looper){
    pthread_setspecific(gTLSKey, looper);
}

Looper* Looper::getForThread(){
    return (Looper*)pthread_getspecific(gTLSKey);
}

void Looper::loop() {
    Message * message;
    Looper::setForThread(this);
    for (;;) {
        //printf("tid:%d Looper::loop \n",(unsigned)pthread_self());
        message = mMessageQueue->getNextMessage();
        if (message != NULL) {
            if(DEBUG) printf("tid:%d Looper::loop message != NULL\n",(unsigned)pthread_self());
            message->mTarget->handlerMessage(message);
            delete message;
        } else {
            if(DEBUG) printf("tid:%d Looper::loop message == NULL\n",(unsigned)pthread_self());
            break;
        }
    }
}

void Looper::quit(bool removeAllMessage) {
    if (removeAllMessage) {
        mMessageQueue->removeAllMessage();
    }
    mMessageQueue->wake();
}

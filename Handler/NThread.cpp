#include"NThread.h"
NThread::NThread(){
    int res = sem_init(&bin_sem,0,0);
    mLooper = new Looper();
    printf("tid:%d NThread::NThread\n", (unsigned)pthread_self());
    pthread_create(&mTid,0,ThreadLoop,this);
    sem_wait(&bin_sem);
}

NThread::~NThread(){
    if(mLooper != NULL) {
        delete mLooper;
        mLooper = NULL;
    }
    sem_destroy(&bin_sem);
}

void* NThread::ThreadLoop(void* arg){
    printf("tid:%d NThread::ThreadLoop\n", (unsigned)pthread_self());
    NThread *ptr = (NThread *)arg;
    ptr->Run();
}

void NThread::Run(){
    sem_post(&bin_sem);
    getLooper()->loop();
}

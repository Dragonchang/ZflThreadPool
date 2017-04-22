#include "./Handler/Handler.h"
#include "./Handler/Message.h"
#include "./Handler/NThread.h"
#include "./Utils/Condition.h"
#include "./Utils/Meutex.h"
#include "./ThreadPool/Task.h"
#include "./ThreadPool/ThreadPoolManager.h"
#include <sys/time.h>
#include <unistd.h>
int mCount = 10;
Mutex mLock1;
Condition mCondition1;
long t(){
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec*1000 + tv.tv_usec/100;
}
/*****************
1.线程的退出:子线程处理完消息退出,其它线程命令其退出.
2.looper在子线程中的使用
  創建NThread對象.
  使用該對象中的Looper 來創建處理消息的handler
3.looper在主线程中的使用
   new 一個Looper對象
   使用該looper對象創建一個處理消息的handler
   調用loop() 進入循環
4.将looper对象保存在TLS中,这样可以在子线程中handlerMessage中创建的对象中创建hanlder进行消息的处理
************************/
class TestHandler: public Handler {

    public:
    TestHandler(Looper* looper)
    :Handler(looper){

    }
    void handlerMessage(Message *message) {
        printf("tid:%d *****testHandler::handlerMessage what =%d\n",(unsigned)pthread_self() ,message->what);
        if(9 == message->what) {
            //可能需要创建一个对象处理某个事务同时该对象中某个逻辑需要sendmessage到该线程的消息队列中进行处理
            //怎样获取looper对象其实就是Messagequeue
            //通过TLS保存looper对象这样所有子线程所有的地方都可以获取looper对象来创建handler
            Message* message = Message::obtain(1000);
            sendMessageDelayed(message,10000);
        }else if (1000 == message->what) {
            Looper::getForThread()->quit(true);//TLS 存儲looper對象Looper::getForThread()来创建其它的handler
            printf("testHandler::handlerMessag broadcast main thread %ld\n",t());
            mCondition1.broadcast();
        } else {
            //sleep(1);
        }
    }
};
//該示例是looper在子线程中的使用case
void TestHandlerAndLoop_1() {
    printf("TestHandlerAndLoop_1-begin>>>>>>>>>>> %ld\n",t());
    mCount = 10;
    NThread thread;
    TestHandler handler(thread.getLooper());
    while(mCount--) {
        Message* message = Message::obtain(mCount);
        handler.sendMessage(message);
    }
    printf("TestHandlerAndLoop_1-begin wait %ld\n",t());
    mCondition1.wait(mLock1);
    printf("TestHandlerAndLoop_1-END<<<<<<<<<<<<<< %ld\n",t());
}

void TestHandlerAndLoop_2() {
    printf("TestHandlerAndLoop_2-begin>>>>>>>>>>>>%ld\n",t());
    mCount = 10;
    NThread thread;
    TestHandler handler(thread.getLooper());
    while(mCount--) {
        Message* message = Message::obtain(mCount);
        handler.sendMessageDelayed(message,mCount*500);
    }
    printf("TestHandlerAndLoop_2-begin wait %ld\n",t());
    mCondition1.wait(mLock1);
    printf("TestHandlerAndLoop_2-END <<<<<<<<<<<<< %ld\n",t());
}
void testLooper() {
    TestHandlerAndLoop_1();
    TestHandlerAndLoop_2();

}

Mutex mLock;
Condition mCondition;
int taskNum = 0;
int taskindex = 0;
class TestTask: public Task {
    public:
    void run() {
        sleep(1);
        taskindex = taskindex + 1;
        printf("this (%d) testtask run function; %ld\n",taskindex,t());
        if (taskindex == taskNum) {
            printf("all the task run end %ld\n",t());
            mCondition.broadcast();
        }
    }
};

// 测试thread pool
void TestThreadPool_start1() {
    printf("TestThreadPool_start1-begin %ld\n",t());
    ThreadPoolManager *threadpoolmanager = new ThreadPoolManager();
    delete threadpoolmanager;
    printf("TestThreadPool_start1-end %ld\n",t());
}

void TestThreadPool_start2() {
    printf("TestThreadPool_start2-begin %ld\n",t());
    ThreadPoolManager *threadpoolmanager = new ThreadPoolManager(100);
    delete threadpoolmanager;
    printf("TestThreadPool_start2-end %ld\n",t());
}

void TestThreadPool_start3() {
    printf("TestThreadPool_start3-begin %ld\n",t());
    ThreadPoolManager *threadpoolmanager = new ThreadPoolManager();
    threadpoolmanager->handlerStartThreadPool();
    delete threadpoolmanager;
    printf("TestThreadPool_start3-end %ld\n",t());
}

void TestThreadPool_start4() {
    printf("TestThreadPool_start4-begin %ld\n",t());
    ThreadPoolManager *threadpoolmanager = new ThreadPoolManager(100);
    threadpoolmanager->handlerStartThreadPool();
    delete threadpoolmanager;
    printf("TestThreadPool_start4-end %ld\n",t());
}

//task number littler than threadpool queue size
void TestThreadPool_runtask1() {
    printf("TestThreadPool_runtask1_begin %ld\n",t());
    Mutex::Autolock _l(mLock);
    ThreadPoolManager *threadpoolmanager = new ThreadPoolManager();
    threadpoolmanager->handlerStartThreadPool();
    Task *testTask = new TestTask();
    taskindex = 0;
    taskNum = 5;
    for (int i = 0; i<taskNum; i++) {
	threadpoolmanager->beginExecuteTask(testTask);
    }
    printf("TestThreadPool_runtask1 begin waite %ld\n",t());
    mCondition.wait(mLock);
    printf("TestThreadPool_runtask1 end waite %ld\n",t());
    delete testTask;
    delete threadpoolmanager;
    printf("TestThreadPool_runtask1 end %ld\n",t());
}


//task number bigger than threadpool queue size
void TestThreadPool_runtask2() {
    printf("TestThreadPool_runtask2_begin %ld\n",t());
    Mutex::Autolock _l(mLock);
    ThreadPoolManager *threadpoolmanager = new ThreadPoolManager();
    threadpoolmanager->handlerStartThreadPool();
    Task *testTask = new TestTask();
    taskindex = 0;
    taskNum = 200;
    for (int i = 0; i<taskNum; i++) {
	threadpoolmanager->beginExecuteTask(testTask);
    }
    printf("TestThreadPool_runtask2 begin waite %ld\n",t());
    mCondition.wait(mLock);
    printf("TestThreadPool_runtask2 end waite %ld\n",t());
    delete testTask;
    printf("TestThreadPool_runtask2 delete testTask %ld\n",t());
    delete threadpoolmanager;
    printf("TestThreadPool_runtask2 delete threadpoolmanager %ld\n",t());
}

//task number bigger than threadpool queue size
//reuse the idle tn
void TestThreadPool_runtask3() {
    printf("TestThreadPool_runtask3_begin %ld\n",t());
    Mutex::Autolock _l(mLock);
    ThreadPoolManager *threadpoolmanager = new ThreadPoolManager();
    threadpoolmanager->handlerStartThreadPool();
    Task *testTask = new TestTask();
    taskindex = 0;
    taskNum = 50;
    for (int i = 0; i<taskNum; i++) {
        sleep(2);
	threadpoolmanager->beginExecuteTask(testTask);
    }
    printf("TestThreadPool_runtask3 begin waite %ld\n",t());
    mCondition.wait(mLock);
    printf("TestThreadPool_runtask3 end waite %ld\n",t());
    delete testTask;
    printf("TestThreadPool_runtask3 delete testTask %ld\n",t());
    delete threadpoolmanager;
    printf("TestThreadPool_runtask3 delete threadpoolmanager %ld\n",t());
}

//task number bigger than threadpool queue size
//reuse the idle tn
void TestThreadPool_runtask4() {
    printf("TestThreadPool_runtask4_begin %ld\n",t());
    Mutex::Autolock _l(mLock);
    ThreadPoolManager *threadpoolmanager = new ThreadPoolManager();
    threadpoolmanager->handlerStartThreadPool();
    Task *testTask = new TestTask();
    taskindex = 0;
    taskNum = 50;
    for (int i = 0; i<taskNum; i++) {
        if (i%5 == 0) {
            sleep(2);
        }
	threadpoolmanager->beginExecuteTask(testTask);
    }
    printf("TestThreadPool_runtask4 begin waite %ld\n",t());
    mCondition.wait(mLock);
    printf("TestThreadPool_runtask4 end waite %ld\n",t());
    delete testTask;
    printf("TestThreadPool_runtask4 delete testTask %ld\n",t());
    delete threadpoolmanager;
    printf("TestThreadPool_runtask4 delete threadpoolmanager %ld\n",t());
}

void testThreadPool() {
    TestThreadPool_start1();
    TestThreadPool_start2();
    TestThreadPool_start3();
    TestThreadPool_start4();
    TestThreadPool_runtask1();
    TestThreadPool_runtask2();
    TestThreadPool_runtask3();
    TestThreadPool_runtask4();
}
int main() {
    testLooper();
    testThreadPool();
    return 0;
}

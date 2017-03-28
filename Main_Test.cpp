#include "./Handler/Handler.h"
#include "./Handler/Message.h"
#include "./Handler/NThread.h"
int mCount = 1;
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
class MyHandler: public Handler {

    public:
    MyHandler(Looper* looper)
    :Handler(looper){

    }
    void handlerMessage(int what) {
        printf("tid:%d MyHandler::handlerMessage what =%d\n",(unsigned)pthread_self() ,what);
        if(9 == what) {
            //可能需要创建一个对象处理某个事务同时该对象中某个逻辑需要sendmessage到该线程的消息队列中进行处理
            //怎样获取looper对象其实就是Messagequeue
            //通过TLS保存looper对象这样所有子线程所有的地方都可以获取looper对象来创建handler
            Message* message = Message::obtain(1000);
            sendMessage(message);
        }else if (1000 == what) {
            //Looper::getForThread()->quit(true);//TLS 存儲looper對象Looper::getForThread()来创建其它的handler
        } else {
            //sleep(1);
        }
    }
};
//該示例是looper在子线程中的使用case
int main() {
    void **tret;
    NThread thread;
    MyHandler handler(thread.getLooper());
    while(mCount--) {
        Message* message = Message::obtain(mCount);
        handler.sendMessage(message);
    }
    //thread.getLooper()->quit(true);
    pthread_join(thread.getTid(),tret);
    return 0;
}

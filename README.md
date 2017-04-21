# ZflThreadPool
thread pool with linux and c++

messagequeue 大于0:

  getmessgae()(返回delay time最小的message,如果有多个相同最小的message返回第一个)
  
     1.取第一个message,赋值给tempmessage,算出delay赋值给mindelay.
     
     2.从第二个message开始遍历(如果只有一个message直接返回tempmessage),计算出temdelay和mindelay比较,
       如果temdelay <mindelay 将mindelay = temdelay,同时tempmessage= indexmessage;就这样遍历到最后一个message.
       
     3.返回tempmessage
     
   getmessgae()之后判断message的delaytime<= 0,如果是则直接remove message进行处理,否则算出timeout重新进入poll_waite    
   

messagequeue 小于等于0:

返回null looper线程退出







1.messageQueue 加一个quit标志

2.looper在第一次进入poll_wait或者处理完message进入poll_wait 设置timeout = 0 去检查是否有消息

3.当有消息要处理返回到looper 进行消息处理,没有message的时候设置timeout =-1 当quit为false的时候

  线程进入poll_wait等待,当quit为true的时候,返回null message给looper退出线程.


MQ: 1.有message:

          1@not delay message 直接将message返回给looper 进行处理

          2@delay message 设置timeout 进入poll_wait

    2.没有message:

          1@quit标志为false,timeout = -1 进入poll_wait

          2@quit 为true,返回null message 给looper

pollOnce 中poll_wait返回<=0要处理好<0:error =0:timeout


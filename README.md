# ZflThreadPool
thread pool with linux and c++

messagequeue 大于0:

  getmessgae()(返回delay time最小的message,如果有多个相同最小的message返回第一个)
  
     1.取第一个message,赋值给tempmessage,算出delay赋值给mindelay.
     
     2.从第二个message开始遍历(如果只有一个message直接返回tempmessage),计算出temdelay和mindelay比较,
       如果temdelay <mindelay 将mindelay = temdelay,同时tempmessage= indexmessage;就这样遍历到最后一个message.
       
     3.返回tempmessage
     
   getmessgae()之后判断delaytime<= 0,则直接remove进行处理,否则算出timeout重新进入poll_waite    
   

messagequeue 小于等于0:

返回null looper线程退出


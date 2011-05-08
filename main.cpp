#include <iostream>
#include <string>
#include <vector>
#include <boost\shared_ptr.hpp>
#include <boost\thread\mutex.hpp>
#include <boost\thread.hpp>

#include "Buffer.h"
#include "Producer.h"
#include "Consumer.h"

using namespace std;


int main()
{         
   boost::shared_ptr<CBuffer> buffer(new CBuffer(100));
   boost::shared_ptr<boost::mutex> bufferLock(new boost::mutex());
   boost::shared_ptr<boost::condition_variable> cond(new boost::condition_variable());

   CProducer producer1;
   producer1.SetBufferLock(bufferLock);
   producer1.SetCondition(cond);
   producer1.SetBuffer(buffer);
   producer1.SetWriteTime(1000);

   CProducer producer2;
   producer2.SetBufferLock(bufferLock);
   producer2.SetCondition(cond);
   producer2.SetBuffer(buffer);
   producer2.SetWriteTime(1000);

   CConsumer consumer;   
   consumer.SetBufferLock(bufferLock);
   consumer.SetCondition(cond);
   consumer.SetBuffer(buffer);
   consumer.SetReadTime(3000);

   producer1.Start();
   producer2.Start();
   consumer.Start();

   boost::this_thread::sleep(boost::posix_time::seconds(20));      

   producer1.Stop();
   producer2.Stop();
   consumer.Stop();

   system("pause");
   return 0;
}
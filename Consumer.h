#pragma once
#include <boost\thread.hpp>
#include <boost\shared_ptr.hpp>
#include <boost\random.hpp>
#include <boost\thread\locks.hpp>
#include "Buffer.h"

class CConsumer
{
public:  
   CConsumer();
   void SetReadTime(int i_msec);
   void SetBuffer(boost::shared_ptr<CBuffer> i_buffer);
   void SetBufferLock(boost::shared_ptr<boost::mutex> i_mutex);
   void SetCondition(boost::shared_ptr<boost::condition_variable> i_cond);
   void Start();
   void Stop();
   void Consuming();

protected:
   void ProcessData(int i_data);

   boost::shared_ptr<boost::thread> m_consumer;
   boost::shared_ptr<CBuffer>       m_buffer;
   boost::shared_ptr<boost::mutex>  m_bufferLock;

   boost::shared_ptr<boost::condition_variable> m_condition;

   boost::variate_generator<boost::mt19937, boost::uniform_int<>> m_random;
   int m_readTime;
};
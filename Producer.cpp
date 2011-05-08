#include <iostream>
#include <boost\format.hpp>
#include <boost\thread\mutex.hpp>
#include <boost\thread\condition_variable.hpp>
#include "Producer.h"


CProducer::CProducer():
   m_random(boost::mt19937(), boost::uniform_int<>())
{}

void CProducer::SetWriteTime(int i_msec)
{
   m_writeTime = i_msec;
}

void CProducer::SetBuffer(boost::shared_ptr<CBuffer> i_buffer)
{
   m_buffer = i_buffer;
}

void CProducer::SetBufferLock(boost::shared_ptr<boost::mutex> i_bufferLock)
{
   m_bufferLock = i_bufferLock;
}

void CProducer::SetCondition(boost::shared_ptr<boost::condition_variable> i_cond)
{
   m_condition = i_cond;
}

void CProducer::Start()
{
   m_producer.reset(new boost::thread(boost::bind(&CProducer::Producing, *this)));   
}

void CProducer::Stop()
{
   m_producer->interrupt();
   m_producer->join();
}

void CProducer::Producing()
{
   while (true)
   {
      int data = GenerateData(); 
        
      // writing data
      {
         boost::this_thread::disable_interruption di;
         boost::shared_ptr<boost::mutex> buf = m_bufferLock;                  
         {
            // ждем освобождения места в буфере
            boost::unique_lock<boost::mutex> lock(*buf);                                                                    
            while (m_buffer->GetMaxSize() == m_buffer->GetActualSize())
            {
               m_condition->wait(lock);
            }
         
            // пишем данные         
            {
               boost::this_thread::sleep(boost::posix_time::milliseconds(m_writeTime));
               m_buffer->WriteData(data);
               std::cout << boost::format("Producer write data %1%. In buffer: %2%") % data % m_buffer->GetActualSize() << std::endl;
            }         
         }

         // сообщаем, что считали
         m_condition->notify_all();
                  
      }
   }
}

int CProducer::GenerateData()
{
   int msec = m_random();
   boost::this_thread::sleep(boost::posix_time::milliseconds(msec));   
   int res = m_random();
   std::cout << boost::format("Producer generate data: %1%. In buffer: %2%") % res % m_buffer->GetActualSize() << std::endl;
   return res;
}
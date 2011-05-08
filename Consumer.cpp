#include <iostream>
#include <boost\format.hpp>
#include <boost\random.hpp>
#include "Consumer.h"


CConsumer::CConsumer() :
   m_random(boost::mt19937(), boost::uniform_int<>())
{}

void CConsumer::SetReadTime(int i_msec)
{
   m_readTime = i_msec;
}

void CConsumer::SetBuffer(boost::shared_ptr<CBuffer> i_buffer)
{
   m_buffer = i_buffer;
}

void CConsumer::SetBufferLock(boost::shared_ptr<boost::mutex> i_bufferLock)
{
   m_bufferLock = i_bufferLock;
}

void CConsumer::SetCondition(boost::shared_ptr<boost::condition_variable> i_cond)
{
   m_condition = i_cond;
}

void CConsumer::Start()
{
   m_consumer.reset(new boost::thread(boost::bind(&CConsumer::Consuming, *this)));
}

void CConsumer::Stop()
{
   m_consumer->interrupt();
   m_consumer->join();
}

void CConsumer::Consuming()
{
   while (true)
   {
      int data;
      
      // reading data
      {
         boost::this_thread::disable_interruption di;         
         boost::shared_ptr<boost::mutex> buf = m_bufferLock;         
                                
         {
            // ждем поступления новых данных
            boost::unique_lock<boost::mutex> lock(*buf);                   
            while (m_buffer->GetActualSize() == 0)
            {
               m_condition->wait(lock);
            }         
                    
            // читаем данные
            {
               boost::this_thread::sleep(boost::posix_time::milliseconds(m_readTime));   
               data = m_buffer->ReadData();            
               std::cout << boost::format("Consumer read data %1%. In buffer: %2%") %data % m_buffer->GetActualSize() << std::endl;
            }         
         }         

         // сообщаем, что записали
         m_condition->notify_all();                
      }

      ProcessData(data);
   }
}

void CConsumer::ProcessData(int i_data)
{   
   int msec = m_random();
   boost::this_thread::sleep(boost::posix_time::milliseconds(msec));   
   std::cout << boost::format("Consumer process data %1%. In buffer: %2%") % i_data % m_buffer->GetActualSize() << std::endl;
}
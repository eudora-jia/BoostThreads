#include "Buffer.h"

CBuffer::CBuffer()
{
   m_bufferActualSize = 0;
   m_bufferMaxSize = -1;
}

CBuffer::CBuffer(int i_bufferMaxSize)
{
   m_bufferActualSize = 0;
   m_bufferMaxSize = i_bufferMaxSize;
   m_buffer.resize(m_bufferMaxSize);
}

void CBuffer::SetConditionVariable(boost::shared_ptr<boost::condition_variable> i_cond)
{
   m_condition = i_cond;
}

void CBuffer::WriteData(int i_data) throw (std::exception)
{
   if (m_bufferMaxSize == -1)
      throw std::exception("Buffer size -1");            
   
   if (m_bufferActualSize == m_bufferMaxSize)
      throw std::exception("Buffer hasn't free space");

   m_buffer.push_back(i_data);  
   m_bufferActualSize++;
}

int CBuffer::ReadData() throw (std::exception)
{
   if (m_bufferMaxSize == -1)
      throw std::exception("Buffer size -1");
   
   if (m_bufferActualSize == 0)
      throw std::exception("Buffer hasn't data");

   int result = m_buffer.front();
   m_buffer.pop_front();
   m_bufferActualSize--;
   return result;
}

int CBuffer::GetActualSize()
{
   boost::mutex bufferLock;
   bufferLock.lock();
   int res = m_bufferActualSize;
   bufferLock.unlock();
   return res;
}

int CBuffer::GetMaxSize()
{
   boost::mutex bufferLock;
   bufferLock.lock();
   int res = m_bufferMaxSize;
   bufferLock.unlock();
   return res;
}
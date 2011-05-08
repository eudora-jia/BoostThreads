#pragma once
#include <deque>
#include <boost\thread\mutex.hpp>
#include <boost\thread\condition_variable.hpp>
#include <boost\shared_ptr.hpp>

class CBuffer
{
public:
   CBuffer();  
   CBuffer(int i_maxSize);

   void SetConditionVariable(boost::shared_ptr<boost::condition_variable> i_cond);
   void WriteData(int i_data) throw (std::exception);
   int ReadData() throw (std::exception);
   int GetActualSize();
   int GetMaxSize();

protected:
   boost::shared_ptr<boost::condition_variable> m_condition;

   std::deque<int> m_buffer;
   int m_bufferActualSize;
   int m_bufferMaxSize;
};
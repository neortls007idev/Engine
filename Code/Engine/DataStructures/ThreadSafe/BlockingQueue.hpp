#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>

//--------------------------------------------------------------------------------------------------------------------------------------------

template<typename T>
class BlockingQueue : protected std::queue<T>
{
public:
	//	typedef std::queue<T> baseQueue;
	using valueType = typename T;
protected:
	using baseQueue = typename std::queue<T>;

public:
	BlockingQueue() : baseQueue() {}
	BlockingQueue( BlockingQueue&& ) = delete;
	BlockingQueue( BlockingQueue const& ) = delete;
	~BlockingQueue() = default;

	BlockingQueue& operator=( BlockingQueue&& ) = delete;
	BlockingQueue& operator=( BlockingQueue const& ) = delete;

	void push( const valueType& value );
	valueType pop();
private:
	std::mutex m_lock;
	std::condition_variable m_condition;
};

//----------------------------------------------------------------------------------------------------------

template<typename T>
void BlockingQueue<T>::push( const valueType& value )
{
	std::lock_guard<std::mutex> guard( m_lock );
	baseQueue::push( value );
	m_condition.notify_all();
}

//----------------------------------------------------------------------------------------------------------

template<typename T>
typename BlockingQueue<T>::valueType BlockingQueue<T>::pop()
{
	valueType value = valueType();

	std::unique_lock<std::mutex> uniqueLock( m_lock );
	if ( baseQueue::empty() )
	{
		m_condition.wait( uniqueLock );
	}

	value = baseQueue::front();
	baseQueue::pop();

	return value;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

template<typename QueueT>
void ReadQueue( QueueT& queue )
{
	int value = QueueT::valueType( 0 );
	do
	{
		value = queue.pop();
	
	} while ( value != QueueT::valueType( -1 ) );
}

//--------------------------------------------------------------------------------------------------------------------------------------------
#pragma once
#include <queue>
#include <atomic>

//--------------------------------------------------------------------------------------------------------------------------------------------

template<typename T>
class  SynchronizedLockFreeQueue : protected std::queue<T>
{
public:
	using valueType = typename T;

protected:
	using Base = std::queue<T>;

public:
	SynchronizedLockFreeQueue() : Base() {}
	SynchronizedLockFreeQueue( SynchronizedLockFreeQueue&& ) = delete;
	SynchronizedLockFreeQueue( SynchronizedLockFreeQueue const& ) = delete;

	~SynchronizedLockFreeQueue() = default;

	SynchronizedLockFreeQueue& operator=( SynchronizedLockFreeQueue const& ) = delete;
	SynchronizedLockFreeQueue& operator=( SynchronizedLockFreeQueue&& ) = delete;

	void push( valueType const& value );
	valueType pop();

protected:
	void Lock();
	void Unlock();

private:
	const int UNLOCKED = 0;
	const int LOCKED = 1;
	std::atomic<int> m_atomicLock;
};

//--------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
void SynchronizedLockFreeQueue<T>::push( valueType const& value )
{
	Lock();
	Base::push( value );
	Unlock();

}

//--------------------------------------------------------------------------------------------------------------------------------------------

template<typename T>
typename SynchronizedLockFreeQueue<T>::valueType SynchronizedLockFreeQueue<T>::pop()
{
	valueType value /*= valueType( 0 )*/;

	Lock();
	if ( !Base::empty() )
	{
		value = Base::front();
		Base::pop();
	}
	Unlock();
	return value;
}

//--------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
void SynchronizedLockFreeQueue<T>::Lock()
{
	int expected = UNLOCKED;
	while ( !m_atomicLock.compare_exchange_strong( expected , LOCKED ) );
}

//--------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
void SynchronizedLockFreeQueue<T>::Unlock()
{
	int expected = LOCKED;
	while ( !m_atomicLock.compare_exchange_strong( expected , UNLOCKED ) );
}

//--------------------------------------------------------------------------------------------------------------------------------------------
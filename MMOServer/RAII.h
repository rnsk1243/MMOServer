#pragma once
#include<WinSock2.h>
#include<mutex>
#include<iostream>

class MUTEX
{
private:
	std::mutex mMutex;
public:
	MUTEX()
	{
		//	cout << "mutex生成者呼び出" << endl;
	}
	~MUTEX()
	{
		//	cout << "mutexソミョルジャ呼び出し" << endl;
	}
	void lock()
	{
		mMutex.lock();
		//	cout << "mutex lock完了" << endl;
	}
	void unlock()
	{
		mMutex.unlock();
		//	cout << "mutex nulock完了" << endl;
	}

};

class CRITICALSECTION
{
private:
	CRITICAL_SECTION mCS;
public:
	CRITICALSECTION()
	{
		InitializeCriticalSection(&mCS);
	}
	~CRITICALSECTION()
	{
		DeleteCriticalSection(&mCS);
			std::cout << "criticalオブジェクトの解除" << std::endl;
	}
	void lock()
	{
		EnterCriticalSection(&mCS);
		//std::cout << "critical lock完了" << std::endl;
	}
	void unlock()
	{
		LeaveCriticalSection(&mCS);
		//std::cout << "critical unlock完了" << std::endl;
	}
};



template<typename T> class ScopeLock {

	T& obj;
public:

	ScopeLock(T* o) : obj(*o)
	{
		obj.lock();
	}

	ScopeLock(T& o) : obj(o)
	{
		obj.lock();
	}

	~ScopeLock()
	{
		obj.unlock();
		//obj.~T();
	}

};
//static MUTEX RAII_MUTEX;
//static CRITICALSECTION RAII_CRITICALSECTION;
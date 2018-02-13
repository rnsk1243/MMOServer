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
		//	cout << "mutex ������ ȣ��" << endl;
	}
	~MUTEX()
	{
		//	cout << "mutex �Ҹ��� ȣ��" << endl;
	}
	void lock()
	{
		mMutex.lock();
		//	cout << "mutex lock �Ϸ�" << endl;
	}
	void unlock()
	{
		mMutex.unlock();
		//	cout << "mutex nulock �Ϸ�" << endl;
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
			std::cout << "critical ��ü ����" << std::endl;
	}
	void lock()
	{
		EnterCriticalSection(&mCS);
		std::cout << "critical lock �Ϸ�" << std::endl;
	}
	void unlock()
	{
		LeaveCriticalSection(&mCS);
		std::cout << "critical unlock �Ϸ�" << std::endl;
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
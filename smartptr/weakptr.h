#pragma once
#include "sharedptr.h"
#include "controlblock.h"
#include <cstdio>

template<typename T>
class CWeakPtr
{
	using Type_t = T;
	using Pointer_t = T*;
	using Reference_t = T&;

public:
	CWeakPtr();
	CWeakPtr(const CSharedPtr<Type_t>& shared_ptr);
	CWeakPtr(const CWeakPtr& other);
	CWeakPtr(CWeakPtr&& other) noexcept;
	~CWeakPtr();

	CSharedPtr<Type_t> Lock();

private:
	ControlBlock_t<Type_t>* m_pControlBlock;
};

template<typename T>
CWeakPtr<T>::CWeakPtr()
	: m_pControlBlock(nullptr)
{
	printf("%p CWeakPtr()\n", this);
}

template<typename T>
CWeakPtr<T>::CWeakPtr(const CSharedPtr<Type_t>& shared_ptr) : m_pControlBlock(shared_ptr.m_pControlBlock)
{
	printf("%p CWeakPtr(const CSharedPtr<Type_t>& shared_ptr)\n", this);
}

template<typename T>
CWeakPtr<T>::CWeakPtr(const CWeakPtr& other) : m_pControlBlock(other.m_pControlBlock)
{
	printf("%p CWeakPtr(const CWeakPtr& other)\n", this);
}

template<typename T>
CWeakPtr<T>::CWeakPtr(CWeakPtr&& other) noexcept : m_pControlBlock(other.m_pControlBlock)
{
	printf("%p CWeakPtr(CWeakPtr&& other)\n", this);
	other.m_pControlBlock = nullptr;
}

template<typename T>
CWeakPtr<T>::~CWeakPtr()
{
	printf("%p ~CWeakPtr()\n", this);
}

template<typename T>
CSharedPtr<typename CWeakPtr<T>::Type_t> CWeakPtr<T>::Lock()
{
	if (m_pControlBlock == nullptr)
	{
		return CSharedPtr<Type_t>();
	}

	++m_pControlBlock->shareds;

	return CSharedPtr<Type_t>(m_pControlBlock);
}

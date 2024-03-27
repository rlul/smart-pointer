#pragma once
#include "controlblock.h"
#include <cstdio>
#include <utility>

template<typename T> class CWeakPtr;

template<typename T>
class CSharedPtr
{
	template<typename _T, typename... Args>
	friend CSharedPtr<_T> MakeShared(Args&&... args);

	friend class CWeakPtr<T>;

	using Type_t = T;
	using Pointer_t = T*;
	using Reference_t = T&;

public:
	CSharedPtr();
	explicit CSharedPtr(Pointer_t pointer);
	CSharedPtr(const CSharedPtr& other);
	CSharedPtr(CSharedPtr&& other) noexcept;
	~CSharedPtr();

	unsigned long UseCount() const;
	CSharedPtr& operator=(const CSharedPtr& other);
	CSharedPtr& operator=(CSharedPtr&& other) noexcept;
	Reference_t operator*();
	Pointer_t operator->();

private:
	explicit CSharedPtr(ControlBlock_t<Type_t>* control_block);

private:
	ControlBlock_t<Type_t>* m_pControlBlock;
};

template<typename T>
CSharedPtr<T>::CSharedPtr() : m_pControlBlock(nullptr)
{
	printf("%p CSharedPtr()\n", this);
}

template<typename T>
CSharedPtr<T>::CSharedPtr(Pointer_t pointer) : m_pControlBlock(nullptr)
{
	printf("%p CSharedPtr(Pointer_t pointer)\n", this);
	if (pointer == nullptr)
	{
		return;
	}

	m_pControlBlock = new ControlBlock_t<Type_t>{ 1, pointer };
}

template<typename T>
CSharedPtr<T>::CSharedPtr(const CSharedPtr& other) : m_pControlBlock(nullptr)
{
	printf("%p CSharedPtr(const CSharedPtr& other)\n", this);
	m_pControlBlock = other.m_pControlBlock;
	++m_pControlBlock->shareds;
}

template<typename T>
CSharedPtr<T>::CSharedPtr(CSharedPtr&& other) noexcept : m_pControlBlock(nullptr)
{
	printf("%p CSharedPtr(CSharedPtr&& other)\n", this);
	m_pControlBlock = other.m_pControlBlock;
	other.m_pControlBlock = nullptr;
}

template<typename T>
CSharedPtr<T>::~CSharedPtr()
{
	printf("%p ~CSharedPtr()\n", this);
	if (m_pControlBlock == nullptr)
	{
		return;
	}

	if (--m_pControlBlock->shareds == 0)
	{
		delete m_pControlBlock;
	}
}

template<typename T>
unsigned long CSharedPtr<T>::UseCount() const
{
	if (m_pControlBlock == nullptr)
	{
		return 0;
	}

	return m_pControlBlock->shareds;
}

template<typename T>
CSharedPtr<T>& CSharedPtr<T>::operator=(const CSharedPtr& other)
{
	printf("%p CSharedPtr& operator=(const CSharedPtr& other)\n", this);
	if (this == &other)
	{
		return *this;
	}

	if (m_pControlBlock != nullptr && --m_pControlBlock->shareds == 0)
	{
		delete m_pControlBlock;
	}

	m_pControlBlock = other.m_pControlBlock;
	++m_pControlBlock->shareds;

	return *this;
}

template<typename T>
CSharedPtr<T>& CSharedPtr<T>::operator=(CSharedPtr&& other) noexcept
{
	printf("%p CSharedPtr& operator=(CSharedPtr&& other)\n", this);
	if (this == &other)
	{
		return *this;
	}

	if (m_pControlBlock != nullptr && --m_pControlBlock->shareds == 0)
	{
		delete m_pControlBlock;
	}

	m_pControlBlock = other.m_pControlBlock;
	other.m_pControlBlock = nullptr;

	return *this;
}

template<typename T>
typename CSharedPtr<T>::Reference_t CSharedPtr<T>::operator*()
{
	return *m_pControlBlock->p_object;
}

template<typename T>
typename CSharedPtr<T>::Pointer_t CSharedPtr<T>::operator->()
{
	return m_pControlBlock->p_object;
}

template<typename T>
CSharedPtr<T>::CSharedPtr(ControlBlock_t<Type_t>* control_block) : m_pControlBlock(control_block)
{
	printf("%p CSharedPtr(ControlBlock_t<Type_t>* control_block)\n", this);
}

template<typename T, typename... Args>
CSharedPtr<T> MakeShared(Args&&... args)
{
	auto p_object = new T(std::forward<Args>(args)...);
	auto p_control_block = new ControlBlock_t<T>{ 1, p_object };
	return CSharedPtr<T>(p_control_block);
}

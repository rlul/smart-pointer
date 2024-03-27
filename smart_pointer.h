#pragma once

template<typename T> class CSharedPtr;
template<typename T> class CWeakPtr;

template<typename T>
struct ControlBlock_t
{
	~ControlBlock_t() { delete p_object; }
	unsigned long shareds;
	T* p_object;
};

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

#include "sharedptr.cpp"
#include "weakptr.cpp"
#include "makeptr.cpp"

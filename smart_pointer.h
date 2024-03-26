#pragma once
#include <utility>
#include <cstdio>

template<typename T>
class CSharedPtr;

template<typename T>
class CWeakPtr;

template<typename T>
struct ControlBlock_t
{
	~ControlBlock_t()
	{
		delete p_object;
	}

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
	CSharedPtr()
		: m_pControlBlock(nullptr)
	{
		printf("%p CSharedPtr()\n", this);
	}

	CSharedPtr(Pointer_t pointer)
		: m_pControlBlock(nullptr)
	{
		printf("%p CSharedPtr(Pointer_t pointer)\n", this);
		if (pointer == nullptr)
		{
			return;
		}

		m_pControlBlock = new ControlBlock_t<Type_t>{ 1, pointer };
	}

	CSharedPtr(const CSharedPtr& other)
		: m_pControlBlock(nullptr)
	{
		printf("%p CSharedPtr(const CSharedPtr& other)\n", this);
		m_pControlBlock = other.m_pControlBlock;
		++m_pControlBlock->shareds;
	}

	CSharedPtr(CSharedPtr&& other) noexcept
		: m_pControlBlock(nullptr)
	{
		printf("%p CSharedPtr(CSharedPtr&& other)\n", this);
		m_pControlBlock = other.m_pControlBlock;
		other.m_pControlBlock = nullptr;
	}

	~CSharedPtr()
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

	unsigned long UseCount() const
	{
		if (m_pControlBlock == nullptr)
		{
			return 0;
		}

		return m_pControlBlock->shareds;
	}

	CSharedPtr& operator=(const CSharedPtr& other)
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

	CSharedPtr& operator=(CSharedPtr&& other) noexcept
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

	Reference_t operator*()
	{
		return *m_pControlBlock->p_object;
	}

	Pointer_t operator->()
	{
		return m_pControlBlock->p_object;
	}

private:
	explicit CSharedPtr(ControlBlock_t<Type_t>* control_block)
		: m_pControlBlock(control_block)
	{
		printf("%p CSharedPtr(ControlBlock_t<Type_t>* control_block)\n", this);
	}

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
	CWeakPtr()
		: m_pControlBlock(nullptr)
	{
		printf("%p CWeakPtr()\n", this);
	}

	CWeakPtr(const CSharedPtr<Type_t>& shared_ptr)
		: m_pControlBlock(shared_ptr.m_pControlBlock)
	{
		printf("%p CWeakPtr(const CSharedPtr<Type_t>& shared_ptr)\n", this);
	}

	CWeakPtr(const CWeakPtr& other)
		: m_pControlBlock(other.m_pControlBlock)
	{
		printf("%p CWeakPtr(const CWeakPtr& other)\n", this);
	}

	CWeakPtr(CWeakPtr&& other) noexcept
		: m_pControlBlock(other.m_pControlBlock)
	{
		printf("%p CWeakPtr(CWeakPtr&& other)\n", this);
		other.m_pControlBlock = nullptr;
	}

	~CWeakPtr()
	{
		printf("%p ~CWeakPtr()\n", this);
	}

	CSharedPtr<Type_t> Lock()
	{
		if (m_pControlBlock == nullptr)
		{
			return CSharedPtr<Type_t>();
		}

		++m_pControlBlock->shareds;

		return CSharedPtr<Type_t>(m_pControlBlock);
	}

private:
	ControlBlock_t<Type_t>* m_pControlBlock;
};

template<typename T, typename... Args>
CSharedPtr<T> MakeShared(Args&&... args)
{
	auto p_object = new T(std::forward<Args>(args)...);
	auto p_control_block = new ControlBlock_t<T>{ 1, p_object };
	return CSharedPtr<T>(p_control_block);
}

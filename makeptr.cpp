#pragma once
#include <cstdio>

template<typename T, typename... Args>
CSharedPtr<T> MakeShared(Args&&... args)
{
	auto p_object = new T(std::forward<Args>(args)...);
	auto p_control_block = new ControlBlock_t<T>{ 1, p_object };
	return CSharedPtr<T>(p_control_block);
}
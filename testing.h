#pragma once

template <typename T>
__forceinline void print_value(CSharedPtr<T>& ptr);

template <>
__forceinline void print_value(CSharedPtr<int>& ptr)
{
	printf("[%p]: %d\n", &ptr, *ptr);
}

template<typename T>
__forceinline void print_use_count(CSharedPtr<T>& ptr)
{
	printf("[%p] use count: %d\n", &ptr, ptr.UseCount());
}
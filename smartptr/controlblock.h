#pragma once

template<typename T>
struct ControlBlock_t
{
	~ControlBlock_t() { delete p_object; }
	unsigned long shareds;
	T* p_object;
};

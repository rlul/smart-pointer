#include "smartptr/sharedptr.h"
#include "smartptr/weakptr.h"
#include "testing.h"
#include <cstdio>

struct TestStruct_t
{
	int a;
	int b;
};

template<>__forceinline void print_value(CSharedPtr<TestStruct_t>& ptr)
{
	printf("[%p]: a = %d, b = %d\n", &ptr, ptr->a, (*ptr).b);
}

int main()
{
	// Test for int
	printf("\n/// TEST FOR INT ///\n");

	CSharedPtr int_shared1 = MakeShared<int>(100);
	print_value(int_shared1);
	print_value(int_shared1);
	print_use_count(int_shared1);
	{
		printf("Entered code block.\n");
		CSharedPtr int_shared2 = int_shared1;
		print_value(int_shared1);
		print_value(int_shared2);
		print_use_count(int_shared1);
		print_use_count(int_shared2);
		printf("Leaving code block...\n");
	}
	print_value(int_shared1);
	print_use_count(int_shared1);
	
	// Test for struct
	printf("\n/// TEST FOR STRUCT ///\n");
	CSharedPtr<TestStruct_t> struct_shared1 = MakeShared<TestStruct_t>(1, 2);
	{
		printf("Entered code block.\n");
		CSharedPtr<TestStruct_t> struct_shared2 = struct_shared1;
		print_value(struct_shared1);
		print_value(struct_shared2);
		print_use_count(struct_shared1);
		print_use_count(struct_shared2);
		printf("Leaving code block...\n");
	}
	print_value(struct_shared1);
	print_use_count(struct_shared1);

	// Test for weak pointer
	printf("\n/// TEST FOR WEAK POINTER ///\n");
    CWeakPtr weak_shared = struct_shared1;
	{
		printf("Entered code block.\n");
		CSharedPtr<TestStruct_t> struct_shared2 = weak_shared.Lock();
		weak_shared.Lock();
		print_value(struct_shared1);
		print_value(struct_shared2);
		print_use_count(struct_shared1);
		print_use_count(struct_shared2);
		printf("Leaving code block...\n");
	}
	print_value(struct_shared1);
	print_use_count(struct_shared1);

	return 0;
}

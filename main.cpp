#include "smart_pointer.h"
#include <cstdio>

#define PRINT_INT_VALUE(x)	printf(#x": %d\n", *(x))
#define PRINT_STRUCT_VALUE(x)	printf(#x": a = %d, b = %d\n", (x)->a, (*(x)).b)
#define PRINT_COUNT(x)	printf(#x" count: %d\n", (x).UseCount())

struct TestStruct_t
{
	int a;
	int b;
};

int main()
{
	// Test for int
	printf("\n/// TEST FOR INT ///\n");

	CSharedPtr int_shared1 = MakeShared<int>(100);
	PRINT_INT_VALUE(int_shared1);
	PRINT_COUNT(int_shared1);
	{
		printf("Entered code block.\n");
		CSharedPtr int_shared2 = int_shared1;
		PRINT_INT_VALUE(int_shared1);
		PRINT_INT_VALUE(int_shared2);
		PRINT_COUNT(int_shared1);
		PRINT_COUNT(int_shared2);
		printf("Leaving code block...\n");
	}
	PRINT_INT_VALUE(int_shared1);
	PRINT_COUNT(int_shared1);
	
	// Test for struct
	printf("\n/// TEST FOR STRUCT ///\n");
	CSharedPtr<TestStruct_t> struct_shared1 = MakeShared<TestStruct_t>(1, 2);
	{
		printf("Entered code block.\n");
		CSharedPtr<TestStruct_t> struct_shared2 = struct_shared1;
		PRINT_STRUCT_VALUE(struct_shared1);
		PRINT_STRUCT_VALUE(struct_shared2);
		PRINT_COUNT(struct_shared1);
		PRINT_COUNT(struct_shared2);
		printf("Leaving code block...\n");
	}
	PRINT_STRUCT_VALUE(struct_shared1);
	PRINT_COUNT(struct_shared1);

	// Test for weak pointer
	printf("\n/// TEST FOR WEAK POINTER ///\n");
    CWeakPtr weak_shared = struct_shared1;
	{
		printf("Entered code block.\n");
		CSharedPtr<TestStruct_t> struct_shared2 = weak_shared.Lock();
		weak_shared.Lock();
		PRINT_STRUCT_VALUE(struct_shared1);
		PRINT_STRUCT_VALUE(struct_shared2);
		PRINT_COUNT(struct_shared1);
		PRINT_COUNT(struct_shared2);
		printf("Leaving code block...\n");
	}
	PRINT_STRUCT_VALUE(struct_shared1);
	PRINT_COUNT(struct_shared1);

	return 0;
}

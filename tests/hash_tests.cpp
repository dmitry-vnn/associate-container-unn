#include <gtest/gtest.h>

template<class T>
size_t HashCode(const T& object)
{
	return std::hash<T>()(object);
}

TEST(hash, test)
{
	int* arr = new int[4];

	EXPECT_EQ(
		HashCode<int*>(arr + 1),
		HashCode<char*>(reinterpret_cast<char*>(arr) + sizeof(int))
	);
}
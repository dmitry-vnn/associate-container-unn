#include <gtest/gtest.h>
#include <unordered_map>

template<class T>
size_t HashCode(const T& object)
{
	return std::hash<T>()(object);
}

TEST(hash, test)
{
	std::unordered_map<int, int>();
	int* arr = new int[4];

	EXPECT_EQ(
		HashCode<int*>(arr + 1),
		HashCode<char*>(reinterpret_cast<char*>(arr) + sizeof(int))
	);
}
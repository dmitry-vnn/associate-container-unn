#include <gtest/gtest.h>

#include "calculator.h"

TEST(CALCULATOR, Get)
{
	Calculator c;
	EXPECT_EQ(c.Calc(), 555);
}
#include <cassert>
#include "Common.RNG.h"
#include "Common.RNG.Tests.h"
#include <format>
#include <functional>
#include <string>
std::function<void()> AddTest(const std::string& functionName, std::function<void()> testFunction);
namespace common::RNG
{
	static auto tests =
	{
		AddTest(
			"common::RNG::FromRange(int,int) should generate numbers within the specified range",
			[]() 
			{
				const int minimum = -10;
				const int maximum = 10;
				auto result = FromRange(minimum, maximum);
				assert(result >= minimum);
				assert(result < maximum);
			}),
		AddTest(
			"common::RNG::FromRange(size_t,size_t) should generate numbers within the specified range",
			[]()
			{
				const size_t minimum = 10;
				const size_t maximum = 20;
				auto result = FromRange(minimum, maximum);
				assert(result >= minimum);
				assert(result < maximum);
			}),
		AddTest(
			"common::RNG::FromRange(double,double) should generate numbers within the specified range",
			[]()
			{
				const double minimum = 10.0;
				const double maximum = 20.0;
				auto result = FromRange(minimum, maximum);
				assert(result >= minimum);
				assert(result < maximum);
			})
	};
}
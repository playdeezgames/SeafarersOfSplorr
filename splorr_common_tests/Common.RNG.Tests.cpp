#include <cassert>
#include "Common.RNG.h"
#include <format>
#include <functional>
#include <string>
void Assert(bool, const std::string&);
std::function<bool()> AddTest(const std::string& functionName, std::function<void()> testFunction);
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
				auto actual = FromRange(minimum, maximum);
				Assert(actual >= minimum,"Actual should be equal to or greater than minimum");
				Assert(actual < maximum, "Actual should be less than maximum");
			}),
		AddTest(
			"common::RNG::FromRange(size_t,size_t) should generate numbers within the specified range",
			[]()
			{
				const size_t minimum = 10;
				const size_t maximum = 20;
				auto actual = FromRange(minimum, maximum);
				Assert(actual >= minimum, "Actual should be equal to or greater than minimum");
				Assert(actual < maximum, "Actual should be less than maximum");
			}),
		AddTest(
			"common::RNG::FromRange(double,double) should generate numbers within the specified range",
			[]()
			{
				const double minimum = 10.0;
				const double maximum = 20.0;
				auto actual = FromRange(minimum, maximum);
				Assert(actual >= minimum, "Actual should be equal to or greater than minimum");
				Assert(actual < maximum, "Actual should be less than maximum");
			}),
		AddTest(
			"common::RNG::FromList should return nullopt when given an empty list",
			[]() 
			{
				std::list<int> inputList;//empty!
				auto actual = FromList(inputList);
				Assert(!actual.has_value(),"Actual should be nullopt");
			}),
		AddTest(
			"common::RNG::FromList should return that value when given a list with one value",
			[]()
			{
				const int expected = 10;
				std::list<int> inputList = { expected };
				auto actual = FromList(inputList);
				Assert(actual.has_value(), "Actual should have a value");
				Assert(actual.value() == expected, "Actual should be expected value");
			}),
		AddTest(
			"common::RNG::FromList should return that value that is in the input list",
			[]()
			{
				std::list<int> inputList = { 10, 20 };
				auto actual = FromList(inputList);
				Assert(actual.has_value(), "Actual should have a value");
				Assert(std::find(inputList.begin(), inputList.end(), actual.value()) != inputList.end(), "Actual should have a value in the list");
			}),
		AddTest(
			"common::RNG::FromGenerator should return the default result",
			[]()
			{
				const int expected = 10;
				std::map<int, size_t> inputTable;//empty!
				auto actual = FromGenerator(inputTable, expected);
				Assert(actual == expected, "Actual should have default value");
			}),
	};
}
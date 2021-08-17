#include "Common.Data.h"
#include "Harness.h"
namespace common::Data
{
	static auto tests =
	{
		AddTest(
			"common::Data::ToDouble should return 0.0 when given garbage",
			[]()
			{
				auto actual = ToDouble("GARBAGE");
				Assert(actual == 0.0, "Actual should be zero");
			}),
		AddTest(
			"common::Data::ToDouble should parse a valid int",
			[]()
			{
				auto actual = ToDouble("1");
				Assert(actual == 1.0, "Actual should be 1");
			}),
		AddTest(
			"common::Data::ToDouble should parse a valid double",
			[]()
			{
				auto actual = ToDouble("1.2");
				Assert(actual == 1.2, "Actual should be 1.2");
			}),
	};
}
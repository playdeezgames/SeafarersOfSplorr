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
		AddTest(
			"common::Data::ToInt should return 0 when given garbage",
			[]()
			{
				auto actual = ToInt("GARBAGE");
				Assert(actual == 0, "Actual should be zero");
			}),
		AddTest(
			"common::Data::ToInt should parse a valid int",
			[]()
			{
				auto actual = ToInt("1");
				Assert(actual == 1, "Actual should be 1");
			}),
		AddTest(
			"common::Data::ToOptionalInt should return nullopt when given empty string",
			[]()
			{
				auto actual = ToOptionalInt("");
				Assert(!actual.has_value(), "Actual should should not have a value");
			}),
		AddTest(
			"common::Data::ToOptionalInt should parse a valid int",
			[]()
			{
				auto actual = ToOptionalInt("1");
				Assert(actual.has_value(), "Actual should have a value");
				Assert(actual.value()==1, "Actual should have a value of 1");
			}),
		AddTest(
			"common::Data::ToOptionalDouble should return nullopt when given empty string",
			[]()
			{
				auto actual = ToOptionalDouble("");
				Assert(!actual.has_value(), "Actual should should not have a value");
			}),
		AddTest(
			"common::Data::ToOptionalDouble should parse a valid int",
			[]()
			{
				auto actual = ToOptionalDouble("1");
				Assert(actual.has_value(), "Actual should have a value");
				Assert(actual.value() == 1.0, "Actual should have a value of 1.0");
			}),
		AddTest(
			"common::Data::ToOptionalDouble should parse a valid double",
			[]()
			{
				auto actual = ToOptionalDouble("1.2");
				Assert(actual.has_value(), "Actual should have a value");
				Assert(actual.value() == 1.2, "Actual should have a value of 1.2");
			}),
		AddTest(
			"common::Data::ToPercentage should return nullopt when given a 0 for maximum",
			[]()
			{
				auto actual = ToPercentage(0,0);
				Assert(!actual.has_value(), "Actual not have a value");
			}),
		AddTest(
			"common::Data::ToPercentage should return percentage when given a nonzero for maximum",
			[]()
			{
				auto actual = ToPercentage(2,10);
				Assert(actual.has_value(), "Actual should have a value");
				Assert(actual.value()==20, "Actual should be the correct percentage");
			}),
		AddTest(
			"common::Data::ClampDouble should return minimum when value is less than minimum",
			[]()
			{
				const double minimum = 10.0;
				const double maximum = 20.0;
				const double value = 5.0;
				auto actual = ClampDouble(value, minimum, maximum);
				Assert(actual >= minimum, "Actual should be no lower than minimum");
				Assert(actual <= maximum, "Actual should be no higher than maximum");
				Assert(actual == minimum, "Actual should be minimum when less than minimum is passed");
			}),
		AddTest(
			"common::Data::ClampDouble should return maximum when value is greater than minimum",
			[]()
			{
				const double minimum = 10.0;
				const double maximum = 20.0;
				const double value = 25.0;
				auto actual = ClampDouble(value, minimum, maximum);
				Assert(actual >= minimum, "Actual should be no lower than minimum");
				Assert(actual <= maximum, "Actual should be no higher than maximum");
				Assert(actual == maximum, "Actual should be maximum when higher than maximum is passed");
			}),
		AddTest(
			"common::Data::ClampDouble should return value when value is in range",
			[]()
			{
				const double minimum = 10.0;
				const double maximum = 20.0;
				const double value = 15.0;
				auto actual = ClampDouble(value, minimum, maximum);
				Assert(actual >= minimum, "Actual should be no lower than minimum");
				Assert(actual <= maximum, "Actual should be no higher than maximum");
				Assert(actual == value, "Actual should be value when value is in range");
			})
	};
}
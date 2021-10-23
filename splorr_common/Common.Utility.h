#pragma once
#include <vector>
#include <string>
#include <functional>
#include <map>
#include <optional>
namespace common
{
	struct Utility
	{
		template<typename TValue>
		static TValue Identity(const TValue& value)
		{
			return value;
		}
		template<typename TValue1, typename TValue2>
		static TValue2 Cast(const TValue1& value)
		{
			return (TValue2)value;
		}
		template<typename TValue>
		static TValue Clamp(const TValue& value, const std::optional<TValue>& minimum, const std::optional<TValue>& maximum)
		{
			return 
				(maximum.has_value() && value > maximum.value()) ? (maximum.value()) :
				(minimum.has_value() && value < minimum.value()) ? (minimum.value()) :
				(value);
		}
	};
}



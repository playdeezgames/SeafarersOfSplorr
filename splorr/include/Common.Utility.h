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
		template<typename TInput, typename TOutput>
		static std::vector<TOutput> MapArray(const std::vector<TInput>& source, std::function<TOutput(const TInput&)> transform, std::function<bool(const TOutput&)> filter)
		{
			std::vector<TOutput> result;
			for (auto& entry : source)
			{
				auto candidate = transform(entry);
				if (filter(candidate))
				{
					result.push_back(candidate);
				}
			}
			return result;
		}
		template<typename TInput, typename TOutput>
		static std::vector<TOutput> MapArray(const std::vector<TInput>& source, std::function<TOutput(const TInput&)> transform)
		{
			return MapArray<TInput, TOutput>(source, transform, [](const TOutput&) { return true; });
		}
		template<typename TInput, typename TOutput>
		static std::optional<TOutput> MapOptional(const std::optional<TInput>& source, std::function<TOutput(const TInput&)> transform)
		{
			if (source)
			{
				return transform(source.value());
			}
			return std::nullopt;
		}
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
		static void IterateOptional(const std::optional<TValue>& value, std::function<void(const TValue&)> iterate)
		{
			if (value)
			{
				iterate(value.value());
			}
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



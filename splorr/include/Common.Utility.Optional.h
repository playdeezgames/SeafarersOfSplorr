#pragma once
#include <functional>
#include <optional>
namespace common::utility
{
	struct Optional
	{
		template<typename TInput, typename TOutput>
		static std::optional<TOutput> Map(const std::optional<TInput>& source, std::function<TOutput(const TInput&)> transform)
		{
			if (source)
			{
				return transform(source.value());
			}
			return std::nullopt;
		}
		template<typename TValue>
		static void Iterate(const std::optional<TValue>& value, std::function<void(const TValue&)> iterate)
		{
			if (value)
			{
				iterate(value.value());
			}
		}
	};
}

#pragma once
#include <functional>
#include <vector>
namespace common::utility
{
	struct Array
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
	};
}

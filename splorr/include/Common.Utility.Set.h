#pragma once
#include <functional>
#include <set>
namespace common::utility
{
	struct Set
	{
		template<typename TInput, typename TOutput>
		static std::set<TOutput> Map(const std::set<TInput>& source, std::function<TOutput(const TInput&)> transform)
		{
			std::set<TOutput> result;
			for (auto& entry : source)
			{
				auto candidate = transform(entry);
				result.insert(candidate);
			}
			return result;
		}
	};
}

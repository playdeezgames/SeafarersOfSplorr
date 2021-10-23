#pragma once
#include <functional>
#include <list>
#include <map>
#include <optional>
namespace common::utility
{
	struct List
	{
		template<typename TItem>
		static std::optional<TItem> GetNth(const std::list<TItem>& items, size_t index)
		{
			auto iter = items.begin();
			while (index > 0 && iter != items.end())
			{
				iter++;
				index--;
			}
			if (iter != items.end())
			{
				return *iter;
			}
			return std::nullopt;
		}
		template<typename TKey, typename TValue>
		static const std::list<TKey>& FromTable(std::list<TKey>& list, const std::map<TKey, TValue>& table, std::function<bool(const TValue&)> filter)
		{
			if (list.empty() && !table.empty())
			{
				for (auto& entry : table)
				{
					if (filter(entry.second))
					{
						list.push_back(entry.first);
					}
				}
			}
			return list;
		}
		template<typename TKey, typename TValue>
		static const std::list<TKey>& FromTable(std::list<TKey>& list, const std::map<TKey, TValue>& table)
		{
			std::function<bool(const TValue&)> filter = [](const TValue&) { return true; };
			return FromTable<TKey, TValue>(list, table, filter);
		}
		template<typename TInput, typename TOutput>
		static std::list<TOutput> Map(std::function<std::list<TInput>()> source, std::function<TOutput(const TInput&)> transform, std::function<bool(const TOutput&)> filter)
		{
			std::list<TOutput> result;
			for (auto& entry : source())
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
		static std::list<TOutput> Map(std::function<std::list<TInput>()> source, std::function<TOutput(const TInput&)> transform)
		{
			return Map<TInput, TOutput>(source, transform, [](const TOutput&) { return true; });
		}
		template<typename TInput>
		static void Iterate(const std::list<TInput>& source, std::function<void(const TInput&)> iterate)
		{
			for (auto& entry : source)
			{
				iterate(entry);
			}
		}
		template<typename TInput>
		static void Iterate(std::function<std::list<TInput>()> source, std::function<void(const TInput&)> iterate)
		{
			Iterate(source(), iterate);
		}
		template<typename TInput, typename TAccumulator>
		static TAccumulator Accumulate(std::function<std::list<TInput>()> source, std::function<void(TAccumulator&, const TInput&)> accumulate, const TAccumulator& initial)
		{
			TAccumulator accumulator = initial;
			for (auto& entry : source())
			{
				accumulate(accumulator, entry);
			}
			return accumulator;
		}
	};
}

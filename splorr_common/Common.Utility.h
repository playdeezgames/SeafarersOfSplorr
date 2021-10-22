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
		static const std::list<TKey>& ListFromTable(std::list<TKey>& list, const std::map<TKey, TValue>& table, std::function<bool(const TValue&)> filter)
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
		static const std::list<TKey>& ListFromTable(std::list<TKey>& list, const std::map<TKey, TValue>& table)
		{
			std::function<bool(const TValue&)> filter = [](const TValue&) { return true; };
			return ListFromTable<TKey, TValue>(list, table, filter);
		}
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
		static std::list<TOutput> MapList(std::function<std::list<TInput>()> source, std::function<TOutput(const TInput&)> transform, std::function<bool(const TOutput&)> filter)
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
		static std::list<TOutput> MapList(std::function<std::list<TInput>()> source, std::function<TOutput(const TInput&)> transform)
		{
			return MapList<TInput, TOutput>(source, transform, [](const TOutput&) { return true; });
		}
		template<typename TInput>
		static void IterateList(const std::list<TInput>& source, std::function<void(const TInput&)> iterate)
		{
			for (auto& entry : source)
			{
				iterate(entry);
			}
		}
		template<typename TInput>
		static void IterateList(std::function<std::list<TInput>()> source, std::function<void(const TInput&)> iterate)
		{
			IterateList(source(), iterate);
		}
		template<typename TInput, typename TAccumulator>
		static TAccumulator AccumulateList(std::function<std::list<TInput>()> source, std::function<void(TAccumulator&, const TInput&)> accumulate, const TAccumulator& initial)
		{
			TAccumulator accumulator = initial;
			for (auto& entry : source())
			{
				accumulate(accumulator, entry);
			}
			return accumulator;
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



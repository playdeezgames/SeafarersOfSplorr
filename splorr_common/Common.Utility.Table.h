#pragma once
#include <functional>
#include <map>
#include <optional>
namespace common::utility
{
	struct Table
	{
		template<typename TKey, typename TValue>
		static std::optional<TKey> GetNthKey(const std::map<TKey, TValue>& table, size_t index)
		{
			auto iter = table.begin();
			while (index > 0 && iter != table.end())
			{
				iter++;
				index--;
			}
			if (iter != table.end())
			{
				return iter->first;
			}
			return std::nullopt;
		}
		template<typename TKey, typename TValue>
		static std::optional<TKey> GetNthKey(const std::map<TKey, TValue>& table, std::optional<size_t> index)
		{
			if (index)
			{
				return GetNthKey(table, index.value());
			}
			return std::nullopt;
		}
		template<typename TKey, typename TValue>
		static std::optional<TValue> TryGetKey(const std::map<TKey, TValue>& table, const TKey& key)
		{
			auto iter = table.find(key);
			if (iter != table.end())
			{
				return iter->second;
			}
			return std::nullopt;
		}
		template <typename TKey1, typename TValue1, typename TKey2, typename TValue2>
		static std::map<TKey2, TValue2> Map(const std::map<TKey1, TValue1>& source, std::function<TKey2(const TKey1&)> transformKey, std::function<TValue2(const TValue1&)> transformValue)
		{
			std::map<TKey2, TValue2> result;
			for (auto& entry : source)
			{
				result[transformKey(entry.first)] = transformValue(entry.second);
			}
			return result;
		}
		template<typename TKey, typename TValue>
		static void Iterate(const std::map<TKey, TValue>& source, std::function<void(const TKey&, const TValue&)> iterate)
		{
			for (auto& entry : source)
			{
				iterate(entry.first, entry.second);
			}
		}
		template<typename TKey, typename TValue, typename TAccumulator>
		static TAccumulator Accumulate(const std::map<TKey, TValue>& source, std::function<void(TAccumulator&, const TKey&, const TValue&)> accumulate, const TAccumulator& initial)
		{
			TAccumulator accumulator = initial;
			for (auto& entry : source)
			{
				accumulate(accumulator, entry.first, entry.second);
			}
			return accumulator;
		}
		template<typename TKey, typename TValue, typename TAccumulator>
		static TAccumulator Accumulate(const std::map<TKey, TValue>& source, std::function<void(TAccumulator&, const TKey&, const TValue&)> accumulate)
		{
			return Accumulate(source, accumulate, TAccumulator());
		}
		template<typename TKey, typename TValue, typename TAccumulator>
		static TAccumulator Accumulate(std::function<std::map<TKey, TValue>()> source, std::function<void(TAccumulator&, const TKey&, const TValue&)> accumulate, const TAccumulator& initial)
		{
			return Accumulate(source(), accumulate, initial);
		}
		template<typename TKey, typename TValue, typename TAccumulator>
		static TAccumulator Accumulate(std::function<std::map<TKey, TValue>()> source, std::function<void(TAccumulator&, const TKey&, const TValue&)> accumulate)
		{
			return Accumulate(source, accumulate, TAccumulator());
		}
	};
}

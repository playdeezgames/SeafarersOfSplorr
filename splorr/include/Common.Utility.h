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
		template<typename TEnum>
		static void Dispatch(const std::map<TEnum, std::function<void()>>& handlers, const TEnum& key)
		{
			auto iter = handlers.find(key);
			if (iter != handlers.end())
			{
				iter->second();
			}
		}
		template<typename TEnum, typename TResult>
		static TResult Dispatch(const std::map<TEnum, std::function<TResult()>>& handlers, const TEnum& key, const TResult& defaultResult)
		{
			auto iter = handlers.find(key);
			if (iter != handlers.end())
			{
				return iter->second();
			}
			return defaultResult;
		}
		template<typename TEnum, typename TParameter>
		static void DispatchParameter(const std::map<TEnum, std::function<void(const TParameter&)>>& handlers, const TEnum& key, const TParameter& parameter)
		{
			auto iter = handlers.find(key);
			if (iter != handlers.end())
			{
				iter->second(parameter);
			}
		}
		template<typename TEnum, typename TResult, typename TParameter>
		static TResult DispatchParameter(const std::map<TEnum, std::function<TResult(const TParameter&)>>& handlers, const TEnum& key, const TParameter& parameter, const TResult& defaultResult)
		{
			auto iter = handlers.find(key);
			if (iter != handlers.end())
			{
				return iter->second(parameter);
			}
			return defaultResult;
		}
		template<typename TEnum, typename TResult>
		static TResult Dispatch(const std::map<TEnum, std::function<void()>>& handlers, const TEnum& key, const TResult& handledResult, const TResult& unhandledResult)
		{
			auto iter = handlers.find(key);
			if (iter != handlers.end())
			{
				iter->second();
				return handledResult;
			}
			return unhandledResult;
		}

		template<typename TKey, typename TValue>
		static std::function<void()> DoNextItem(size_t& index, const std::map<TKey, TValue>& table, std::function<void()> refresh)
		{
			return [&index, &table, refresh]()
			{
				index = (index + 1) % table.size();
				refresh();
			};
		}

		template<typename TKey, typename TValue>
		static std::function<void()> DoNextItem(std::optional<size_t>& index, const std::map<TKey, TValue>& table, std::function<void()> refresh)
		{
			return [&index, &table, refresh]()
			{
				index = (index.value_or(table.size() - 1) + 1) % table.size();
				refresh();
			};
		}

		template<typename TKey, typename TValue>
		static std::function<void()> DoPreviousItem(size_t& index, const std::map<TKey, TValue>& table, std::function<void()> refresh)
		{
			return [&index, &table, refresh]()
			{
				index = (index + table.size() - 1) % table.size();
				refresh();
			};
		}

		template<typename TKey, typename TValue>
		static std::function<void()> DoPreviousItem(std::optional<size_t>& index, const std::map<TKey, TValue>& table, std::function<void()> refresh)
		{
			return [&index, &table, refresh]()
			{
				index = (index.value_or(0) + table.size() - 1) % table.size();
				refresh();
			};
		}

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
		static std::optional<TValue> TryGetKey(const std::map<TKey, TValue>& table, const TKey& key)
		{
			auto iter = table.find(key);
			if (iter != table.end())
			{
				return iter->second;
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
		template<typename TKey, typename TValue>
		static void IterateTable(const std::map<TKey, TValue>& source, std::function<void(const TKey&, const TValue&)> iterate)
		{
			for (auto& entry : source)
			{
				iterate(entry.first, entry.second);
			}
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
		template<typename TKey, typename TValue, typename TAccumulator>
		static TAccumulator AccumulateTable(std::function<std::map<TKey, TValue>()> source, std::function<void(TAccumulator&, const TKey&, const TValue&)> accumulate, const TAccumulator& initial)
		{
			TAccumulator accumulator = initial;
			for (auto& entry : source())
			{
				accumulate(accumulator, entry.first, entry.second);
			}
			return accumulator;
		}
		template<typename TKey, typename TValue, typename TAccumulator>
		static TAccumulator AccumulateTable(std::function<std::map<TKey, TValue>()> source, std::function<void(TAccumulator&, const TKey&, const TValue&)> accumulate)
		{
			return AccumulateTable(source, accumulate, TAccumulator());
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
	};
}



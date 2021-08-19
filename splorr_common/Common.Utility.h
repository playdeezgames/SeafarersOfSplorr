#pragma once
#include <vector>
#include <string>
#include <functional>
#include <map>
#include <optional>
namespace common::Utility
{
	template<typename TEnum>
	void Dispatch(const std::map<TEnum, std::function<void()>> handlers, const TEnum& key)
	{
		auto iter = handlers.find(key);
		if (iter != handlers.end())
		{
			iter->second();
		}
	}
	template<typename TEnum, typename TResult>
	TResult Dispatch(const std::map<TEnum, std::function<TResult()>> handlers, const TEnum& key, const TResult& defaultResult)
	{
		auto iter = handlers.find(key);
		if (iter != handlers.end())
		{
			return iter->second();
		}
		return defaultResult;
	}
	template<typename TEnum, typename TResult, typename TParameter>
	TResult DispatchParameter(const std::map<TEnum, std::function<TResult(const TParameter&)>> handlers, const TEnum& key, const TParameter& parameter, const TResult& defaultResult)
	{
		auto iter = handlers.find(key);
		if (iter != handlers.end())
		{
			return iter->second(parameter);
		}
		return defaultResult;
	}

	template<typename TKey, typename TValue>
	std::function<void()> DoNextItem(size_t& index, const std::map<TKey, TValue>& table, std::function<void()> refresh)
	{
		return [&index, &table, refresh]()
		{
			index = (index + 1) % table.size();
			refresh();
		};
	}

	template<typename TKey, typename TValue>
	std::function<void()> DoPreviousItem(size_t& index, const std::map<TKey, TValue>& table, std::function<void()> refresh)
	{
		return [&index, &table, refresh]()
		{
			index = (index + table.size() - 1) % table.size();
			refresh();
		};
	}

	template<typename TKey, typename TValue>
	std::optional<TKey> GetNthKey(const std::map<TKey, TValue>& table, size_t index)
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
}


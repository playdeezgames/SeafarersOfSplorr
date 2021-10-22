#pragma once
#include <functional>
#include <map>
#include <optional>
namespace common::utility
{
	struct Navigator
	{
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
	};
}

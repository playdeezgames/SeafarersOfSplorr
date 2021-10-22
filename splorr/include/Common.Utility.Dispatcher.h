#pragma once
#include <functional>
#include <map>
namespace common::utility
{
	struct Dispatcher
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
		template<typename TEnum, typename TResult>
		static std::function<TResult(const TEnum&)> DoDispatch(const std::map<TEnum, std::function<void()>>& handlers, const TResult& handledResult, const TResult& unhandledResult)
		{
			return [handlers, handledResult, unhandledResult](const TEnum& key)
			{
				auto iter = handlers.find(key);
				if (iter != handlers.end())
				{
					iter->second();
					return handledResult;
				}
				return unhandledResult;
			};
		}
	};
}

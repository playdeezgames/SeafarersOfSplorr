#pragma once
#include <vector>
#include <string>
#include <functional>
#include <map>
#include <optional>
namespace common::Utility
{
	std::vector<std::string> ParseCommandLine(int, char**);
	template<typename TEnum>
	void Dispatch(const std::map<TEnum, std::function<void()>> handlers, const TEnum& key)
	{
		auto iter = handlers.find(key);
		if (iter != handlers.end())
		{
			iter->second();
		}
	}
	template<typename TEnum, typename TParameter>
	void DispatchParameter(const std::map<TEnum, std::function<void(const TParameter&)>> handlers, const TEnum& key, const TParameter& parameter)
	{
		auto iter = handlers.find(key);
		if (iter != handlers.end())
		{
			iter->second(parameter);
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

	bool FileExists(const std::string&);
	unsigned char GetFileCheckSum(const std::string&);
}



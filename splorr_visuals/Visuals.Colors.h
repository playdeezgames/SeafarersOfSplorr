#pragma once
#include <functional>
#include <string>
#include <tuple>
#include <vector>
namespace visuals::Colors
{
	void SetStore(int);
	template<typename TStore>
	std::function<void()> DoSetStore(const TStore& store)
	{
		return [store]() 
		{
			SetStore((int)store);
		};
	}
	std::tuple<unsigned char, unsigned char, unsigned char, unsigned char> Read(const std::string&);
}
#pragma once
#include <functional>
namespace Options
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
	void Initialize();
	void Save();
}
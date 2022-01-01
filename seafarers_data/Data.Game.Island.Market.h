#pragma once
#include <map>
#include <optional>
namespace data::game::island
{
	struct Market
	{
		Market() = delete;
		struct Data
		{
			double supply;
			double demand;
			double purchases;
			double sales;
		};
		static void Initialize();
		static void Write(int, int, const Data&);
		static std::optional<Data> Read(int, int);
		static std::map<int, Data> All(int);
		static void Clear();
	};

}
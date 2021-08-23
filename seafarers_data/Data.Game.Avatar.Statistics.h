#pragma once
#include <optional>
namespace data::game::avatar
{
	class Statistics
	{
	public:
		struct Data
		{
			std::optional<double> minimum;
			std::optional<double> maximum;
			double current;
		};
		static void Write(int, const Data&);
		static std::optional<Data> Read(int);
	};
}

#include <algorithm>
#include "Data.Game.Island.Feature.h"
#include "Data.Game.Island.District.h"
#include "Data.Game.Common.h"
#include <iterator>
namespace data::game::island
{
	using namespace std::string_view_literals;
	static constexpr std::string_view QUERY_ALL = 
		R"(SELECT DISTINCT 
			[District] 
		FROM [Features] 
		WHERE 
			[IslandId]={};)"sv;

	static constexpr std::string_view FIELD_DISTRICT = "District";

	void District::Initialize()
	{
		data::game::island::Feature::Initialize();
	}

	std::vector<int> District::Read(int islandId)
	{
		Initialize();
		std::vector<int> result;
		auto records = Common::Execute(QUERY_ALL, islandId);
		std::transform(
			records.begin(),
			records.end(),
			std::back_inserter(result),
			Common::DoToInt(FIELD_DISTRICT));
		return result;
	}
}
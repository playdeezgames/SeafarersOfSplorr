#include "Game.Session.Island.District.h"
#include <map>
namespace game::session::island
{
	struct DistrictDescriptor
	{
		std::string name;
	};

	static const std::map<game::island::District, DistrictDescriptor> districtDescriptors =
	{
		{
			game::island::District::BUSINESS, 
			{
				"Business District"
			}
		}
	};

	static const DistrictDescriptor& GetDescriptor(const game::island::District& district)
	{
		return districtDescriptors.find(district)->second;
	}

	std::string District::GetName() const
	{
		return GetDescriptor(district).name;
	}
}
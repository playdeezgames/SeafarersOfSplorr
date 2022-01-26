#pragma once
#include <string>
namespace game::session::item::type
{
	struct Properties
	{
		constexpr Properties(int itemTypeId) : itemTypeId(itemTypeId) {}
		void AddProperty(const std::string& name, int value) const;
	private:
		int itemTypeId;
	};
}

#pragma once
namespace data::game::item::type
{
	struct Category
	{
		Category() = delete;
		static void Initialize();
		static void AddToCategory(int itemTypeId, int category);
		static bool HasCategory(int itemTypeId, int category);
	};
}

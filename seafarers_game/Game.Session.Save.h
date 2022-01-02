#pragma once
namespace game::session
{
	struct Save
	{
		constexpr explicit Save(int storeIndex) : storeIndex(storeIndex) {}
		bool Exists();
		void Store();
		void Load();
	private:
		int storeIndex;
	};
}
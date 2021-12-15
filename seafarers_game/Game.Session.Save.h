#pragma once
namespace game::session
{
	struct Save
	{
		Save(int);
		bool Exists();
		void Store();
		void Load();
	private:
		int storeIndex;
	};
}
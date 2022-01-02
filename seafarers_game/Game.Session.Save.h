#pragma once
namespace game::session
{
	struct Save
	{
		explicit Save(int);
		bool Exists();
		void Store();
		void Load();
	private:
		int storeIndex;
	};
}
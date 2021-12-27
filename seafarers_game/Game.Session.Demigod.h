#pragma once
#include <string>
namespace game::session
{
	struct Demigod
	{
		constexpr Demigod(int demigodId) : demigodId(demigodId) {}
		std::string GetName() const;

	private:
		int demigodId;
	};
}

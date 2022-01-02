#pragma once
#include <string>
namespace game::session
{
	struct Demigod
	{
		constexpr explicit Demigod(int demigodId) : demigodId(demigodId) {}
		std::string GetName() const;
	private:
		int demigodId;
	};
}

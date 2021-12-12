#pragma once
#include <string>
namespace game::session::character
{
	struct Messages
	{
		Messages(int);
		void Add(const std::string&, const std::string&) const;
	private:
		int characterId;
	};
}

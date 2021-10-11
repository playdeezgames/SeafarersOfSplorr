#include <Data.Game.Avatar.Log.h>
#include "Game.Avatar.Log.h"
#include "Game.Colors.h"
#include <Game.Player.h>
namespace game::avatar
{
	static const std::string WELCOME_TEXT = "Adventure ahoy!";

	void Log::Reset(const Difficulty&)
	{
		data::game::avatar::Log::Clear(Player::GetAvatarId());
		Log::Write({ game::Colors::YELLOW,  WELCOME_TEXT });
	}

	std::list<Log> Log::Read(size_t entryCount)
	{
		std::list<Log> results;
		auto entries = data::game::avatar::Log::Read(Player::GetAvatarId(), entryCount);
		for (auto entry : entries)
		{
			results.push_back(
				{
					entry.color,
					entry.text
				});
		}
		return results;
	}

	void Log::Write(const Log& entry)
	{
		data::game::avatar::Log::Write(Player::GetAvatarId(), entry.color, entry.text);
	}
}

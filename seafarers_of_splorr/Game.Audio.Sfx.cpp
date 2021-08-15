#include "Common.Audio.h"
#include "Game.Audio.Sfx.h"
namespace game::audio::Sfx
{
	const std::map<game::audio::GameSfx, std::string> table =
	{
		{ GameSfx::DEATH, "death"},
		{ GameSfx::ENEMY_HIT, "enemyhit"},
		{ GameSfx::HIT, "hit"}
	};

	void Play(const game::audio::GameSfx& gameSfx)
	{
		auto iter = table.find(gameSfx);
		if (iter != table.end())
		{
			common::audio::Sfx::Play(iter->second);
		}
	}
}
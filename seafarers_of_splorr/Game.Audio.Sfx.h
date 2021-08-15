#pragma once
namespace game::audio
{
	enum class GameSfx
	{
		DEATH,
		ENEMY_HIT,
		HIT
	};
}
namespace game::audio::Sfx
{
	void Play(const game::audio::GameSfx&);
}


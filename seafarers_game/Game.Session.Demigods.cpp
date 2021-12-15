#include "Game.Demigods.h"
#include "Game.Player.h" //eliminate
#include "Game.Session.Demigods.h"
namespace game::session
{
	void Demigods::Reset(const Difficulty& difficulty) const
	{
		game::Demigods::Reset(game::Player::GetCharacterId(), difficulty);//TODO: reset ALL
	}

	void Demigods::ApplyTurnEffects() const
	{
		game::Demigods::ApplyTurnEffects(game::Player::GetCharacterId());//TODO: apply to ALL characters
	}
}
#include "Game.Session.Characters.h"
#include "Game.Session.Character.h"
#include "Game.Characters.h" //FOR APPLY TURN EFFECTS
#include "Game.Characters.Equipment.h" //FOR RESET
#include "Game.Characters.Items.h" //FOR RESET
#include "Game.Characters.Plights.h" //FOR APPLY TURN EFFECTS
#include "Game.Characters.Statistics.h" //FOR RESET
#include "Game.Player.h"
namespace game::session
{
	session::Character Characters::GetCharacter(int characterId) const
	{
		return Character(characterId);
	}

	void Characters::Reset(const Difficulty& difficulty) const
	{
		game::Characters::Reset(difficulty);
		game::characters::Equipment::Reset(difficulty);
		game::characters::Plights::Reset(difficulty);
	}

	void Characters::ApplyTurnEffects() const
	{
		game::Characters::ApplyTurnEffects();
		game::characters::Plights::ApplyTurnEffects(game::Player::GetCharacterId());
	}

}
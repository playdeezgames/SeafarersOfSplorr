#include <Common.Utility.Optional.h>
#include <Data.Game.Player.h>
#include <Game.Characters.Message.h>
#include <Data.Game.Character.Ship.h>
#include <Game.Session.h>
#include <Game.Ship.h>
#include "State.InPlay.Globals.h"
namespace state::in_play
{
	int GetPlayerCharacterId()
	{
		return data::game::Player::GetCharacterId().value();
	}

	std::optional<int> GetPlayerCharacterShipId()
	{
		return data::game::character::Ship::ReadShipForCharacter(GetPlayerCharacterId());
	}

	std::optional<int> GetPlayerCharacterIslandId()
	{
		throw "FIXME";
	}

	void PlayMainTheme()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
	}

	void DoPlayerCharacterAction(const game::characters::Action& action)
	{
		game::Session().GetPlayer().GetCharacter().DoAction(action);
	}

	static int crewDetailCharacterId = 0;

	void SetCrewDetailCharacterId(int characterId)
	{
		crewDetailCharacterId = characterId;
	}

	int GetCrewDetailCharacterId()
	{
		return crewDetailCharacterId;
	}

	static const std::map<cards::Rank, std::string> ranks =
	{
		{cards::Rank::ACE, "A "},
		{cards::Rank::DEUCE, "2 "},
		{cards::Rank::THREE, "3 "},
		{cards::Rank::FOUR, "4 "},
		{cards::Rank::FIVE, "5 "},
		{cards::Rank::SIX, "6 "},
		{cards::Rank::SEVEN, "7 "},
		{cards::Rank::EIGHT, "8 "},
		{cards::Rank::NINE, "9 "},
		{cards::Rank::TEN, "10"},
		{cards::Rank::JACK, "J "},
		{cards::Rank::QUEEN, "Q "},
		{cards::Rank::KING, "K "}
	};

	static const std::map<cards::Suit, std::string> suitColors =
	{
		{cards::Suit::CLUB, game::Colors::DARK_GRAY},
		{cards::Suit::DIAMOND, game::Colors::RED},
		{cards::Suit::HEART, game::Colors::RED},
		{cards::Suit::SPADE, game::Colors::DARK_GRAY}
	};

	static const std::map<cards::Suit, std::string> suits =
	{
		{cards::Suit::CLUB, "\x05"},
		{cards::Suit::DIAMOND, "\x04"},
		{cards::Suit::HEART, "\x03"},
		{cards::Suit::SPADE, "\x06"}
	};

	const std::map<cards::Rank, std::string>& GetRanks()
	{
		return ranks;
	}

	const std::map<cards::Suit, std::string>& GetSuitColors()
	{
		return suitColors;
	}

	const std::map<cards::Suit, std::string>& GetSuits()
	{
		return suits;
	}

	static std::string renameShipAdjective = "";

	void SetRenameShipAdjective(const std::string& adjective)
	{
		renameShipAdjective = adjective;
	}

	const std::string& GetRenameShipAdjective()
	{
		return renameShipAdjective;
	}

	void AddPlayerCharacterMessage(const std::string& color, const std::string& text)
	{
		game::Session().GetPlayer().GetCharacter().GetMessages().Add(color, text);
	}

	void AddPlayerCharacterMessage(const std::string& text)
	{
		game::Session().GetPlayer().GetCharacter().GetMessages().Add(game::Colors::GRAY, text);
	}
}

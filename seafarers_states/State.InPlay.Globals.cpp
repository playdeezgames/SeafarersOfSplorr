#include <Common.Utility.Optional.h>
#include <Data.Game.Player.h>
#include <Game.Characters.Items.h>
#include <Game.Characters.Message.h>
#include <Game.Characters.Quests.h>
#include <Game.Characters.Ships.h>
#include <Game.Ship.Statistics.h>
#include <Game.Islands.h>
#include <Game.Islands.DarkAlley.h>
#include <Game.Islands.Features.h>
#include <Game.Islands.Items.h>
#include <Game.Session.h>
#include <Game.Ship.h>
#include <Game.Ship.Docked.h>
#include "State.InPlay.Globals.h"
namespace state::in_play
{
	int GetPlayerCharacterId()
	{
		return data::game::Player::GetCharacterId().value();
	}

	std::optional<int> GetPlayerCharacterShipId()
	{
		return game::characters::Ships::ReadShipId(GetPlayerCharacterId());
	}

	std::optional<int> GetPlayerCharacterIslandId()
	{
		return
			common::utility::Optional::Bind<int, int>(
				GetPlayerCharacterShipId(),
				game::ship::Docked::GetIsland);
	}

	std::optional<game::ship::DockResult> Dock()
	{
		return 
			common::utility::Optional::Bind<int, game::ship::DockResult>(
				GetPlayerCharacterShipId(), 
				game::ship::Docked::Dock);
	}

	std::optional<game::Quest> GetPlayerCharacterQuest()
	{
		return game::characters::Quests::Read(GetPlayerCharacterId());
	}

	std::optional<common::XY<double>> GetPlayerCharacterShipLocation()
	{
		return
			common::utility::Optional::Bind<int, common::XY<double>>(
				GetPlayerCharacterShipId(),
				game::Ship::GetLocation);
	}

	std::optional<double> GetPlayerCharacterShipHeading()
	{
		return common::utility::Optional::Bind<int, double>(
			GetPlayerCharacterShipId(),
			game::Ship::GetHeading);
	}

	std::optional<double> GetPlayerCharacterShipSpeed()
	{
		return common::utility::Optional::Bind<int, double>(
			GetPlayerCharacterShipId(),
			game::Ship::GetSpeed);
	}

	void PlayMainTheme()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
	}

	void SetPlayerCharacterShipHeading(double heading)
	{
		common::utility::Optional::Iterate<int>(
			GetPlayerCharacterShipId(),
			[heading](int shipId) {game::Ship::SetHeading(shipId, heading); }
			);
	}

	void DoPlayerCharacterAction(const game::characters::Action& action)
	{
		game::Session().GetPlayer().GetCharacter().DoAction(action);
	}

	std::optional<common::XY<double>> GetPlayerCharacterIslandLocation()
	{
		auto characterId = GetPlayerCharacterId();
		auto islandId = GetPlayerCharacterIslandId();
		if (islandId)
		{
			auto island = game::Islands::Read(characterId, islandId.value());
			if (island)
			{
				return island.value().absoluteLocation;
			}
		}
		return std::nullopt;
	}

	std::optional<double> GetPlayerCharacterDarkAlleyAnte()
	{
		return common::utility::Optional::Bind<int, double>(
			GetPlayerCharacterIslandId(),
			game::islands::DarkAlley::GetAnte);
	}

	std::optional<double> GetPlayerCharacterDarkAlleyMinimumWager()
	{
		return common::utility::Optional::Bind<int, double>(
			GetPlayerCharacterIslandId(),
			game::islands::DarkAlley::GetMinimumWager);
	}

	std::optional<double> GetPlayerCharacterAvailableTonnage()
	{
		return game::characters::Ships::AvailableTonnage(GetPlayerCharacterId());
	}

	std::optional<std::map<game::Item, double>> GetPlayerCharacterPurchasePrices()
	{
		return common::utility::Optional::Bind<int, std::map<game::Item, double>>(
			GetPlayerCharacterIslandId(),
			game::islands::Items::GetPurchasePrices);
	}
	std::map<game::Item, size_t> GetPlayerCharacterItems()
	{
		return game::characters::Items::All(GetPlayerCharacterId());
	}

	void PlayerCharacterCleanHull(const game::Side& side)
	{
		game::ship::Statistics::CleanHull(GetPlayerCharacterShipId().value(), side);
	}

	static game::Item islandTradeItem = game::Item::RATIONS;

	void SetIslandTradeItem(const game::Item& item)
	{
		islandTradeItem = item;
	}

	const game::Item& GetIslandTradeItem()
	{
		return islandTradeItem;
	}

	static game::EquipSlot equipmentSlot = game::EquipSlot::PRISON_WALLET;

	void SetEquipmentSlot(const game::EquipSlot& equipSlot)
	{
		equipmentSlot = equipSlot;
	}

	const game::EquipSlot& GetEquipmentSlot()
	{
		return equipmentSlot;
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

	std::optional<bool> PlayerCharacterIslandHasFeature(const game::Feature& feature)
	{
		return common::utility::Optional::Map<int, bool>(
			GetPlayerCharacterIslandId(),
			[feature](int islandId) { return game::islands::Features::Read(islandId, feature); });
	}
}

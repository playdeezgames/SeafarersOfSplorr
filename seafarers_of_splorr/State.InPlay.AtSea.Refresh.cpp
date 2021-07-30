#include <format>
#include "Game.Avatar.h"
#include "Game.Avatar.Destination.h"
#include "Game.Avatar.Quest.h"
#include "Game.Avatar.Statistics.h"
#include "Game.Heading.h"
#include "Game.Islands.h"
#include "Game.World.h"
#include <map>
#include <string>
#include "Visuals.Images.h"
#include "Visuals.Menus.h"
#include "Visuals.Texts.h"
namespace state::in_play::AtSea
{
	const std::string LAYOUT_NAME = "State.InPlay.AtSea";

	const std::string TEXT_AVATAR_TURNS = "avatar-turns";
	const std::string TEXT_AVATAR_HEALTH = "avatar-health";
	const std::string TEXT_AVATAR_SATIETY = "avatar-satiety";
	const std::string TEXT_AVATAR_HEADING = "avatar-heading";
	const std::string TEXT_AVATAR_SPEED = "avatar-speed";
	const std::string TEXT_AVATAR_MONEY = "avatar-money";
	const std::string TEXT_AVATAR_REPUTATION = "avatar-reputation";

	const std::string FORMAT_TURNS = "Turns Left: {:d}";
	const std::string FORMAT_MONEY = "Money: {:.3f}";
	const std::string FORMAT_REPUTATION = "Reputation: {:.0f}";
	const std::string FORMAT_SATIETY = "Satiety: {:.0f}";
	const std::string FORMAT_HEALTH = "Health: {:.0f}";
	const std::string FORMAT_HEADING = "Heading: {:.2f}";
	const std::string FORMAT_SPEED = "Speed: {:.2f}";

	const std::string IMAGE_CURRENT_HEADING = "CurrentHeading";
	const std::string IMAGE_DESTINATION_1 = "Destination1";
	const std::string IMAGE_DESTINATION_2 = "Destination2";
	const std::string IMAGE_DESTINATION_3 = "Destination3";
	const std::string IMAGE_DESTINATION_4 = "Destination4";
	const std::string IMAGE_QUEST_DESTINATION = "QuestDestination";

	const std::string MENU_ITEM_DOCK = "Dock";
	const std::string MENU_ITEM_JOB = "Job";

	static void RefreshAvatarSatiety()
	{
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_AVATAR_SATIETY, std::format(FORMAT_SATIETY, game::avatar::Statistics::GetSatiety()));
	}

	static void RefreshAvatarMoney()
	{
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_AVATAR_MONEY, std::format(FORMAT_MONEY, game::avatar::Statistics::GetCurrent(game::avatar::Statistic::MONEY)));
	}

	static void RefreshAvatarReputation()
	{
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_AVATAR_REPUTATION, std::format(FORMAT_REPUTATION, game::avatar::Statistics::GetReputation()));
	}

	static void RefreshAvatarHealth()
	{
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_AVATAR_HEALTH, std::format(FORMAT_HEALTH, game::avatar::Statistics::GetCurrent(game::avatar::Statistic::HEALTH)));
	}

	static void RefreshAvatarTurns()
	{
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_AVATAR_TURNS, std::format(FORMAT_TURNS, game::avatar::Statistics::GetTurnsRemaining()));
	}

	static void RefreshAvatarHeading()
	{
		auto heading = game::Avatar::GetHeading();
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_AVATAR_HEADING, std::format(FORMAT_HEADING, heading));
		visuals::Images::SetAngle(LAYOUT_NAME, IMAGE_CURRENT_HEADING, heading);
	}

	static void RefreshAvatarSpeed()
	{
		auto speed = game::Avatar::GetSpeed();
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_AVATAR_SPEED, std::format(FORMAT_SPEED, speed));
	}

	const std::map<game::avatar::Destination, std::string> destinationIdImages =
	{
		{game::avatar::Destination::ONE, IMAGE_DESTINATION_1},
		{game::avatar::Destination::TWO, IMAGE_DESTINATION_2},
		{game::avatar::Destination::THREE, IMAGE_DESTINATION_3},
		{game::avatar::Destination::FOUR, IMAGE_DESTINATION_4}
	};

	static common::XY<double> Plot(const common::XY<double>&);
	static void RefreshAvatarDestination(const game::avatar::Destination& destinationId)
	{
		auto destination = game::avatar::destination::GetDestination(destinationId);
		auto imageId = destinationIdImages.find(destinationId)->second;
		if (destination)
		{
			auto clampedDistance = game::Heading::ClampDistance(destination.value() - game::Avatar::GetLocation(), game::World::GetViewDistance() + 0.5);//TODO: magic number
			auto plot = Plot(clampedDistance);
			visuals::Images::SetLocation(LAYOUT_NAME, imageId, { (int)plot.GetX(), (int)plot.GetY() });
			visuals::Images::SetVisible(LAYOUT_NAME, imageId, true);
			return;
		}
		visuals::Images::SetVisible(LAYOUT_NAME, imageId, false);
	}

	static void RefreshAvatarDestinations()
	{
		for (auto destinationId : game::avatar::destination::All())
		{
			RefreshAvatarDestination(destinationId);
		}
	}

	static void RefreshAvatarQuestDestination()
	{
		auto quest = game::avatar::Quest::Read();
		if (quest)
		{
			auto clampedDistance = game::Heading::ClampDistance(quest.value().destination - game::Avatar::GetLocation(), game::World::GetViewDistance() + 0.5);//TODO: magic number
			auto plot = Plot(clampedDistance);
			visuals::Images::SetLocation(LAYOUT_NAME, IMAGE_QUEST_DESTINATION, { (int)plot.GetX(), (int)plot.GetY() });
			visuals::Images::SetVisible(LAYOUT_NAME, IMAGE_QUEST_DESTINATION, true);
			return;
		}
		visuals::Images::SetVisible(LAYOUT_NAME, IMAGE_QUEST_DESTINATION, false);
	}

	//TODO: get this not hardcoded
	const common::XY<double> VIEW_CENTER = { 162.0,182.0 };
	const double VIEW_RADIUS = 144.0;
	const int ISLAND_ICON_COUNT = 10;
	const int IMAGE_OFFSET_X = 0;
	const int IMAGE_OFFSET_Y = 0;
	const int TEXT_OFFSET_X = 0;
	const int TEXT_OFFSET_Y = 8;

	static void HideVisibleIslands()
	{
		for (int icon = 0; icon < ISLAND_ICON_COUNT; ++icon)
		{
			auto visualId = std::format("AtSeaIsland{}", icon);
			visuals::Images::SetVisible(LAYOUT_NAME, visualId, false);
			visuals::Texts::SetText(LAYOUT_NAME, visualId, "");
		}
	}

	static common::XY<double> Plot(const common::XY<double>& location)
	{
		double viewScale = VIEW_RADIUS / game::World::GetViewDistance();
		return location * viewScale + VIEW_CENTER;
	}


	void RefreshAvatarStatus()
	{
		RefreshAvatarHeading();
		RefreshAvatarHealth();
		RefreshAvatarSatiety();
		RefreshAvatarSpeed();
		RefreshAvatarMoney();
		RefreshAvatarReputation();
		RefreshAvatarTurns();
		RefreshAvatarDestinations();
		RefreshAvatarQuestDestination();
		visuals::MenuItems::SetEnabled(LAYOUT_NAME, MENU_ITEM_JOB, game::avatar::Quest::Read().has_value());
	}

	bool RefreshIslands()
	{
		HideVisibleIslands();
		auto islands = game::Islands::GetViewableIslands();
		int icon = 0;
		double dockDistance = game::World::GetDockDistance();
		bool canDock = false;
		for (auto& entry : islands)
		{
			canDock |= (game::Heading::Distance(entry.relativeLocation, { 0.0, 0.0 }) <= dockDistance);
			auto plot = Plot(entry.relativeLocation);
			auto visualId = std::format("AtSeaIsland{}", icon);//TODO: magic string
			visuals::Images::SetLocation(LAYOUT_NAME, visualId, { (int)plot.GetX() + IMAGE_OFFSET_X,(int)plot.GetY() + IMAGE_OFFSET_Y });
			visuals::Images::SetVisible(LAYOUT_NAME, visualId, true);
			visuals::Texts::SetLocation(LAYOUT_NAME, visualId, { (int)plot.GetX() + TEXT_OFFSET_X,(int)plot.GetY() + TEXT_OFFSET_Y });
			visuals::Texts::SetText(LAYOUT_NAME, visualId, (entry.visits.has_value()) ? (entry.name) : (game::Islands::UNKNOWN));
			++icon;
		}
		visuals::MenuItems::SetEnabled(LAYOUT_NAME, MENU_ITEM_DOCK, canDock);
		return canDock;
	}
}
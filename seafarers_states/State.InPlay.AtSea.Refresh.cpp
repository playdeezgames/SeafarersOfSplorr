#include <Common.Heading.h>
#include <format>
#include <Game.Avatar.AtSea.h>
#include <Game.Avatar.Destination.h>
#include <Game.Avatar.Log.h>
#include <Game.Avatar.Quest.h>
#include <Game.Avatar.Statistics.h>
#include <Game.Islands.h>
#include <Game.World.h>
#include <map>
#include <string>
#include <Visuals.Images.h>
#include <Visuals.Menus.h>
#include <Visuals.SpriteGrid.h>
#include <Visuals.Texts.h>
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

	const std::string FORMAT_TURNS = "\x81 {:d}";
	const std::string FORMAT_MONEY = "\x80 {:.3f}";
	const std::string FORMAT_REPUTATION = "\x86 {:.0f}";
	const std::string FORMAT_SATIETY = "\x82 {:.0f}";
	const std::string FORMAT_HEADING = "\x84 {}";
	const std::string FORMAT_SPEED = "\x85 {:.2f}";

	const std::string IMAGE_CURRENT_HEADING = "CurrentHeading";
	const std::string IMAGE_DESTINATION_1 = "Destination1";
	const std::string IMAGE_DESTINATION_2 = "Destination2";
	const std::string IMAGE_DESTINATION_3 = "Destination3";
	const std::string IMAGE_DESTINATION_4 = "Destination4";
	const std::string IMAGE_QUEST_DESTINATION = "QuestDestination";
	const std::string IMAGE_WIND_DIRECTION = "WindDirection";

	const std::string MENU_ITEM_DOCK = "Dock";
	const std::string MENU_ITEM_JOB = "Job";

	const std::string SPRITE_GRID_LOG = "Log";

	const std::string FONT_LOG = "font5x7";
	const size_t LOG_ENTRIES = 20;

	static void RefreshAvatarSatiety()
	{
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_AVATAR_SATIETY, std::format(FORMAT_SATIETY, game::avatar::Statistics::GetSatiety()));
	}

	static void RefreshAvatarMoney()
	{
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_AVATAR_MONEY, std::format(FORMAT_MONEY, game::avatar::Statistics::GetMoney()));
	}

	static void RefreshAvatarReputation()
	{
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_AVATAR_REPUTATION, std::format(FORMAT_REPUTATION, game::avatar::Statistics::GetReputation()));
	}

	static void RefreshAvatarHealth()
	{
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_AVATAR_HEALTH, std::format(game::avatar::Statistics::FORMAT_HEALTH, game::avatar::Statistics::GetHealth()));
	}

	static void RefreshAvatarTurns()
	{
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_AVATAR_TURNS, std::format(FORMAT_TURNS, game::avatar::Statistics::GetTurnsRemaining()));
	}

	static void RefreshAvatarHeading()
	{
		auto heading = game::avatar::AtSea::GetHeading();
		visuals::Texts::SetText(
			LAYOUT_NAME, 
			TEXT_AVATAR_HEADING, 
			std::format(FORMAT_HEADING, common::Heading::ToCompassPoint(heading)));
		visuals::Images::SetAngle(LAYOUT_NAME, IMAGE_CURRENT_HEADING, heading);
	}

	static void RefreshAvatarSpeed()
	{
		auto speed = game::avatar::AtSea::GetSpeed();
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
			auto clampedDistance = common::Heading::ClampDistance(destination.value() - game::avatar::AtSea::GetLocation(), game::World::GetViewDistance() + 0.5);//TODO: magic number
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
		const double FUDGE_FACTOR = 0.5;
		auto quest = game::avatar::Quest::Read();
		if (quest)
		{
			auto clampedDistance = common::Heading::ClampDistance(quest.value().destination - game::avatar::AtSea::GetLocation(), game::World::GetViewDistance() + FUDGE_FACTOR);
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
	const std::string FORMAT_AT_SEA_ISLAND = "AtSeaIsland{}";

	static void RefreshWindDirection()
	{
		auto windHeading = common::Heading::ToRadians(game::World::GetWindHeading()+common::Heading::DEGREES/2.0);
		common::XY<double> plot = { VIEW_CENTER.GetX() + std::cos(windHeading) * VIEW_RADIUS, VIEW_CENTER.GetY() + std::sin(windHeading) * VIEW_RADIUS };
		visuals::Images::SetLocation(LAYOUT_NAME, IMAGE_WIND_DIRECTION, { (int)plot.GetX(), (int)plot.GetY() });
	}

	static void HideVisibleIslands()
	{
		for (int icon = 0; icon < ISLAND_ICON_COUNT; ++icon)
		{
			auto visualId = std::format(FORMAT_AT_SEA_ISLAND, icon);
			visuals::Images::SetVisible(LAYOUT_NAME, visualId, false);
			visuals::Texts::SetText(LAYOUT_NAME, visualId, "");
		}
	}

	static common::XY<double> Plot(const common::XY<double>& location)
	{
		double viewScale = VIEW_RADIUS / game::World::GetViewDistance();
		return location * viewScale + VIEW_CENTER;
	}

	static void RefreshLog()
	{
		visuals::SpriteGrid::Clear(LAYOUT_NAME, SPRITE_GRID_LOG);
		auto entries = game::avatar::Log::Read(LOG_ENTRIES);
		int row = (int)entries.size()-1;
		for (auto entry : entries)
		{
			visuals::SpriteGrid::WriteText(LAYOUT_NAME, SPRITE_GRID_LOG, { 0,row }, FONT_LOG, entry.text, entry.color, visuals::HorizontalAlignment::LEFT);
			--row;
		}
	}

	void RefreshAvatarStatus()
	{
		RefreshWindDirection();
		RefreshAvatarHeading();
		RefreshAvatarHealth();
		RefreshAvatarSatiety();
		RefreshAvatarSpeed();
		RefreshAvatarMoney();
		RefreshAvatarReputation();
		RefreshAvatarTurns();
		RefreshAvatarDestinations();
		RefreshAvatarQuestDestination();
		RefreshLog();
		visuals::MenuItems::SetEnabled(LAYOUT_NAME, MENU_ITEM_JOB, game::avatar::Quest::Read().has_value());
	}

	void RefreshIslands()
	{
		HideVisibleIslands();
		auto islands = game::Islands::GetViewableIslands();
		int icon = 0;
		double dockDistance = game::World::GetDockDistance();
		bool canDock = false;
		for (auto& entry : islands)
		{
			canDock |= (common::Heading::Distance(entry.relativeLocation, { 0.0, 0.0 }) <= dockDistance);
			auto plot = Plot(entry.relativeLocation);
			auto visualId = std::format(FORMAT_AT_SEA_ISLAND, icon);//TODO: magic string
			visuals::Images::SetLocation(LAYOUT_NAME, visualId, { (int)plot.GetX() + IMAGE_OFFSET_X,(int)plot.GetY() + IMAGE_OFFSET_Y });
			visuals::Images::SetVisible(LAYOUT_NAME, visualId, true);
			visuals::Texts::SetLocation(LAYOUT_NAME, visualId, { (int)plot.GetX() + TEXT_OFFSET_X,(int)plot.GetY() + TEXT_OFFSET_Y });
			visuals::Texts::SetText(LAYOUT_NAME, visualId, (entry.visits.has_value()) ? (entry.name) : (game::Islands::UNKNOWN));
			++icon;
		}
		visuals::MenuItems::SetEnabled(LAYOUT_NAME, MENU_ITEM_DOCK, canDock);
	}
}
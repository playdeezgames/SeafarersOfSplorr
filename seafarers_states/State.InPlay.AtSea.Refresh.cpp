#include <Common.Heading.h>
#include <format>
#include <Game.Avatar.AtSea.h>
#include <Game.Avatar.Destinations.h>
#include <Game.Avatar.Log.h>
#include <Game.Avatar.Quest.h>
#include <Game.Avatar.Statistics.h>
#include <Game.Islands.h>
#include <Game.World.h>
#include <map>
#include <string>
#include <Visuals.Images.h>
#include <Visuals.Menus.h>
#include <Visuals.MenuItems.h>
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

	const std::string IMAGE_DESTINATION_1 = "Destination1";
	const std::string IMAGE_DESTINATION_2 = "Destination2";
	const std::string IMAGE_DESTINATION_3 = "Destination3";
	const std::string IMAGE_DESTINATION_4 = "Destination4";
	const std::string IMAGE_QUEST_DESTINATION = "QuestDestination";

	const std::string MENU_ITEM_DOCK = "Dock";

	const std::string SPRITE_GRID_LOG = "Log";

	const std::string FONT_LOG = "font5x7";
	const size_t LOG_ENTRIES = 20;

	static void RefreshAvatarSatiety()
	{
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_AVATAR_SATIETY, std::format(game::avatar::Statistics::FORMAT_SATIETY, game::avatar::Statistics::GetSatiety()));
	}

	static void RefreshAvatarMoney()
	{
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_AVATAR_MONEY, std::format(game::avatar::Statistics::FORMAT_MONEY, game::avatar::Statistics::GetMoney()));
	}

	static void RefreshAvatarReputation()
	{
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_AVATAR_REPUTATION, std::format(game::avatar::Statistics::FORMAT_REPUTATION, game::avatar::Statistics::GetReputation()));
	}

	static void RefreshAvatarHealth()
	{
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_AVATAR_HEALTH, std::format(game::avatar::Statistics::FORMAT_HEALTH, game::avatar::Statistics::GetHealth()));
	}

	static void RefreshAvatarTurns()
	{
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_AVATAR_TURNS, std::format(game::avatar::Statistics::FORMAT_TURNS, game::avatar::Statistics::GetTurnsRemaining()));
	}

	static void RefreshAvatarHeading()//TODO: send to sublayout
	{
		auto heading = game::avatar::AtSea::GetHeading();
		visuals::Texts::SetText(
			LAYOUT_NAME, 
			TEXT_AVATAR_HEADING, 
			std::format(game::avatar::Statistics::FORMAT_HEADING, common::Heading::ToCompassPoint(heading)));
	}

	static void RefreshAvatarSpeed()//TODO: send to sublayout
	{
		auto speed = game::avatar::AtSea::GetSpeed();
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_AVATAR_SPEED, std::format(game::avatar::Statistics::FORMAT_SPEED, speed));
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
		auto destination = game::avatar::Destinations::GetDestination(destinationId);
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
		for (auto destinationId : game::avatar::Destinations::All())
		{
			RefreshAvatarDestination(destinationId);
		}
	}

	static const common::XY<double> VIEW_CENTER = { 162.0,182.0 };
	static const double VIEW_RADIUS = 144.0;

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
		RefreshAvatarHeading();
		RefreshAvatarHealth();
		RefreshAvatarSatiety();
		RefreshAvatarSpeed();
		RefreshAvatarMoney();
		RefreshAvatarReputation();
		RefreshAvatarTurns();
		RefreshAvatarDestinations();
		RefreshLog();
	}
}
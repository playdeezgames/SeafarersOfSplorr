#include <Application.MouseMotion.h>
#include <Common.Heading.h>
#include <format>
#include <Game.Avatar.Destinations.h>
#include <Game.Avatar.Log.h>
#include <Game.Avatar.Quest.h>
#include <Game.Avatar.Statistics.h>
#include <Game.Avatar.StatisticFormats.h>
#include <Game.Islands.h>
#include <Game.Ship.h>
#include <Game.World.h>
#include <map>
#include <string>
#include "Sublayouts.h"
#include "UIState.h"
#include <Visuals.Areas.h>
#include <Visuals.Images.h>
#include <Visuals.Menus.h>
#include <Visuals.MenuItems.h>
#include <Visuals.SpriteGrid.h>
#include <Visuals.Texts.h>
namespace sublayout
{
	static const std::string SUBLAYOUT_NAME = "Sublayout.AtSeaStatistics";
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_AT_SEA;

	static const std::string TEXT_AVATAR_TURNS = "avatar-turns";
	static const std::string TEXT_AVATAR_HEALTH = "avatar-health";
	static const std::string TEXT_AVATAR_SATIETY = "avatar-satiety";
	static const std::string TEXT_AVATAR_HEADING = "avatar-heading";
	static const std::string TEXT_AVATAR_SPEED = "avatar-speed";
	static const std::string TEXT_AVATAR_MONEY = "avatar-money";
	static const std::string TEXT_AVATAR_REPUTATION = "avatar-reputation";
	static const std::string TEXT_TOOL_TIP = "tool-tip";

	static void RefreshAvatarSatiety()
	{
		visuals::Texts::SetText(SUBLAYOUT_NAME, TEXT_AVATAR_SATIETY, game::avatar::StatisticFormats::SATIETY, game::avatar::Statistics::GetSatiety());
	}

	static void RefreshAvatarMoney()
	{
		visuals::Texts::SetText(SUBLAYOUT_NAME, TEXT_AVATAR_MONEY, game::avatar::StatisticFormats::MONEY, game::avatar::Statistics::GetMoney());
	}

	static void RefreshAvatarReputation()
	{
		visuals::Texts::SetText(SUBLAYOUT_NAME, TEXT_AVATAR_REPUTATION, game::avatar::StatisticFormats::REPUTATION, game::avatar::Statistics::GetReputation());
	}

	static void RefreshAvatarHealth()
	{
		visuals::Texts::SetText(SUBLAYOUT_NAME, TEXT_AVATAR_HEALTH, game::avatar::StatisticFormats::HEALTH, game::avatar::Statistics::GetHealth());
	}

	static void RefreshAvatarTurns()
	{
		visuals::Texts::SetText(SUBLAYOUT_NAME, TEXT_AVATAR_TURNS, game::avatar::StatisticFormats::TURNS, game::avatar::Statistics::GetTurnsRemaining());
	}

	static void RefreshAvatarHeading()
	{
		auto heading = game::Ship::GetHeading();
		visuals::Texts::SetText(
			SUBLAYOUT_NAME,
			TEXT_AVATAR_HEADING, 
			game::avatar::StatisticFormats::HEADING,
			common::Heading::ToCompassPoint(heading));
	}

	static void RefreshAvatarSpeed()
	{
		auto speed = game::Ship::GetSpeed();
		visuals::Texts::SetText(
			SUBLAYOUT_NAME, 
			TEXT_AVATAR_SPEED, 
			game::avatar::StatisticFormats::SPEED,
			speed);
	}

	void AtSeaAvatarStatus::Refresh()
	{
		RefreshAvatarHeading();
		RefreshAvatarHealth();
		RefreshAvatarSatiety();
		RefreshAvatarSpeed();
		RefreshAvatarMoney();
		RefreshAvatarReputation();
		RefreshAvatarTurns();
	}

	static void OnMouseMotionInArea(const std::string& areaName, const common::XY<int>&)
	{
		visuals::Texts::SetText(SUBLAYOUT_NAME, TEXT_TOOL_TIP, visuals::Areas::GetToolTip(SUBLAYOUT_NAME, areaName).value_or(""));
	}

	static void OnMouseMotionOutsideAreas(const common::XY<int>&)
	{
		visuals::Texts::SetText(SUBLAYOUT_NAME, TEXT_TOOL_TIP, "");
	}

	void AtSeaAvatarStatus::Start()
	{
		::application::MouseMotion::AddHandler(CURRENT_STATE, visuals::Areas::HandleMouseMotion(SUBLAYOUT_NAME, OnMouseMotionInArea, OnMouseMotionOutsideAreas));
	}

}
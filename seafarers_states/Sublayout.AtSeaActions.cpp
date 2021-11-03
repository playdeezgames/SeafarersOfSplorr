#include <Application.OnEnter.h>
#include <Application.MouseButtonUp.h>
#include <Application.MouseMotion.h>
#include <Application.UIState.h>
#include <Common.Utility.h>
#include <Common.Utility.Dispatcher.h>
#include <Common.Utility.Optional.h>
#include <Common.Utility.Table.h>
#include <functional>
#include <Game.Avatar.h>
#include <Game.Avatar.Actions.h>
#include <Game.Avatar.Items.h>
#include <Game.Player.h>
#include "Sublayouts.h"
#include "UIState.h"
#include <Visuals.Areas.h>
#include <Visuals.Images.h>
namespace sublayout
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_AT_SEA_DEPRECATED;
	static const std::string LAYOUT_NAME = "Sublayout.AtSeaActions";

	static const std::string IMAGE_FISHING_HOVER = "hover-fishing";
	static const std::string IMAGE_FISHING_ACTION = "action-fishing";

	static const std::string AREA_FISHING = "fishing";

	static const std::string SPRITE_FISHING_DISABLED = "AtSeaStartFishingDisabled";
	static const std::string SPRITE_FISHING_ENABLED = "AtSeaStartFishing";

	enum class HoverButton
	{
		FISHING
	};

	struct ActionDescriptor
	{
		std::string hoverImage;
		std::string actionImage;
		std::string enabledSprite;
		std::string disabledSprite;
		std::function<bool()> isEnabled;
	};

	static bool IsFishingEnabled()
	{
		return 
				game::avatar::Items::Has(game::Player::GetAvatarId(), game::Item::FISHING_POLE) &&
				game::avatar::Items::Has(game::Player::GetAvatarId(), game::Item::BAIT);
	}

	static const std::map<HoverButton, ActionDescriptor> actions =
	{
		{HoverButton::FISHING, 
			{
				IMAGE_FISHING_HOVER,
				IMAGE_FISHING_ACTION,
				SPRITE_FISHING_ENABLED,
				SPRITE_FISHING_DISABLED,
				IsFishingEnabled
			}
		}
	};

	static std::optional<HoverButton> hoverButton = std::nullopt;

	static const std::map<std::string, HoverButton> hoverAreas =
	{
		{AREA_FISHING, HoverButton::FISHING}
	};

	static void Refresh()
	{
		for (auto& entry : actions)
		{
			auto& button = entry.first;
			auto& action = entry.second;
			visuals::Images::SetVisible(
				LAYOUT_NAME, 
				action.hoverImage, 
				hoverButton.has_value() && 
				hoverButton.value() == button && 
				action.isEnabled());

			visuals::Images::SetSprite(
				LAYOUT_NAME, 
				action.actionImage, 
				(action.isEnabled()) ?
				(action.enabledSprite) :
				(action.disabledSprite));
		}
	}

	static void OnFishing()
	{
		game::avatar::Actions::DoAction(game::avatar::Action::START_FISHING);
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static const std::map<HoverButton, std::function<void()>> buttonUpHandlers =
	{
		{HoverButton::FISHING, OnFishing}
	};

	static bool OnMouseButtonUpInArea(const std::string& area)
	{
		return common::utility::Dispatcher::Dispatch(buttonUpHandlers, hoverAreas.find(area)->second, true, true);
	}

	static void OnSetHoverButton(const HoverButton& button)
	{
		hoverButton = button;
	}

	static void OnMouseMotionInArea(const std::string& area, const common::XY<int>&)
	{
		hoverButton = std::nullopt;
		common::utility::Optional::Iterate<HoverButton>(
			common::utility::Table::TryGetKey(hoverAreas, area),
			OnSetHoverButton);
		Refresh();
	}

	static void OnMouseMotionOutsideAreas(const common::XY<int>&)
	{
		hoverButton = std::nullopt;
		Refresh();
	}

	void AtSeaActions::Start()
	{
		::application::OnEnter::AddHandler(
			CURRENT_STATE, 
			Refresh);
		::application::MouseButtonUp::AddHandler(
			CURRENT_STATE, 
			visuals::Areas::HandleMouseButtonUp(
				LAYOUT_NAME, 
				OnMouseButtonUpInArea));
		::application::MouseMotion::AddHandler(
			CURRENT_STATE, 
			visuals::Areas::HandleMouseMotion(
				LAYOUT_NAME, 
				OnMouseMotionInArea, 
				OnMouseMotionOutsideAreas));
	}
}
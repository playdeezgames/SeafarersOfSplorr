#include <Application.MouseButtonUp.h>
#include <Application.MouseMotion.h>
#include <Application.UIState.h>
#include <functional>
#include <Game.Avatar.h>
#include <Game.Avatar.Items.h>
#include "Sublayouts.h"
#include "UIState.h"
#include <Visuals.Areas.h>
#include <Visuals.Images.h>
namespace sublayout
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_AT_SEA;
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
				game::avatar::Items::Has(game::Item::FISHING_POLE) && 
				game::avatar::Items::Has(game::Item::BAIT);
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
			visuals::Images::SetVisible(LAYOUT_NAME, entry.second.hoverImage, hoverButton.has_value() && hoverButton.value() == entry.first && entry.second.isEnabled());
			if (entry.second.isEnabled())
			{
				visuals::Images::SetSprite(LAYOUT_NAME, entry.second.actionImage, entry.second.enabledSprite);
			}
			else
			{
				visuals::Images::SetSprite(LAYOUT_NAME, entry.second.actionImage, entry.second.disabledSprite);
			}
		}
	}

	static void OnFishing()
	{
		game::Avatar::DoAction(game::avatar::Action::START_FISHING);
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static bool OnMouseButtonUpInArea(const std::string& area)
	{
		switch (hoverAreas.find(area)->second)
		{
		case HoverButton::FISHING:
			OnFishing();
			break;
		}
		return true;
	}

	static void OnMouseMotionInArea(const std::string& area, const common::XY<int>&)
	{
		hoverButton = std::nullopt;
		auto iter = hoverAreas.find(area);
		if (iter != hoverAreas.end())
		{
			hoverButton = iter->second;
		}
		Refresh();
	}

	static void OnMouseMotionOutsideAreas(const common::XY<int>&)
	{
		hoverButton = std::nullopt;
		Refresh();
	}

	void AtSeaActions::Start()
	{
		::application::MouseButtonUp::AddHandler(CURRENT_STATE, visuals::Areas::HandleMouseButtonUp(LAYOUT_NAME, OnMouseButtonUpInArea));
		::application::MouseMotion::AddHandler(CURRENT_STATE, visuals::Areas::HandleMouseMotion(LAYOUT_NAME, OnMouseMotionInArea, OnMouseMotionOutsideAreas));
	}
}
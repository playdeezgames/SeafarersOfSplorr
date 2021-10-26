#include <Application.Command.h>
#include <Application.MouseButtonUp.h>
#include <Application.MouseMotion.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.Optional.h>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.Ship.h>
#include <Game.Avatar.ShipStatistics.h>
#include <Game.Islands.h>
#include <Game.Ship.h>
#include <Game.ShipTypes.h>
#include "State.InPlay.ShipStatus.h"
#include "UIState.h"
#include <Visuals.Areas.h>
#include <Visuals.Menus.h>
#include <Visuals.MenuItems.h>
#include <Visuals.Texts.h>
namespace state::in_play//20211019
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_SHIP_STATUS;
	static const std::string LAYOUT_NAME = "State.InPlay.ShipStatus";

	static const std::string TEXT_SHIP_NAME = "ShipName";
	static const std::string TEXT_SHIP_TYPE = "ShipType";
	static const std::string TEXT_SPEED_FACTOR = "SpeedFactor";
	static const std::string TEXT_TONNAGE = "Tonnage";
	static const std::string TEXT_FOULING = "Fouling";

	static const std::string FORMAT_SHIP_NAME = "Name: {}";
	static const std::string FORMAT_SHIP_TYPE = "Ship Type: {}";
	static const std::string FORMAT_SPEED_FACTOR = "Speed Factor: {}";
	static const std::string FORMAT_TONNAGE = "Tonnage: {}/{}";
	static const std::string FORMAT_FOULING = "Fouling: {:.0f}%({:.0f}%P, {:.0f}%S)";

	static const std::string MENU_ID = "ShipStatus";
	static const std::string MENU_ITEM_ID = "Careen";

	enum class ShipStatusItem
	{
		CREW,
		CHANGE_SPEED,
		CARGO,
		CAREEN,
		GO_BACK
	};

	static auto OnGoBack = ::application::UIState::GoTo(::UIState::IN_PLAY_NEXT);

	static void UpdateShipProperties()
	{
		auto shipType =
			common::utility::Optional::Map<int, std::optional<game::ShipType>>(
				game::avatar::Ship::ReadShipId(),
				game::Ship::GetShipType).value().value();//i might have overdone it here....

		visuals::Texts::SetText(
			LAYOUT_NAME, 
			TEXT_SHIP_TYPE, 
			FORMAT_SHIP_TYPE, 
			game::ShipTypes::GetName(shipType));

		visuals::Texts::SetText(
			LAYOUT_NAME,
			TEXT_SPEED_FACTOR,
			FORMAT_SPEED_FACTOR,
			game::ShipTypes::GetSpeedFactor(shipType));

		visuals::Texts::SetText(
			LAYOUT_NAME,
			TEXT_SHIP_NAME,
			FORMAT_SHIP_NAME,
			game::Ship::GetName());

		visuals::Texts::SetText(
			LAYOUT_NAME,
			TEXT_TONNAGE,
			FORMAT_TONNAGE,
			game::avatar::Ship::AvailableTonnage().value(),
			game::ShipTypes::GetTotalTonnage(shipType));

		visuals::Texts::SetText(
			LAYOUT_NAME,
			TEXT_FOULING,
			FORMAT_FOULING,
			game::avatar::ShipStatistics::GetFoulingPercentage(),
			game::avatar::ShipStatistics::GetFoulingPercentage(game::Side::PORT),
			game::avatar::ShipStatistics::GetFoulingPercentage(game::Side::STARBOARD));
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		visuals::MenuItems::SetEnabled(
			LAYOUT_NAME, 
			MENU_ITEM_ID, 
			game::Islands::CanDock());
		UpdateShipProperties();
	}

	static const std::map<ShipStatusItem, std::function<void()>> activators =
	{
		{ ShipStatusItem::CREW, ::application::UIState::GoTo(::UIState::IN_PLAY_CREW_LIST) },
		{ ShipStatusItem::CAREEN, ::application::UIState::GoTo(::UIState::IN_PLAY_AT_SEA_CAREEN_SELECT) },
		{ ShipStatusItem::CARGO, ::application::UIState::GoTo(::UIState::IN_PLAY_CARGO) },
		{ ShipStatusItem::GO_BACK, OnGoBack }
	};

	static const auto ActivateItem = visuals::Menus::DoActivateItem(LAYOUT_NAME, MENU_ID, activators);

	static const std::map<Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{ ::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{ ::Command::GREEN, ActivateItem },
		{ ::Command::BACK, OnGoBack },
		{ ::Command::RED, OnGoBack }
	};

	void ShipStatus::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::MouseButtonUp::AddHandler(CURRENT_STATE, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
		::application::MouseMotion::AddHandler(CURRENT_STATE, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::Command::SetHandlers(CURRENT_STATE, commandHandlers);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, LAYOUT_NAME);
	}
}

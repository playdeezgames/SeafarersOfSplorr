#include "Application.Command.h"
#include "Application.MouseButtonUp.h"
#include "Application.OnEnter.h"
#include "Application.Renderer.h"
#include "Application.UIState.h"
#include <format>
#include "Game.Audio.Mux.h"
#include "Game.Avatar.Ship.h"
#include "Game.Ships.h"
#include "Visuals.Texts.h"
namespace state::in_play::ShipStatus
{
	const std::string LAYOUT_NAME = "State.InPlay.ShipStatus";

	const std::string TEXT_SHIP_TYPE = "ShipType";
	const std::string TEXT_SPEED_FACTOR = "SpeedFactor";
	const std::string TEXT_TONNAGE = "Tonnage";

	const std::string FORMAT_SHIP_TYPE = "Ship Type: {}";
	const std::string FORMAT_SPEED_FACTOR = "Speed Factor: {}";
	const std::string FORMAT_TONNAGE = "Tonnage: {}";

	static bool OnMouseButtonUp(const common::XY<int>& xy, MouseButton)
	{
		::application::UIState::Pop();
		return true;
	}

	static void UpdateShipProperties()
	{
		auto shipType = game::avatar::Ship::Read();
		auto& descriptor = game::Ships::Read(shipType);
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_SHIP_TYPE, std::format(FORMAT_SHIP_TYPE, descriptor.name));
		visuals::Texts::SetText(
			LAYOUT_NAME,
			TEXT_SPEED_FACTOR,
			std::format(FORMAT_SPEED_FACTOR, 
				descriptor.properties.find(game::ship::Property::SPEED_FACTOR)->second));
		visuals::Texts::SetText(
			LAYOUT_NAME,
			TEXT_TONNAGE,
			std::format(FORMAT_TONNAGE,
				descriptor.properties.find(game::ship::Property::TONNAGE)->second));
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Mux::Theme::MAIN);
		UpdateShipProperties();
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_SHIP_STATUS, OnEnter);
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_SHIP_STATUS, OnMouseButtonUp);
		::application::Command::SetHandler(::UIState::IN_PLAY_SHIP_STATUS, application::UIState::PopFrom());
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_SHIP_STATUS, LAYOUT_NAME);
	}
}

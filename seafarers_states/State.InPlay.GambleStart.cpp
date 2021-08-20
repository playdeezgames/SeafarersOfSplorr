#include <Application.Renderer.h>
#include <Application.Command.h>
#include <Application.MouseButtonUp.h>
#include <Application.OnEnter.h>
#include <Application.TextInput.h>
#include <Application.UIState.h>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.Docked.h>
#include <Game.Islands.DarkAlley.GamblingHand.h>
#include "UIState.h"
#include <Visuals.Images.h>
#include <Visuals.CardSprites.h>
namespace state::in_play::GambleStart
{
	const std::string LAYOUT_NAME = "State.InPlay.GambleStart";
	const std::string IMAGE_FIRST_CARD = "FirstCard";
	const std::string IMAGE_SECOND_CARD = "SecondCard";

	static void OnLeave()
	{
		game::avatar::Docked::DoDockedAction(game::avatar::DockedAction::ENTER_DARK_ALLEY);
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static bool OnMouseButtonUp(const common::XY<int>& xy, MouseButton)
	{
		OnLeave();
		return true;
	}

	static common::XY<double> GetDockedLocation()
	{
		return game::avatar::Docked::GetDockedLocation().value();
	}

	const size_t FIRST_CARD_INDEX = 0;
	const size_t SECOND_CARD_INDEX = 1;

	static void RefreshCards()
	{
		auto hand = game::islands::dark_alley::GamblingHand::Read(GetDockedLocation());
		visuals::Images::SetSprite(LAYOUT_NAME, IMAGE_FIRST_CARD, visuals::CardSprites::GetSpriteForCard(hand[FIRST_CARD_INDEX]));
		visuals::Images::SetSprite(LAYOUT_NAME, IMAGE_SECOND_CARD, visuals::CardSprites::GetSpriteForCard(hand[SECOND_CARD_INDEX]));
	}

	static void Refresh()
	{
		RefreshCards();
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Mux::Theme::MAIN);
		game::islands::dark_alley::GamblingHand::Deal(GetDockedLocation());
		Refresh();
	}

	static void OnTextInput(const std::string& text)
	{
		return;
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_GAMBLE_START, OnEnter);
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_GAMBLE_START, OnMouseButtonUp);
		::application::Command::SetHandler(::UIState::IN_PLAY_GAMBLE_START, OnLeave);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_GAMBLE_START, LAYOUT_NAME);
		::application::TextInput::AddHandler(::UIState::IN_PLAY_GAMBLE_START, OnTextInput);
	}
}

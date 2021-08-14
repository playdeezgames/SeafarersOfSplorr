#include "Application.Renderer.h"
#include "Application.Command.h"
#include "Application.MouseMotion.h"
#include "Application.MouseButtonUp.h"
#include "Application.OnEnter.h"
#include "Application.UIState.h"
#include "Common.Card.h"
#include <format>
#include "Game.Audio.Mux.h"
#include "Game.Avatar.Docked.h"
#include "Game.Islands.DarkAlley.FightCards.h"
#include "Visuals.Areas.h"
#include "Visuals.CardSprites.h"
#include "Visuals.Images.h"
#include "Visuals.Texts.h"
namespace state::in_play::DarkAlleyEntrance
{
	const std::string LAYOUT_NAME = "State.InPlay.DarkAlleyEntrance";
	const std::string SPRITE_CARD_BACK = "CardBackRed";

	static void OnLeave()
	{
		game::avatar::Docked::DoDockedAction(game::avatar::DockedAction::ENTER_DOCK);
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	const std::map<std::string, size_t> cardAreas =
	{
		{"Card0Hover", 0},
		{"Card1Hover", 1},
		{"Card2Hover", 2},
		{"Card3Hover", 3},
		{"Card4Hover", 4},
		{"Card5Hover", 5},
		{"Card6Hover", 6},
		{"Card7Hover", 7},
		{"Card8Hover", 8},
		{"Card9Hover", 9},
		{"Card10Hover", 10},
		{"Card11Hover", 11}
	};

	const std::map<size_t,std::string> cardSelects =
	{
		{ 0,"Card0Select"},
		{ 1,"Card1Select"},
		{ 2,"Card2Select"},
		{ 3,"Card3Select"},
		{ 4,"Card4Select"},
		{ 5,"Card5Select"},
		{ 6,"Card6Select"},
		{ 7,"Card7Select"},
		{ 8,"Card8Select"},
		{ 9,"Card9Select"},
		{ 10,"Card10Select"},
		{ 11,"Card11Select"}
	};

	const std::map<size_t, std::string> textAdjacents =
	{
		{ 0,"Card0Adjacent"},
		{ 1,"Card1Adjacent"},
		{ 2,"Card2Adjacent"},
		{ 3,"Card3Adjacent"},
		{ 4,"Card4Adjacent"},
		{ 5,"Card5Adjacent"},
		{ 6,"Card6Adjacent"},
		{ 7,"Card7Adjacent"},
		{ 8,"Card8Adjacent"},
		{ 9,"Card9Adjacent"},
		{ 10,"Card10Adjacent"},
		{ 11,"Card11Adjacent"}
	};

	const std::map<size_t, std::string> cardImages =
	{
		{ 0,"Card0"},
		{ 1,"Card1"},
		{ 2,"Card2"},
		{ 3,"Card3"},
		{ 4,"Card4"},
		{ 5,"Card5"},
		{ 6,"Card6"},
		{ 7,"Card7"},
		{ 8,"Card8"},
		{ 9,"Card9"},
		{ 10,"Card10"},
		{ 11,"Card11"}
	};

	static std::optional<size_t> hoverCard = std::nullopt;

	static void RefreshCardSelect()
	{
		for (auto cardSelect : cardSelects)
		{
			visuals::Images::SetVisible(
				LAYOUT_NAME, 
				cardSelect.second, 
				(hoverCard.has_value() && hoverCard.value() == cardSelect.first));
		}
	}

	static void RefreshCards()
	{
		auto fightCards = game::islands::dark_alley::FightCards::Read(game::avatar::Docked::GetDockedLocation().value());
		for (auto& fightCard : fightCards)
		{
			auto& cardImage = cardImages.find(fightCard.first)->second;
			auto& sprite = visuals::CardSprites::GetSpriteForCard(fightCard.second.card);
			visuals::Images::SetSprite(LAYOUT_NAME, cardImage, (fightCard.second.shown) ? (sprite) :(SPRITE_CARD_BACK));
			visuals::Texts::SetText(
				LAYOUT_NAME, 
				textAdjacents.find(fightCard.first)->second, 
				(!fightCard.second.shown || fightCard.second.success) ?
				("") :
				(std::format("{}", fightCard.second.adjacent)));
		}
	}

	static void Refresh()
	{
		RefreshCards();
		RefreshCardSelect();
	}

	static void OnMouseMotionInArea(const std::string& areaName, const common::XY<int>&)
	{
		auto iter = cardAreas.find(areaName);
		if (iter != cardAreas.end())
		{
			hoverCard = iter->second;
			RefreshCardSelect();
		}
	}

	static void OnMouseMotionOutsideArea(const common::XY<int>&)
	{
		hoverCard = std::nullopt;
		RefreshCardSelect();
	}

	static bool OnMouseButtonUpInArea(const std::string&)
	{
		if (hoverCard)
		{
			game::islands::dark_alley::FightCards::Pick(game::avatar::Docked::GetDockedLocation().value(), hoverCard.value());
			Refresh();
		}
		return false;
	}

	static void ResetDisplay()
	{
		for (auto& textAdjacent : textAdjacents)
		{
			visuals::Texts::SetText(LAYOUT_NAME, textAdjacent.second, "");
		}
		for (auto& cardImage : cardImages)
		{
			visuals::Images::SetSprite(LAYOUT_NAME, cardImage.second, SPRITE_CARD_BACK);
		}
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Mux::Theme::MAIN);
		game::islands::dark_alley::FightCards::Generate(game::avatar::Docked::GetDockedLocation().value());
		hoverCard = std::nullopt;
		ResetDisplay();
		Refresh();
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_DARK_ALLEY_ENTRANCE, OnEnter);
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_DARK_ALLEY_ENTRANCE, visuals::Areas::HandleMouseMotion(LAYOUT_NAME, OnMouseMotionInArea, OnMouseMotionOutsideArea));
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_DARK_ALLEY_ENTRANCE, visuals::Areas::HandleMouseButtonUp(LAYOUT_NAME, OnMouseButtonUpInArea));
		::application::Command::SetHandler(::UIState::IN_PLAY_DARK_ALLEY_ENTRANCE, OnLeave);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_DARK_ALLEY_ENTRANCE, LAYOUT_NAME);
	}
}

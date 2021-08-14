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

	struct CardPosition
	{
		std::string adjacentText;
		std::string cardImage;
		std::string cardSelectImage;
	};

	const std::map<size_t, CardPosition> cardPositions =
	{
		{
			0, 
			{
				"Card0Adjacent",
				"Card0",
				"Card0Select"
			}
		},
		{
			1,
			{
				"Card1Adjacent",
				"Card1",
				"Card1Select"
			}
		},
		{
			2,
			{
				"Card2Adjacent",
				"Card2",
				"Card2Select"
			}
		},
		{
			3,
			{
				"Card3Adjacent",
				"Card3",
				"Card3Select"
			}
		},
		{
			4,
			{
				"Card4Adjacent",
				"Card4",
				"Card4Select"
			}
		},
		{
			5,
			{
				"Card5Adjacent",
				"Card5",
				"Card5Select"
			}
		},
		{
			6,
			{
				"Card6Adjacent",
				"Card6",
				"Card6Select"
			}
		},
		{
			7,
			{
				"Card7Adjacent",
				"Card7",
				"Card7Select"
			}
		},
		{
			8,
			{
				"Card8Adjacent",
				"Card8",
				"Card8Select"
			}
		},
		{
			9,
			{
				"Card9Adjacent",
				"Card9",
				"Card9Select"
			}
		},
		{
			10,
			{
				"Card10Adjacent",
				"Card10",
				"Card10Select"
			}
		},
		{
			11,
			{
				"Card11Adjacent",
				"Card11",
				"Card11Select"
			}
		}
	};

	static std::optional<size_t> hoverCard = std::nullopt;

	static void RefreshCardSelect()
	{
		for (auto cardPosition : cardPositions)
		{
			visuals::Images::SetVisible(
				LAYOUT_NAME, 
				cardPosition.second.cardSelectImage,
				(hoverCard.has_value() && hoverCard.value() == cardPosition.first));
		}
	}

	static void RefreshCards()
	{
		auto fightCards = game::islands::dark_alley::FightCards::Read(game::avatar::Docked::GetDockedLocation().value());
		for (auto& fightCard : fightCards)
		{
			auto& cardImage = cardPositions.find(fightCard.first)->second.cardImage;
			auto& sprite = visuals::CardSprites::GetSpriteForCard(fightCard.second.card);
			visuals::Images::SetSprite(LAYOUT_NAME, cardImage, (fightCard.second.shown) ? (sprite) :(SPRITE_CARD_BACK));
			visuals::Texts::SetText(
				LAYOUT_NAME, 
				cardPositions.find(fightCard.first)->second.adjacentText, 
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
		for (auto& cardPosition : cardPositions)
		{
			visuals::Texts::SetText(LAYOUT_NAME, cardPosition.second.adjacentText, "");
			visuals::Images::SetSprite(LAYOUT_NAME, cardPosition.second.cardImage, SPRITE_CARD_BACK);
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

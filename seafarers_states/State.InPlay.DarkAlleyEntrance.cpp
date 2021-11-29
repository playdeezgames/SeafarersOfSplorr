#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Data.Game.Island.h>
#include <Game.Audio.Mux.h>
#include <Game.Audio.Sfx.h>
#include <Game.Character.Actions.h>
#include <Game.Character.Docked.h>
#include <Game.Character.Statistics.h>
#include <Game.Colors.h>
#include <Game.Islands.DarkAlley.h>
#include <Game.Islands.DarkAlley.FightCard.h>
#include <Game.Player.h>
#include "State.InPlay.DarkAlleyEntrance.h"
#include "State.Terminal.h"
#include "UIState.h"
#include <Visuals.Messages.h>
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_DARK_ALLEY_ENTRANCE;
	static const auto ReadLocation = []() { return data::game::Island::Read(game::character::Docked::ReadLocation(game::Player::GetCharacterId()).value()).value().location; };
	static const auto GetRuffianBrawling = []() { return game::islands::DarkAlley::GetRuffianBrawling(game::character::Docked::ReadLocation(game::Player::GetCharacterId()).value()).value(); };
	static const size_t ROWS = 3;
	static const size_t COLUMNS = 4;
	static size_t hitsTaken = 0;

	static const std::string CARD_TOP = "   \xda\xc4\xc4\xc4\xbf";
	static const std::string CARD_MIDDLE_FACE_DOWN_LINE1_A = "{:2d})";
	static const std::string CARD_MIDDLE_FACE_DOWN_LINE1_B = "\xb3\xb0\xb0\xb0\xb3";
	static const std::string CARD_MIDDLE_FACE_DOWN_LINE2 = "   \xb3\xb0\xb0\xb0\xb3";
	static const std::string CARD_BOTTOM = "   \xc0\xc4\xc4\xc4\xd9";

	static void RefreshRowTop()
	{
		Terminal::SetForeground(game::Colors::GRAY);
		for (size_t column = 0; column < COLUMNS; ++column)
		{
			Terminal::Write(CARD_TOP);
		}
		Terminal::WriteLine();
	}

	static void RefreshRowBottom()
	{
		for (size_t column = 0; column < COLUMNS; ++column)
		{
			Terminal::Write(CARD_BOTTOM);
		}
		Terminal::WriteLine();
	}

	const std::map<cards::Rank, std::string> DarkAlleyEntrance::Ranks =
	{
		{cards::Rank::ACE, "A "},
		{cards::Rank::DEUCE, "2 "},
		{cards::Rank::THREE, "3 "},
		{cards::Rank::FOUR, "4 "},
		{cards::Rank::FIVE, "5 "},
		{cards::Rank::SIX, "6 "},
		{cards::Rank::SEVEN, "7 "},
		{cards::Rank::EIGHT, "8 "},
		{cards::Rank::NINE, "9 "},
		{cards::Rank::TEN, "10"},
		{cards::Rank::JACK, "J "},
		{cards::Rank::QUEEN, "Q "},
		{cards::Rank::KING, "K "}
	};

	const std::map<cards::Suit, std::string> DarkAlleyEntrance::SuitColors =
	{
		{cards::Suit::CLUB, game::Colors::DARK_GRAY},
		{cards::Suit::DIAMOND, game::Colors::RED},
		{cards::Suit::HEART, game::Colors::RED},
		{cards::Suit::SPADE, game::Colors::DARK_GRAY}
	};

	const std::map<cards::Suit, std::string> DarkAlleyEntrance::Suits =
	{
		{cards::Suit::CLUB, "\x05"},
		{cards::Suit::DIAMOND, "\x04"},
		{cards::Suit::HEART, "\x03"},
		{cards::Suit::SPADE, "\x06"}
	};

	static void RefreshCardLine1(const std::map<size_t, game::islands::dark_alley::FightCard>& cards, size_t row, size_t column)
	{
		auto card = cards.find(row * COLUMNS + column)->second;
		if (card.revealed)
		{
			Terminal::SetForeground(game::Colors::GRAY);
			Terminal::Write("   \xb3");
			auto suit = std::get<1>(card.card);
			Terminal::SetForeground(DarkAlleyEntrance::SuitColors.find(suit)->second);
			auto rank = std::get<0>(card.card);
			Terminal::Write(DarkAlleyEntrance::Ranks.find(rank)->second);
			Terminal::Write(DarkAlleyEntrance::Suits.find(suit)->second);
			Terminal::SetForeground(game::Colors::GRAY);
			Terminal::Write("\xb3");
		}
		else
		{
			Terminal::SetForeground(game::Colors::YELLOW);
			Terminal::Write(CARD_MIDDLE_FACE_DOWN_LINE1_A, row * COLUMNS + column + 1);
			Terminal::SetForeground(game::Colors::GRAY);
			Terminal::Write(CARD_MIDDLE_FACE_DOWN_LINE1_B);
		}
	}

	static void RefreshRowLine1(const std::map<size_t, game::islands::dark_alley::FightCard>& cards, size_t row)
	{
		for (size_t column = 0; column < COLUMNS; ++column)
		{
			RefreshCardLine1(cards, row, column);
		}
		Terminal::WriteLine();
	}

	static void RefreshCardLine2(const std::map<size_t, game::islands::dark_alley::FightCard>& cards, size_t row, size_t column)
	{
		auto card = cards.find(row * COLUMNS + column)->second;
		if (card.revealed)
		{
			Terminal::Write("   \xb3{:2d}\x20\xb3", card.adjacentSuccesses);
		}
		else
		{
			Terminal::Write(CARD_MIDDLE_FACE_DOWN_LINE2);
		}
	}

	static void RefreshRowLine2(const std::map<size_t, game::islands::dark_alley::FightCard>& cards, size_t row)
	{
		for (size_t column = 0; column < COLUMNS; ++column)
		{
			RefreshCardLine2(cards, row, column);
		}
		Terminal::WriteLine();
	}

	static void RefreshRow(const std::map<size_t, game::islands::dark_alley::FightCard>& cards, size_t row)
	{
		RefreshRowTop();
		RefreshRowLine1(cards, row);
		RefreshRowLine2(cards, row);
		RefreshRowBottom();
	}

	static void RefreshBoard()
	{
		auto fightCards = game::islands::dark_alley::FightCard::Read(ReadLocation());
		for (size_t row = 0; row < ROWS; ++row)
		{
			RefreshRow(fightCards, row);
		}
	}

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Pick a face card to defeat enemy");
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine("Enemy Brawling: {:.1f}", GetRuffianBrawling());
		Terminal::WriteLine(
			"Yer Brawling: {:.1f} Yer Health: {:.0f}", 
			game::character::Statistics::GetBrawling(game::Player::GetCharacterId()),
			game::character::Statistics::GetHealth(game::Player::GetCharacterId()));

		RefreshBoard();

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("0) Retreat");

		Terminal::ShowPrompt();
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		hitsTaken = 0;
		game::islands::dark_alley::FightCard::Generate(ReadLocation());
		Refresh();
	}

	static void OnRetreat()
	{
		game::character::Statistics::ChangeMoney(game::Player::GetCharacterId(), -game::character::Statistics::ReadMoney(game::Player::GetCharacterId()) / 2.0);
		game::character::Actions::DoAction(game::Player::GetCharacterId(), game::character::Action::ENTER_DOCK);
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void IncreaseInfamy()
	{
		const double INFAMY_DELTA = 0.1;
		game::character::Statistics::ChangeInfamy(game::Player::GetCharacterId(), (hitsTaken == 0) ? (INFAMY_DELTA) : (INFAMY_DELTA / 2.0));
	}

	static void IncreaseBrawling()
	{
		const double BRAWLING_DELTA = 0.1;
		game::character::Statistics::ChangeBrawling(game::Player::GetCharacterId(), (hitsTaken > 0) ? (BRAWLING_DELTA) : (BRAWLING_DELTA / 2.0));
	}

	static void HandleRuffianDefeated()
	{
		game::audio::Sfx::Play(game::audio::GameSfx::ENEMY_HIT);
		RefreshBoard();
		visuals::Messages::Write({ "VICTORY!",{} });
		IncreaseInfamy();
		IncreaseBrawling();
		game::character::Actions::DoAction(game::Player::GetCharacterId(), game::character::Action::DEFEAT_RUFFIAN);
		application::UIState::Write(::UIState::IN_PLAY_NEXT);

	}

	static void HandleTakeDamage()
	{
		game::character::Statistics::ChangeHealth(game::Player::GetCharacterId(), -GetRuffianBrawling());
		if (game::character::Statistics::IsDead(game::Player::GetCharacterId()))
		{
			visuals::Messages::Write({ "DEFEAT!",{} });
			application::UIState::Write(::UIState::IN_PLAY_NEXT);
		}
		game::audio::Sfx::Play(game::audio::GameSfx::HIT);
		hitsTaken++;
	}

	static void HandleFightCard(const game::islands::dark_alley::FightCard& fightCard)
	{
		if (fightCard.success)
		{
			HandleRuffianDefeated();
			return;
		}
		HandleTakeDamage();
	}

	static void PickCard(size_t cardIndex)
	{
		auto fightCards = game::islands::dark_alley::FightCard::Read(ReadLocation());
		auto card = fightCards.find(cardIndex)->second;
		if (card.revealed)
		{
			Terminal::WriteLine(Terminal::INVALID_INPUT);
			Refresh();
		}
		else
		{
			auto card = game::islands::dark_alley::FightCard::Pick(ReadLocation(), cardIndex);
			if (card)
			{
				HandleFightCard(card.value());
				Refresh();
			}
		}
	}

	static std::function<void()> DoPickCard(size_t cardIndex)
	{
		return [cardIndex]() 
		{
			PickCard(cardIndex);
		};
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{ "1", DoPickCard(0) },
		{ "2", DoPickCard(1) },
		{ "3", DoPickCard(2) },
		{ "4", DoPickCard(3) },
		{ "5", DoPickCard(4) },
		{ "6", DoPickCard(5) },
		{ "7", DoPickCard(6) },
		{ "8", DoPickCard(7) },
		{ "9", DoPickCard(8) },
		{ "10", DoPickCard(9) },
		{ "11", DoPickCard(10) },
		{ "12", DoPickCard(11) },
		{ "0", OnRetreat }
	};

	void DarkAlleyEntrance::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoIntegerInput(
				menuActions,
				Terminal::INVALID_INPUT,
				Refresh));
	}
}
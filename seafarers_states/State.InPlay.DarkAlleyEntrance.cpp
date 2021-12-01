#include <Game.Character.Statistics.h>
#include <Game.Islands.DarkAlley.h>
#include <Game.Islands.DarkAlley.FightCard.h>
#include "State.InPlay.DarkAlleyEntrance.h"
#include "State.InPlay.Globals.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_DARK_ALLEY_ENTRANCE;
	static const auto GetRuffianBrawling = []() { return game::islands::DarkAlley::GetRuffianBrawling(GetPlayerCharacterIslandId().value()).value(); };
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

	static void RefreshCardLine1(const std::map<size_t, game::islands::dark_alley::FightCard>& cards, size_t row, size_t column)
	{
		auto card = cards.find(row * COLUMNS + column)->second;
		if (card.revealed)
		{
			Terminal::SetForeground(game::Colors::GRAY);
			Terminal::Write("   \xb3");
			auto suit = std::get<1>(card.card);
			Terminal::SetForeground(GetSuitColors().find(suit)->second);
			auto rank = std::get<0>(card.card);
			Terminal::Write(GetRanks().find(rank)->second);
			Terminal::Write(GetSuits().find(suit)->second);
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
		auto fightCards = game::islands::dark_alley::FightCard::Read(GetPlayerCharacterIslandId().value());
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
			game::character::Statistics::GetBrawling(GetPlayerCharacterId()),
			game::character::Statistics::GetHealth(GetPlayerCharacterId()));

		RefreshBoard();

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("0) Retreat");

		Terminal::ShowPrompt();
	}

	static void OnEnter()
	{
		PlayMainTheme();
		hitsTaken = 0;
		game::islands::dark_alley::FightCard::Generate(GetPlayerCharacterId(), GetPlayerCharacterIslandId().value());
		Refresh();
	}

	static void OnRetreat()
	{
		ChangePlayerCharacterMoney(-GetPlayerCharacterMoney().value() / 2.0);
		DoPlayerCharacterAction(game::character::Action::ENTER_DOCK);
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void IncreaseInfamy()
	{
		const double INFAMY_DELTA = 0.1;
		game::character::Statistics::ChangeInfamy(GetPlayerCharacterId(), (hitsTaken == 0) ? (INFAMY_DELTA) : (INFAMY_DELTA / 2.0));
	}

	static void IncreaseBrawling()
	{
		const double BRAWLING_DELTA = 0.1;
		game::character::Statistics::ChangeBrawling(GetPlayerCharacterId(), (hitsTaken > 0) ? (BRAWLING_DELTA) : (BRAWLING_DELTA / 2.0));
	}

	static void HandleRuffianDefeated()
	{
		game::audio::Sfx::Play(game::audio::GameSfx::ENEMY_HIT);
		RefreshBoard();
		Terminal::WriteLine("VICTORY!");
		IncreaseInfamy();
		IncreaseBrawling();
		DoPlayerCharacterAction(game::character::Action::DEFEAT_RUFFIAN);
		application::UIState::Write(::UIState::IN_PLAY_NEXT);

	}

	static void HandleTakeDamage()
	{
		game::character::Statistics::ChangeHealth(GetPlayerCharacterId(), -GetRuffianBrawling());
		if (game::character::Statistics::IsDead(GetPlayerCharacterId()))
		{
			Terminal::WriteLine("DEFEAT!");
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
		auto fightCards = game::islands::dark_alley::FightCard::Read(GetPlayerCharacterIslandId().value());
		auto card = fightCards.find(cardIndex)->second;
		if (card.revealed)
		{
			Terminal::WriteLine(Terminal::INVALID_INPUT);
			Refresh();
		}
		else
		{
			auto card = game::islands::dark_alley::FightCard::Pick(GetPlayerCharacterIslandId().value(), cardIndex);
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
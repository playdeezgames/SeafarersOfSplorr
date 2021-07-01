#include "Application.UIState.h"
#include "Game.Avatar.h"
#include "Game.Creatures.h"
#include "Game.Avatar.Statistics.h"
#include "Application.Sounds.h"
#include "Game.h"
#include "Game.Achievements.h"
namespace application::UIState
{
	static ::UIState uiState = ::UIState::SPLASH;

	void Write(const ::UIState& state)
	{
		uiState = state;
	}

	std::function<void()> GoTo(::UIState uiState)
	{
		return [uiState]() 
		{
			Write(uiState);
		};
	}


	const ::UIState& Read()
	{
		return uiState;
	}

	std::optional<std::string> EnterGame()
	{
		if (game::Avatar::HasTeleportFlag())
		{
			game::Avatar::ClearTeleportFlag();
			application::UIState::Write(::UIState::IN_PLAY_TELEPORT);
			return std::nullopt;//TODO: sound effect for teleporting?
		}

		if (game::avatar::Statistics::IsMinimum(game::avatar::Statistic::HEALTH))//are you dead?
		{
			auto killedByStatistic = game::avatar::Statistics::GetKilledByStatistic();
			if (killedByStatistic)
			{
				game::Statistics::Increment(killedByStatistic.value());
			}
			auto deathByAchievement = game::avatar::Statistics::GetDeathByAchievement();
			if (deathByAchievement)
			{
				game::Achievements::Add(deathByAchievement.value());
			}
			game::Achievements::Add(game::Achievement::YER_DEAD);
			application::UIState::Write(::UIState::IN_PLAY_DEAD);
			return application::Sounds::Read(application::UI::Sfx::DEAD_HUNTER);
		}

		if (!game::Creatures::AnyLeft())//did you win?
		{
			application::UIState::Write(::UIState::IN_PLAY_EXIT);
			return application::Sounds::Read(application::UI::Sfx::EXIT);
		}

		if (game::Creatures::GetInstance(game::Avatar::GetPosition()))//are you fighting?
		{
			application::UIState::Write(::UIState::IN_PLAY_COMBAT);
			return std::nullopt;
		}

		//yer exploring
		game::AutoSave();
		application::UIState::Write(::UIState::IN_PLAY_MAP);
		return std::nullopt;
	}
}
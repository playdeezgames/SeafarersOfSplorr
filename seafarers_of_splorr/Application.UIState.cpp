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
			return application::Sounds::Read(application::UI::Sfx::WOOHOO);
		}

		if (!game::Creatures::AnyLeft())//did you win?
		{
			return application::Sounds::Read(application::UI::Sfx::WOOHOO);
		}

		if (game::Creatures::GetInstance(game::Avatar::GetPosition()))//are you fighting?
		{
			return std::nullopt;
		}

		//yer exploring
		game::AutoSave();
		return std::nullopt;
	}
}
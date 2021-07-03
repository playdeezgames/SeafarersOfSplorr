#pragma once
#include "Application.UIState.h"
#include <optional>
#include <string>
#include <functional>
enum class UIState
{
	SPLASH,
	MAIN_MENU,
	CONFIRM_QUIT,
	QUIT,
	ABOUT,
	OPTIONS,
	START_GAME,
	LEAVE_PLAY,
	LOAD_GAME,
	SAVE_GAME,
	CONFIRM_ABANDON,
	CONFIRM_OVERWRITE_SLOT1,
	CONFIRM_OVERWRITE_SLOT2,
	CONFIRM_OVERWRITE_SLOT3,
	CONFIRM_OVERWRITE_SLOT4,
	CONFIRM_OVERWRITE_SLOT5,
	TIP,
	STATISTICS,
	IN_PLAY_AT_SEA
};
namespace application::UIState
{
	void Write(const ::UIState&);
	const ::UIState& Read();
	std::optional<std::string> EnterGame();
	std::function<void()> GoTo(::UIState);
}
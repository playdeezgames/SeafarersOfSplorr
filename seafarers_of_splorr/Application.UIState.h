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
	IN_PLAY_AT_SEA,
	IN_PLAY_CHANGE_HEADING,
	IN_PLAY_CHANGE_SPEED,
	IN_PLAY_DOCKED,
	IN_PLAY_WIN,
	IN_PLAY_LOSE,
	IN_PLAY_HEAD_FOR,
	IN_PLAY_ISLAND_JOBS,
	IN_PLAY_CONFIRM_REPLACE_JOB,
	IN_PLAY_CONFIRM_ABANDON_JOB,
	IN_PLAY_CURRENT_JOB
};
namespace application::UIState
{
	void Write(const ::UIState&);
	const ::UIState& Read();
	std::optional<std::string> EnterGame();
	std::function<void()> GoTo(const ::UIState&);

	void Push(const ::UIState&);
	void Pop();
	std::function<void()> PushTo(const ::UIState&);
}
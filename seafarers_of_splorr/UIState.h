#pragma once
enum class UIState//TODO: likely moves to seafarers_state
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
	IN_PLAY_CURRENT_JOB,
	IN_PLAY_ISLAND_TRADE,
	IN_PLAY_ISLAND_BUY,
	IN_PLAY_ISLAND_SELL,
	IN_PLAY_CARGO,
	IN_PLAY_SHIP_STATUS,
	IN_PLAY_NEXT,
	IN_PLAY_SHIPYARD,
	IN_PLAY_DARK_ALLEY_ENTRANCE,
	IN_PLAY_DARK_ALLEY,
	IN_PLAY_MESSAGE,
	IN_PLAY_CONFIRM
};

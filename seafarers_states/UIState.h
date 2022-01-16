#pragma once
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
	CHOOSE_START_TYPE,
	DETAILED_START,
	IN_PLAY_AT_SEA_OVERVIEW,
	IN_PLAY_CHANGE_SPEED,
	IN_PLAY_DOCKED,
	IN_PLAY_WIN,
	IN_PLAY_LOSE,
	IN_PLAY_NEXT,
	IN_PLAY_MESSAGE,
	IN_PLAY_CREW_LIST,
	IN_PLAY_CREW_DETAIL,
	IN_PLAY_SHIP_STATUS,
	IN_PLAY_CHANGE_HEADING, 
	IN_PLAY_MANUAL_HEADING,
	IN_PLAY_DOCK_OR_CAREEN,
	IN_PLAY_HEAD_FOR_NEAR_BY,
	IN_PLAY_HEAD_FOR_KNOWN,
	IN_PLAY_MULTIPLE_MOVE,
	IN_PLAY_RENAME_SHIP_ADJECTIVE,
	IN_PLAY_RENAME_SHIP_NOUN,
	IN_PLAY_CREW_DETAIL_CHARACTERISTICS,
	IN_PLAY_HEAD_FOR_MARK,
	IN_PLAY_MARK_LOCATION,
	IN_PLAY_CONFIRM_HEAD_FOR_MARK,
	CHOOSE_ELECTIVE_SKILL_CATEGORIES,
	CHOOSE_PROFESSIONAL_SKILL,
	SPEND_PROFESSIONAL_SKILL_POINTS,
	CHOOSE_PERSONAL_SKILL,
	SPEND_PERSONAL_SKILL_POINTS,
	IN_PLAY_ISLAND_DISTRICT,
	IN_PLAY_DELIVERY_SERVICE,
	IN_PLAY_DELIVERY_SERVICE_DELIVERY_LIST,
	IN_PLAY_DELIVERY_SERVICE_DELIVERY_DETAIL,
	IN_PLAY_CREW_DETAIL_DELIVERIES,
	IN_PLAY_CREW_DETAIL_DELIVERY_DETAIL,
	IN_PLAY_CREW_DETAIL_CONFIRM_ABANDON_DELIVERY,
	IN_PLAY_DELIVERY_SERVICE_MAKE_DELIVERY
};

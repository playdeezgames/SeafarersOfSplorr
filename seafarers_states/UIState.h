#pragma once
enum class UIState
{
	SPLASH,//this is the first state, so needs to remain here
	QUIT,//this is the final state, so needs to remain here
	CONFIRM_OVERWRITE_SLOT1,//keep it here for now, because its a bit weird
	CONFIRM_OVERWRITE_SLOT2,//keep it here for now, because its a bit weird
	CONFIRM_OVERWRITE_SLOT3,//keep it here for now, because its a bit weird
	CONFIRM_OVERWRITE_SLOT4,//keep it here for now, because its a bit weird
	CONFIRM_OVERWRITE_SLOT5,//keep it here for now, because its a bit weird

	IN_PLAY_DELIVERY_SERVICE,
	IN_PLAY_DELIVERY_SERVICE_DELIVERY_LIST,
	IN_PLAY_DELIVERY_SERVICE_DELIVERY_DETAIL,
	IN_PLAY_CREW_DETAIL_DELIVERIES,
	IN_PLAY_CREW_DETAIL_DELIVERY_DETAIL,
	IN_PLAY_CREW_DETAIL_CONFIRM_ABANDON_DELIVERY,
	IN_PLAY_DELIVERY_SERVICE_MAKE_DELIVERY,
	IN_PLAY_CREW_DETAIL_INVENTORY,
	IN_PLAY_CREW_DETAIL_INVENTORY_DETAIL,
	IN_PLAY_STREET_VENDOR,

	REGISTRAR_STARTS_HERE//so that the registrar's state id can get property initialized
};

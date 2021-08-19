#include "Application.Command.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Application.OnEnter.h"
#include "Application.Renderer.h"
#include "Application.UIState.h"
#include "Audio.h"
#include "Audio.Mux.h"
#include "Audio.Sfx.h"
#include "Application.Engine.h"
#include "Common.Data.h"
#include "Common.Utility.h"
#include "Game.Audio.Mux.h"
#include "Options.h"
#include <format>
#include <SDL_mixer.h>
#include "UIState.h"
#include "Visuals.Menus.h"
#include "Visuals.Areas.h"
namespace state::Options
{
	const std::string LAYOUT_NAME = "State.Options";
	const std::string MENU_ID = "Options";
	const std::string MENU_ITEM_MUTE = "Mute";
	const std::string MENU_ITEM_SFX_VOLUME = "SfxVolume";
	const std::string MENU_ITEM_MUX_VOLUME = "MuxVolume";
	const std::string MUTE = "Mute";
	const std::string UNMUTE = "Unmute";
	const std::string AREA_SFX_DECREASE = "DecreaseSfxVolume";
	const std::string AREA_SFX_INCREASE = "IncreaseSfxVolume";
	const std::string AREA_MUX_DECREASE = "DecreaseMuxVolume";
	const std::string AREA_MUX_INCREASE = "IncreaseMuxVolume";
	const int VOLUME_DELTA = 8;
	const std::string FORMAT_SFX_VOLUMNE = "< SFX Volume ({}%) >";
	const std::string FORMAT_MUX_VOLUMNE = "< MUX Volume ({}%) >";

	enum class OptionsItem
	{
		TOGGLE_MUTE,
		SFX_VOLUME,
		MUX_VOLUME,
		TOGGLE_FULLSCREEN,
		BACK
	};

	const std::string SFX_SAMPLE = "woohoo";

	static void AdjustSfxVolume(int delta)
	{
		::audio::Sfx::SetVolume(::audio::Sfx::GetVolume() + delta);
		::audio::Sfx::Play(SFX_SAMPLE);
		::Options::Save();
	}

	static void AdjustMuxVolume(int delta)
	{
		::audio::Mux::SetVolume(::audio::Mux::GetVolume() + delta);
		::Options::Save();
	}

	static OptionsItem GetCurrentItem()
	{
		return (OptionsItem)visuals::Menus::ReadIndex(LAYOUT_NAME, MENU_ID).value();
	}

	const std::map<OptionsItem, std::function<void(int)>> itemChangers =
	{
		{ OptionsItem::SFX_VOLUME, AdjustSfxVolume },
		{ OptionsItem::MUX_VOLUME, AdjustMuxVolume }
	};

	static void ChangeItem(int delta)
	{
		auto iter = itemChangers.find(GetCurrentItem());
		if (iter != itemChangers.end())
		{
			iter->second(delta);
		}
	}

	static void RefreshMuteMenuItem()
	{
		visuals::MenuItems::SetText(LAYOUT_NAME, MENU_ITEM_MUTE,
			Audio::IsMuted() ? UNMUTE : MUTE);
	}

	static void RefreshSfxMenuItem()
	{
		std::string ss = std::format(FORMAT_SFX_VOLUMNE, common::Data::ToPercentage(::audio::Sfx::GetVolume(), MIX_MAX_VOLUME).value());
		visuals::MenuItems::SetText(LAYOUT_NAME, MENU_ITEM_SFX_VOLUME, ss);
	}

	static void RefreshMuxMenuItem()
	{
		std::string ss = std::format(FORMAT_MUX_VOLUMNE, common::Data::ToPercentage(::audio::Mux::GetVolume(), MIX_MAX_VOLUME).value());
		visuals::MenuItems::SetText(LAYOUT_NAME, MENU_ITEM_MUX_VOLUME, ss);
	}

	static void Refresh()
	{
		RefreshMuteMenuItem();
		RefreshSfxMenuItem();
		RefreshMuxMenuItem();
	}

	static void IncreaseItem()
	{
		ChangeItem(VOLUME_DELTA);
		Refresh();
	}

	static void DecreaseItem()
	{
		ChangeItem(-VOLUME_DELTA);
		Refresh();
	}

	static void ToggleMute()
	{
		Audio::SetMuted(!Audio::IsMuted()); 
		::Options::Save(); 
		Refresh();
	}

	static void ToggleFullscreen()
	{
		application::Engine::SetFullscreen(!application::Engine::IsFullscreen());
		::Options::Save();
	}

	const std::map<OptionsItem, std::function<void()>> activators =
	{
		{ OptionsItem::TOGGLE_MUTE, ToggleMute },
		{ OptionsItem::BACK, ::application::UIState::Pop },
		{ OptionsItem::TOGGLE_FULLSCREEN, ToggleFullscreen }

	};

	const auto ActivateItem = visuals::Menus::DoActivateItem(LAYOUT_NAME, MENU_ID, activators);

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{::Command::LEFT, DecreaseItem },
		{::Command::RIGHT, IncreaseItem },
		{::Command::BACK, ::application::UIState::GoTo(::UIState::MAIN_MENU) },
		{::Command::RED, ::application::UIState::GoTo(::UIState::MAIN_MENU) },
		{ ::Command::GREEN, ActivateItem }
	};

	static void OnCommand(const ::Command& command)
	{
		common::Utility::Dispatch(commandHandlers, command);
	}

	const std::map<std::string, std::function<void()>> areaClickActions =
	{
		{AREA_MUX_DECREASE, DecreaseItem},
		{AREA_SFX_DECREASE, DecreaseItem},
		{AREA_MUX_INCREASE, IncreaseItem},
		{AREA_SFX_INCREASE, IncreaseItem}
	};

	static bool OnMouseButtonUpInArea(const std::string& area)
	{
		auto iter = areaClickActions.find(area);
		if (iter != areaClickActions.end())
		{
			iter->second();
			return true;
		}
		ActivateItem();
		return true;
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::OPTIONS, game::audio::Mux::GoToTheme(game::audio::Mux::Theme::MAIN));
		::application::MouseButtonUp::AddHandler(::UIState::OPTIONS, visuals::Areas::HandleMouseButtonUp(LAYOUT_NAME, OnMouseButtonUpInArea));
		::application::MouseMotion::AddHandler(::UIState::OPTIONS, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::Command::SetHandlers(::UIState::OPTIONS, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::OPTIONS, LAYOUT_NAME);
		::application::OnEnter::AddHandler(::UIState::OPTIONS, Refresh);
	}
}
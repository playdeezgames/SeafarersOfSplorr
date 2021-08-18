#include "Application.Renderer.h"
#include "Application.Command.h"
#include "Application.MouseButtonUp.h"
#include "Application.OnEnter.h"
#include "Application.UIState.h"
#include "Game.Audio.Mux.h"
#include "Game.Messages.h"
#include "UIState.h"
#include "Visuals.SpriteGrid.h"
#include "Visuals.Texts.h"
namespace state::in_play::Message
{
	const std::string LAYOUT_NAME = "State.InPlay.Message";
	const std::string SPRITE_GRID = "Grid";
	const std::string TEXT_CAPTION = "Caption";
	const std::string FONT_DEFAULT = "default";

	static bool OnMouseButtonUp(const common::XY<int>& xy, MouseButton)
	{
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
		return true;
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Mux::Theme::MAIN);
		if (!visuals::Messages::HasMessage())
		{
			::application::UIState::Write(::UIState::IN_PLAY_NEXT);
			return;
		}
		visuals::SpriteGrid::Clear(LAYOUT_NAME, SPRITE_GRID);
		auto message = visuals::Messages::Read();
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_CAPTION, message.caption);
		for (auto& detail : message.details)
		{
			visuals::SpriteGrid::WriteText(LAYOUT_NAME, SPRITE_GRID, detail.position, FONT_DEFAULT, detail.text, detail.color, detail.alignment);
		}
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_MESSAGE, OnEnter);
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_MESSAGE, OnMouseButtonUp);
		::application::Command::SetHandler(::UIState::IN_PLAY_MESSAGE, ::application::UIState::GoTo(::UIState::IN_PLAY_NEXT));
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_MESSAGE, LAYOUT_NAME);
	}
}

#include "Application.Renderer.h"
#include "Application.Command.h"
#include "Application.Update.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Application.OnEnter.h"
#include "Common.Audio.h"
#include "Game.Audio.Mux.h"
#include "Game.Avatar.h"
#include "Visuals.Texts.h"
#include <format>
#include "Visuals.Areas.h"
#include "Visuals.Images.h"
#include "Common.Utility.h"
namespace state::in_play::ChangeHeading
{
	const std::string LAYOUT_NAME = "State.InPlay.ChangeHeading";
	const std::string TEXT_CURRENT_HEADING = "CurrentHeading";
	const std::string TEXT_NEW_HEADING = "NewHeading";
	const std::string AREA_HELM = "Helm";
	const std::string IMAGE_NEW_HEADING = "NewHeading";
	const std::string IMAGE_CURRENT_HEADING = "CurrentHeading";
	static double newHeading = 0.0;

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{::Command::BACK, ::application::UIState::GoTo(::UIState::IN_PLAY_AT_SEA) },
		{::Command::RED, ::application::UIState::GoTo(::UIState::IN_PLAY_AT_SEA) }
	};

	static void UpdateHeadings()
	{
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_CURRENT_HEADING, std::format("Current: {:.2f}", game::Avatar::GetHeading()));
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_NEW_HEADING, std::format("New: {:.2f}", newHeading));
		visuals::Images::SetAngle(LAYOUT_NAME, IMAGE_NEW_HEADING, newHeading);
		visuals::Images::SetAngle(LAYOUT_NAME, IMAGE_CURRENT_HEADING, game::Avatar::GetHeading());
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Mux::Theme::MAIN);
		newHeading = game::Avatar::GetHeading();
		UpdateHeadings();
	}

	static void HandleHelmMouseMotion(const common::XY<int>& location)
	{
		auto area = visuals::Areas::Get(LAYOUT_NAME, AREA_HELM);
		double x = (double)location.GetX() - (double)area.size.GetX()/2.0;
		double y = (double)location.GetY() - (double)area.size.GetY()/2.0;
		newHeading = (x==0.0 && y==0.0) ? (0.0) : (common::Utility::ToDegrees(atan2(x, -y)));
		UpdateHeadings();
	}

	static void OnMouseMotionInArea(const std::string& area, const common::XY<int>& location)
	{
		if (area == AREA_HELM)
		{
			HandleHelmMouseMotion(location);
			return;
		}
	}

	static bool HandleHelmButtonUp()
	{
		game::Avatar::SetHeading(newHeading);
		UpdateHeadings();
		return true;
	}

	static bool OnMouseButtonUpInArea(const std::string& area)
	{
		if (area == AREA_HELM)
		{
			return HandleHelmButtonUp();
		}
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_CHANGE_HEADING, OnEnter);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_CHANGE_HEADING, LAYOUT_NAME);
		::application::Command::SetHandlers(::UIState::IN_PLAY_CHANGE_HEADING, commandHandlers);
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_CHANGE_HEADING, visuals::Areas::HandleMouseMotion(LAYOUT_NAME, OnMouseMotionInArea));
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_CHANGE_HEADING, visuals::Areas::HandleMouseButtonUp(LAYOUT_NAME, OnMouseButtonUpInArea));
	}
}

#include "Application.Renderer.h"
#include "Application.UIState.h"
#include "Common.Application.h"
#include <map>
#include "Visuals.Layouts.h"
namespace application::Renderer
{
	static std::map<::UIState, std::string> renderLayouts;

	void SetRenderLayout(const ::UIState& state, const std::string& layoutName)
	{
		renderLayouts[state] = layoutName;
	}

}
namespace common::Application
{
	void Render(const std::shared_ptr<Renderer>& renderer)
	{
		auto layoutName = ::application::Renderer::renderLayouts.find(::application::UIState::Read());
		if (layoutName != ::application::Renderer::renderLayouts.end())
		{
			visuals::Layouts::Draw(renderer, layoutName->second);
		}
	}
}

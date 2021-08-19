#include "Application.Renderer.h"
#include "Application.UIState.h"
#include "Application.Engine.h"
#include <map>
namespace application::Renderer
{
	static std::map<int, std::string> renderLayouts;

	void SetRenderLayout(int state, const std::string& layoutName)
	{
		renderLayouts[state] = layoutName;
	}
}
namespace application::Engine
{
	void RenderLayout(const std::shared_ptr<application::Engine::Renderer>&, const std::string&);
	void Render(const std::shared_ptr<application::Engine::Renderer>& renderer)
	{
		auto layoutName = ::application::Renderer::renderLayouts.find(::application::UIState::Read());
		if (layoutName != ::application::Renderer::renderLayouts.end())
		{
			RenderLayout(renderer, layoutName->second);
		}
	}
}

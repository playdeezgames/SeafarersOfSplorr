#pragma once
#include <string>
namespace application::Renderer
{
	void SetRenderLayout(int, const std::string&);
	template<typename TState>
	void SetRenderLayout(const TState& state, const std::string& layoutName)
	{
		SetRenderLayout((int)state, layoutName);
	}
}

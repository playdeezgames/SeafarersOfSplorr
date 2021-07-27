#include "Common.Application.h"
#include "Common.Data.Properties.h"
#include "json.hpp"
#include <SDL.h>
#include "Visuals.Layouts.h"
namespace visuals::Sublayout
{
	struct InternalSublayout
	{
		std::string name;
	};

	static std::vector<InternalSublayout> internalSublayouts;

	static void DrawInternalSublayout(const std::shared_ptr<common::Application::Renderer>& renderer, size_t index)
	{
		auto& sublayout = internalSublayouts[index];
		visuals::Layouts::Draw(renderer, sublayout.name);
	}

	std::function<void(const std::shared_ptr<common::Application::Renderer>&)> Internalize(const std::string& layoutName, const nlohmann::json& model)
	{
		size_t index = internalSublayouts.size();
		internalSublayouts.push_back(
			{ 
				model[common::data::Properties::NAME]
			});
		return [index](const std::shared_ptr<common::Application::Renderer>& renderer) 
		{
			DrawInternalSublayout(renderer, index);
		};
	}
}
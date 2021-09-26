#include <Common.XY.h>
#include "Visuals.Buttons.h"
#include "Visuals.Data.Properties.h"
#include "Visuals.Fonts.h"
#include "Visuals.HorizontalAlignment.h"
namespace visuals
{
	struct InternalButton
	{
		std::string font;
		std::string text;
		std::string hiliteColor;
		std::string normalColor;
		common::XY<int> xy;
		HorizontalAlignment alignment;
		bool dropShadow;
		std::string dropShadowColor;
		common::XY<int> dropShadowXY;
		std::string layoutName;
	};

	static std::vector<InternalButton> internalButtons;
	static std::map<std::string, std::map<std::string, size_t>> buttonTable;
	static std::map<std::string, size_t> hoverButtons;

	static void DrawInternalButton(const std::shared_ptr<application::Engine::Renderer>& renderer, size_t buttonIndex)
	{
		auto& button = internalButtons[buttonIndex];
		bool hilite = hoverButtons.contains(button.layoutName) && hoverButtons[button.layoutName] == buttonIndex;
		if (button.dropShadow)
		{
			visuals::Fonts::WriteText(
				button.font,
				renderer,
				button.xy + button.dropShadowXY,
				button.text,
				button.dropShadowColor,
				button.alignment);
		}
		visuals::Fonts::WriteText(
			button.font,
			renderer,
			button.xy,
			button.text,
			(hilite) ? (button.hiliteColor) : (button.normalColor),
			button.alignment);
	}

	DrawerFunction Buttons::Internalize(const std::string& layoutName, const nlohmann::json& model)
	{
		size_t buttonIndex = internalButtons.size();
		internalButtons.push_back(
			{
				model[visuals::data::Properties::FONT],
				model[visuals::data::Properties::TEXT],
				model[visuals::data::Properties::COLORS][visuals::data::Properties::HILITE],
				model[visuals::data::Properties::COLORS][visuals::data::Properties::NORMAL],
				common::XY<int>(
					model[visuals::data::Properties::X],
					model[visuals::data::Properties::Y]),
				(HorizontalAlignment)(int)model[visuals::data::Properties::HORIZONTAL_ALIGNMENT],
				(bool)model[visuals::data::Properties::DROP_SHADOW],
				model[visuals::data::Properties::DROP_SHADOW_COLOR],
				common::XY<int>(
					model[visuals::data::Properties::DROP_SHADOW_X],
					model[visuals::data::Properties::DROP_SHADOW_Y]),
				layoutName
			});
		if (model.count(visuals::data::Properties::TEXT_ID) > 0)
		{
			buttonTable[layoutName][model[visuals::data::Properties::TEXT_ID]] = buttonIndex;
		}
		return [buttonIndex](const std::shared_ptr<application::Engine::Renderer>& renderer)
		{
			DrawInternalButton(renderer, buttonIndex);
		};
	}

	void Buttons::SetHoverButton(const std::string& layoutName, const std::string& buttonName)
	{
		hoverButtons[layoutName] = buttonTable[layoutName][buttonName];
	}

	void Buttons::ClearHoverButton(const std::string& layoutName)
	{
		if (hoverButtons.contains(layoutName))
		{
			hoverButtons.erase(layoutName);
		}
	}

}
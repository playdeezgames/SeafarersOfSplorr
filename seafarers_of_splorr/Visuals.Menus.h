#pragma once
#include "Common.Utility.h"
#include <functional>
#include <optional>
#include <string>
#include "json.hpp"
#include "Visuals.DrawerFunction.h"
namespace visuals::Menu
{
	DrawerFunction Internalize(const std::string&, const nlohmann::json&);
}
namespace visuals::Menus
{

	std::optional<int> ReadIndex(const std::string&, const std::string&);
	void WriteIndex(const std::string&, const std::string&, int);
	void WriteMenuItemId(const std::string&, const std::string&, const std::string&);
	size_t GetCount(const std::string&, const std::string&);
	std::function<void()> NavigateNext(const std::string&, const std::string&);
	std::function<void()> NavigatePrevious(const std::string&, const std::string&);

	template<typename TEnum>
	std::function<void()> DoActivateItem(const std::string& layoutName, const std::string& menuId, const std::map<TEnum, std::function<void()>>& actions)
	{
		return [actions, layoutName, menuId]()
		{
			common::Utility::Dispatch(actions, (TEnum)visuals::Menus::ReadIndex(layoutName, menuId).value());
		};
	}
}
namespace visuals::MenuItems
{
	void SetText(const std::string&, const std::string&, const std::string&);
	void SetEnabled(const std::string&, const std::string&, bool);
}
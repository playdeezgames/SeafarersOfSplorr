#include <format>
#include "Menu.h"
void Menu::Clear()
{
	defaultAction = std::nullopt;
	refresh = std::nullopt;
	actions.clear();
	page = 0;
}

void Menu::SetDefaultAction(const std::optional<MenuAction>& action)
{
	defaultAction = action;
}

void Menu::AddAction(const MenuAction& action)
{
	actions.push_back(action);
}

bool Menu::IsFirstPage() const
{
	return page == 0;
}

bool Menu::IsLastPage() const
{
	auto pageCount = GetPageCount(actions.size());
	return (pageCount == 1) || (page == (pageCount - 1));
}

size_t Menu::GetPageItemCount() const
{
	return (size_t)(7 + ((IsFirstPage()) ? (1) : (0)) + ((IsLastPage()) ? (1) : (0)));
}

bool Menu::HasPages() const
{
	return !actions.empty();
}

int Menu::GetStartingNumber() const
{
	return (IsFirstPage()) ? (1) : (2);
}


std::list<std::string> Menu::GetCurrentPage() const
{
	std::list<std::string> result;
	if (HasPages())
	{
		auto iter = actions.begin() + GetPageIndex(page);
		size_t pageItemCount = GetPageItemCount();
		if (!IsFirstPage())
		{
			result.push_back("1) Previous Page");
		}
		int number = GetStartingNumber();
		while (iter != actions.end() && pageItemCount > 0)
		{
			result.push_back(std::format("{}) {}", number++, iter->caption));
			iter++;
			pageItemCount--;
		}
		if (!IsLastPage())
		{
			result.push_back("9) Next Page");
		}
	}
	if (defaultAction)
	{
		result.push_back(std::format("0) {}", defaultAction->caption));
	}
	return result;
}

void Menu::NextPage()
{
	if (HasPages() && !IsLastPage())
	{
		page++;
		refresh.value()();
	}
}

void Menu::PreviousPage()
{
	if (HasPages() && !IsFirstPage())
	{
		page--;
		refresh.value()();
	}
}

size_t Menu::GetPageCount(size_t itemCount)
{
	return
		(itemCount <= 9) ? (1) :
		(itemCount <= 16) ? (2) :
		((itemCount + 4) / 7);
}

size_t Menu::GetPageIndex(size_t page)
{
	return
		(page == 0) ? (0) :
		(page == 1) ? (8) :
		(page * 7 + 3);
}

void Menu::SetRefresh(std::function<void()> r)
{
	refresh = r;
}


bool Menu::DoAction(size_t menuIndex)
{
	if (menuIndex > 9)
	{
		return false;
	}
	if (menuIndex == 0)
	{
		if (defaultAction)
		{
			defaultAction->action();
			return true;
		}
		return false;
	}
	if (!IsFirstPage() && menuIndex == 1)
	{
		PreviousPage();
		return true;
	}
	if (!IsLastPage() && menuIndex == 9)
	{
		NextPage();
		return true;
	}
	menuIndex -= GetStartingNumber();
	menuIndex += GetPageIndex(page);
	if (menuIndex < actions.size())
	{
		actions[menuIndex].action();
		return true;
	}
	return false;
}

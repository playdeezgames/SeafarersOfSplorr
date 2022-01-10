#pragma once
#include <functional>
#include <optional>
#include <string>
#include <vector>
struct MenuAction
{
	std::string caption;
	std::function<void()> action;
};
struct Menu
{
	void Clear();
	void SetDefaultAction(const std::optional<MenuAction>&);
	void SetRefresh(std::function<void()>);
	void AddAction(const MenuAction&);
	std::list<std::string> GetCurrentPage() const;
	bool DoAction(size_t menuIndex);
private:
	void NextPage();
	void PreviousPage();
	static size_t GetPageCount(size_t itemCount);
	static size_t GetPageIndex(size_t page);
	bool IsFirstPage() const;
	bool IsLastPage() const;
	size_t GetPageItemCount() const;
	bool HasPages() const;
	int GetStartingNumber() const;
	std::optional<std::function<void()>> refresh;
	std::optional<MenuAction> defaultAction;
	std::vector<MenuAction> actions;
	size_t page;
};
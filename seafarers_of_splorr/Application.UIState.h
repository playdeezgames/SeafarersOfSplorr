#pragma once
#include <optional>
#include <string>
#include <functional>
namespace application::UIState
{
	void Write(int);
	int Read();
	std::function<void()> GoTo(int);

	void Push(int);
	void Pop();
	std::function<void()> PushTo(int);
	std::function<void()> PopFrom();

	template<typename TState>
	void Write(const TState& state)
	{
		Write((int)state);
	}
	template<typename TState>
	std::function<void()> GoTo(const TState& state)
	{
		return GoTo((int)state);
	}
	template<typename TState>
	void Push(const TState& state)
	{
		Push((int)state);
	}
	template<typename TState>
	std::function<void()> PushTo(const TState& state)
	{
		return PushTo((int)state);
	}
}
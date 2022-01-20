#pragma once
#include <optional>
#include <string>
#include <functional>
namespace application
{
	struct UIState
	{
		static void SetFinalState(int);
		static int GetFinalState();

		static void Write(int);
		static int Read();
		static std::function<void()> GoTo(int);
		static std::function<void()> DoGoTo(std::function<int()>);

		static void Push(int);
		static void Pop();
		static std::function<void()> PushTo(int);
		static std::function<void()> DoPushTo(std::function<int()>);
		static bool CanPop();

	};
}
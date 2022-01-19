#pragma once
#include <functional>
#include <optional>
namespace state
{
	struct Registrar
	{
		Registrar() = delete;
		static void Register(std::optional<int>& stateId, std::function<void(int)> starter);
	private:
		static int NextStateId() { return stateId++; }
		static int stateId;
	};
}

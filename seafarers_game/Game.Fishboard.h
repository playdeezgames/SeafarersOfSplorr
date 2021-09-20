#pragma once
#include <Common.XY.h>
#include "Game.Fish.h"
#include <optional>
namespace game
{
	struct Fishboard
	{
		bool revealed;
		std::optional<Fish> fish;

		static const size_t COLUMNS;
		static const size_t ROWS;
		static void Generate();
		static void Reveal(const common::XY<int>&);
		static std::optional<Fishboard> Read(const common::XY<int>&);
	};
}

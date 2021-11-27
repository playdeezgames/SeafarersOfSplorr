#pragma once
#include <Common.XY.h>
#include <list>
#include <optional>
namespace data::game::island
{
	struct Known
	{
		static void Write(int);
		static bool Read(int);
		static void Clear();
		static std::list<common::XY<double>> All();
	};
}
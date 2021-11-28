#pragma once
#include <Common.XY.h>
#include <list>
#include <optional>
namespace data::game
{
	struct Fishery
	{
		int fisheryId;
		int fishType;
		common::XY<double> location;
		common::XY<double> movement;
		double radius;
		int stock;
		int depletion;

		static void Clear();
		static void Add(const Fishery&);
		static std::list<Fishery> All();
		static void Write(const Fishery&);
		static std::optional<Fishery> Read(int);
	};
}

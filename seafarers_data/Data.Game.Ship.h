#pragma once
#include <Common.XY.h>
#include <list>
#include <optional>
#include <string>
namespace data::game
{
	struct Ship
	{
		Ship() = delete;
		static void Clear();

		static void Initialize();
		static int Create(
			int shipType, 
			const std::string& name, 
			const common::XY<double>& location, 
			double heading, 
			double speed);
		static std::optional<int> GetShipType(int shipId);
		static std::optional<std::string> GetName(int shipId);
		static std::optional<common::XY<double>> GetLocation(int shipId);
		static std::optional<double> GetHeading(int shipId);
		static std::optional<double> GetSpeed(int shipId);
		static void SetName(int shipId, const std::string& name);
		static void SetLocation(int shipId, const common::XY<double>& location);
		static void SetHeading(int shipId, double heading);
		static void SetSpeed(int shipId, double speed);

		static std::list<int> All();
	};
}

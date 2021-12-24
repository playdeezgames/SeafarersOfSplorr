#pragma once
#include <Common.XY.h>
#include <list>
#include <optional>
#include <string>
namespace data::game
{
	struct Ship
	{
		int shipId;
		int shipType;
		std::string name;
		common::XY<double> location;
		double heading;
		double speed;

		static int Write(const Ship&);
		static void Clear();

		static void Initialize();
		static int Create(int, const std::string&, const common::XY<double>&, double, double);
		static std::optional<int> GetShipType(int);
		static std::optional<std::string> GetName(int);
		static std::optional<common::XY<double>> GetLocation(int);
		static std::optional<double> GetHeading(int);
		static std::optional<double> GetSpeed(int);
		static void SetName(int, const std::string&);
		static void SetLocation(int, const common::XY<double>&);
		static void SetHeading(int, double);
		static void SetSpeed(int, double);

		static std::list<int> All();
	};
}

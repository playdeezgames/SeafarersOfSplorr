#pragma once
namespace data::game::avatar
{
	struct Ship//TODO: this will manage the relationships between avatar and ship
	{
		static void Write(int, int, int);
		static int ReadShipType(int);
		static int ReadShipId(int);
	};
}
#pragma once
namespace data::game::avatar
{
	struct Ship//TODO: this will manage the relationships between avatar and ship
	{
		static void Write(int, int);
		static int Read(int);
		//static void Write(int);
		//static int Read();
	};
}
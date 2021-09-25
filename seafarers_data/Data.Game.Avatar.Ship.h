#pragma once
namespace data::game::avatar
{
	struct Ship
	{
		static void Write(int, int);
		static int Read(int);
		static void Write(int);
		static int Read();
	};
}
#pragma once
#include <string>
namespace state::scratch_pad
{
	struct HeadForMark
	{
		HeadForMark() = delete;
		static void SetMark(const std::string& mark);
		static const std::string& GetMark();
	private:
		static std::string headForMark;
	};
}

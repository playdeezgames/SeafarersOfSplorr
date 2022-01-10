#pragma once
#include <string>
namespace state::scratch_pad
{
	struct ShipAdjective
	{
		ShipAdjective() = delete;
		static void SetAdjective(const std::string& adjective);
		static const std::string& GetAdjective();
	private:
		static std::string shipAdjective;
	};
}

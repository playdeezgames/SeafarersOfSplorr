#pragma once
#include <list>
#include <optional>
#include <string>
namespace data::game
{
	struct Demigod
	{
		typedef int demigodid_t;
		typedef std::string name_t;
		typedef size_t patronweight_t;
		static void Initialize();
		static void Clear();
		static std::list<demigodid_t> All();
		static int Create(const name_t&, patronweight_t);
		static std::optional<name_t> ReadName(demigodid_t);
		static std::optional<patronweight_t> ReadPatronWeight(demigodid_t);
	};
}

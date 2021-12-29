#pragma once
#include <list>
#include <optional>
#include <string>
namespace data::game
{
	struct Character 
	{
		typedef int characterid_t;
		typedef std::string name_t;
		typedef int state_t;
		typedef int islandid_t;
		typedef int tribeid_t;
		static int Create(state_t, const name_t&, islandid_t, tribeid_t);
		static void WriteState(characterid_t, state_t);
		static std::optional<state_t> ReadState(characterid_t);
		static std::optional<name_t> ReadName(characterid_t);
		static std::optional<islandid_t> ReadOriginIslandId(characterid_t);
		static std::optional<tribeid_t> ReadTribeId(characterid_t);
		static void Initialize();
		static std::list<characterid_t> All();
	};
}

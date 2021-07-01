#include "Game.Avatar.h"
#include "Game.World.h"
#include "Game.Avatar.Statistics.h"
#include <vector>
#include "Common.Audio.h"
#include "Application.UIState.h"
#include "Application.Sounds.h"
#include "Common.RNG.h"
#include "Game.Creatures.h"
#include "Game.World.h"
#include "json.hpp"
#include "Game.Data.Properties.h"
#include "Game.h"
#include "Data.Stores.h"
#include "Game.World.Items.h"
#include "Game.Avatar.Items.h"
#include "Game.World.Borders.h"
#include "Game.Achievements.h"
namespace game::Avatar
{
	static bool initialized = false;

	static Descriptor descriptor;

	static void Initialize()
	{
		if (!initialized)
		{
			auto& store = ::data::Stores::GetStore(::data::Store::AVATAR);
			descriptor.poopItemId = store[data::Properties::POOP];
			descriptor.hungerRate = store[game::data::Properties::HUNGER_RATE];
			descriptor.sobrietyRate = store[game::data::Properties::SOBRIETY_RATE];
			descriptor.mapFragmentWidth = store[data::Properties::MAP_FRAGMENT_WIDTH];
			descriptor.mapFragmentHeight = store[data::Properties::MAP_FRAGMENT_HEIGHT];
			descriptor.keyItemId = store[data::Properties::KEY];
			descriptor.mapFragmentItemId = store[data::Properties::MAP_FRAGMENT];
			descriptor.poopThreshold = store[data::Properties::POOP_THRESHOLD];
			descriptor.joolsItemId = store[data::Properties::JOOLS];
			descriptor.trousersItemId = store[data::Properties::TROUSERS];
			descriptor.soiledTrousersItemId = store[data::Properties::SOILED_TROUSERS];
			descriptor.wisdom = store[data::Properties::WISDOM];
			initialized = true;
		}
	}

	const Descriptor& GetDescriptor()
	{
		Initialize();
		return descriptor;
	}

	nlohmann::json& GetAvatar()
	{
		auto& data = game::GetData();
		if (data.count(game::data::Properties::AVATAR) == 0)
		{
			data[game::data::Properties::AVATAR] = nlohmann::json();
		}
		return data[game::data::Properties::AVATAR];
	}

	static void SetFacing(maze::Direction direction)
	{
		GetAvatar()[game::data::Properties::FACING] = (int)direction;
	}

	maze::Direction GetFacing()
	{
		return (maze::Direction)(int)GetAvatar()[game::data::Properties::FACING];
	}

	static void SetColumn(size_t column)
	{
		GetAvatar()[game::data::Properties::COLUMN] = column;
	}

	static void SetRow(size_t row)
	{
		GetAvatar()[game::data::Properties::ROW] = row;
	}

	void SetPosition(const common::XY<size_t>& position)
	{
		SetColumn(position.GetX());
		SetRow(position.GetY());
	}

	common::XY<size_t> GetPosition()
	{
		return { (size_t)GetAvatar()[game::data::Properties::COLUMN], (size_t)GetAvatar()[game::data::Properties::ROW] };
	}

	static void ActuallyTurnLeft()
	{
		SetFacing(
			(GetFacing() == maze::Direction::NORTH) ? (maze::Direction::WEST) :
			(GetFacing() == maze::Direction::EAST) ? (maze::Direction::NORTH) :
			(GetFacing() == maze::Direction::SOUTH) ? (maze::Direction::EAST) :
			(maze::Direction::SOUTH));
	}

	static void ActuallyTurnRight()
	{
		SetFacing(
			(GetFacing() == maze::Direction::NORTH) ? (maze::Direction::EAST) :
			(GetFacing() == maze::Direction::EAST) ? (maze::Direction::SOUTH) :
			(GetFacing() == maze::Direction::SOUTH) ? (maze::Direction::WEST) :
			(maze::Direction::NORTH));
	}

	std::optional<std::string> TurnLeft()
	{
		if (!game::avatar::Statistics::IsMinimum(game::avatar::Statistic::HEALTH))
		{
			if (game::avatar::Statistics::IsMinimum(game::avatar::Statistic::NAUSEA))
			{
				ActuallyTurnLeft();
				return application::Sounds::Read(application::UI::Sfx::WOOHOO);
			}
			else
			{
				ActuallyTurnRight();
				return application::Sounds::Read(application::UI::Sfx::WOOHOO);
			}
		}
		return std::nullopt;
	}

	std::optional<std::string> TurnRight()
	{
		if (!game::avatar::Statistics::IsMinimum(game::avatar::Statistic::HEALTH))
		{
			if (game::avatar::Statistics::IsMinimum(game::avatar::Statistic::NAUSEA))
			{
				ActuallyTurnRight();
				return application::Sounds::Read(application::UI::Sfx::WOOHOO);
			}
			else
			{
				ActuallyTurnLeft();
				return application::Sounds::Read(application::UI::Sfx::WOOHOO);
			}
		}
		return std::nullopt;
	}

	std::optional<std::string> Poop(bool accidental)
	{
		int itemId = GetDescriptor().poopItemId;
		game::avatar::Statistics::Write(game::avatar::Statistic::BOWEL, game::avatar::Statistics::Default(game::avatar::Statistic::BOWEL));
		if (accidental)
		{
			auto& descriptor = GetDescriptor();
			if (game::avatar::Items::Read(descriptor.trousersItemId) > 0)
			{
				game::avatar::Items::Remove(descriptor.trousersItemId, 1);
				game::avatar::Items::Add(descriptor.soiledTrousersItemId, 1);
			}
		}
		game::world::Items::Add(GetPosition(), itemId, 1);
		return application::Sounds::Read(application::UI::Sfx::WOOHOO);
	}

	static std::optional<std::string> HandleHunger()
	{
		int hungerRate = GetDescriptor().hungerRate;
		if (game::avatar::Statistics::IsMinimum(game::avatar::Statistic::HUNGER))
		{
			avatar::Statistics::SetCauseOfDeath("starvation(the REAL enemy)", std::nullopt, std::nullopt);
			avatar::Statistics::Decrease(avatar::Statistic::HEALTH, hungerRate);
			return application::Sounds::Read(application::UI::Sfx::WOOHOO);
		}
		else
		{
			avatar::Statistics::Decrease(avatar::Statistic::HUNGER, hungerRate);
			return std::nullopt;
		}
	}

	static std::optional<std::string> HandleBowel(std::optional<std::string> sfx)
	{
		if (game::avatar::Statistics::IsMaximum(game::avatar::Statistic::BOWEL))
		{
			return Poop(true);
		}
		return sfx;
	}

	static void HandleNausea()
	{
		int sobrietyRate = GetDescriptor().sobrietyRate;
		if (!game::avatar::Statistics::IsMinimum(game::avatar::Statistic::NAUSEA))
		{
			avatar::Statistics::Decrease(avatar::Statistic::NAUSEA, sobrietyRate);
		}
	}

	static void HandleSobriety()
	{
		int sobrietyRate = GetDescriptor().sobrietyRate;
		if (!game::avatar::Statistics::IsMinimum(game::avatar::Statistic::DRUNKENNESS))
		{
			avatar::Statistics::Decrease(avatar::Statistic::DRUNKENNESS, sobrietyRate);
		}
	}

	static void DoFourthWallAchievement()
	{
		auto wallBreaks = game::avatar::Statistics::Read(game::avatar::Statistic::WALL_BREAKS);
		wallBreaks = wallBreaks + 1;
		game::avatar::Statistics::Write(game::avatar::Statistic::WALL_BREAKS, wallBreaks);
	}

	static std::optional<std::string> BumpWall()
	{
		auto bumpResult = game::world::Borders::BumpAhead(GetPosition(), GetFacing());
		avatar::Statistics::SetCauseOfDeath("wall (aka Epic Stupidity)", std::nullopt, std::nullopt);
		avatar::Statistics::Decrease(avatar::Statistic::HEALTH, 1);


		switch (bumpResult)
		{
		case game::world::Borders::WallBumpResult::WALL_FALLS:
			DoFourthWallAchievement();
			game::world::Borders::SetBorderAhead(GetPosition(), GetFacing(), game::world::Border::BROKEN_WALL);
			return application::Sounds::Read(application::UI::Sfx::WOOHOO);//TODO: update for better wall fall sound!
		default:
			return application::Sounds::Read(application::UI::Sfx::WOOHOO);
		}
	}

	static std::optional<std::string> AttemptUnlock()
	{
		if (game::avatar::Statistics::Read(game::avatar::Statistic::KEYS) > 0)
		{
			game::avatar::Statistics::Decrease(game::avatar::Statistic::KEYS, 1);
			game::world::Borders::SetBorderAhead(game::Avatar::GetPosition(), game::Avatar::GetFacing(), game::world::Border::DOOR);
			return application::Sounds::Read(application::UI::Sfx::WOOHOO);
		}
		return application::Sounds::Read(application::UI::Sfx::WOOHOO);
	}

	static std::optional<std::string> StepAhead()
	{
		std::optional<std::string> result = std::nullopt;
		switch (GetFacing())
		{
		case maze::Direction::NORTH:
			SetRow(GetPosition().GetY() - 1);
			break;
		case maze::Direction::EAST:
			SetColumn(GetPosition().GetX() + 1);
			break;
		case maze::Direction::SOUTH:
			SetRow(GetPosition().GetY() + 1);
			break;
		case maze::Direction::WEST:
			SetColumn(GetPosition().GetX() - 1);
			break;
		}
		game::World::SetExplored(GetPosition());
		result = HandleHunger();
		HandleNausea();
		HandleSobriety();
		result = HandleBowel(result);
		game::Statistics::Increment(game::Statistic::MOVES_MADE);
		return result;
	}

	const std::map<game::world::Border, std::function<std::optional<std::string>()>> moveAheadTable =
	{
		{game::world::Border::DOOR, StepAhead},
		{game::world::Border::BROKEN_WALL, StepAhead},
		{game::world::Border::LOCK, AttemptUnlock},
		{game::world::Border::BREAKABLE_WALL, BumpWall},
		{game::world::Border::UNBREAKABLE_WALL, BumpWall}
	};

	std::optional<std::string> MoveAhead()
	{
		std::optional<std::string> result = std::nullopt;
		if (!game::avatar::Statistics::IsMinimum(game::avatar::Statistic::HEALTH))
		{
			auto border = game::world::Borders::GetBorderAhead(game::Avatar::GetPosition(), game::Avatar::GetFacing());
			result = moveAheadTable.find(border)->second();
		}
		common::audio::Sfx::Play(application::UIState::EnterGame());
		return result;
	}

	std::optional<std::string> MoveBack()
	{
		TurnRight();
		TurnRight();
		auto result = MoveAhead();
		TurnRight();
		TurnRight();
		return result;
	}

	bool CanPoop()
	{
		int poopThreshold = game::Avatar::GetDescriptor().poopThreshold;
		return (game::avatar::Statistics::Read(game::avatar::Statistic::BOWEL) >= poopThreshold);
	}

	void Reset()
	{
		auto worldSize = game::World::GetSize();
		SetFacing((maze::Direction)common::RNG::FromRange(0, (int)maze::Directions::All().size()));
		do
		{
			SetColumn((size_t)common::RNG::FromRange(0, (int)worldSize.GetX()));
			SetRow((size_t)common::RNG::FromRange(0, (int)worldSize.GetY()));
		} while (!game::world::Borders::CanSpawnAvatar(GetPosition()));
		game::World::SetExplored(GetPosition());
	}

	static bool teleportFlag = false;

	void SetTeleportFlag()
	{
		teleportFlag = true;
	}

	void ClearTeleportFlag()
	{
		teleportFlag = false;
	}

	bool HasTeleportFlag()
	{
		return teleportFlag;
	}
}

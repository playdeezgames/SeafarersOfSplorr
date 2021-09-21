#include <Common.XY.h>
#include <Game.Colors.h>
#include <Game.Fishboard.h>
#include <Visuals.Colors.h>
#include <Visuals.Data.Properties.h>
#include <Visuals.Sprites.h>
#include "Visuals.Fishboard.h"
namespace visuals
{
	static const std::string FISHBOARD_ID = "fishboard-id";
	static const int CELL_WIDTH = 64;
	static const int CELL_HEIGHT = 64;
	static const int CELL_COLUMNS = 5;
	static const int CELL_ROWS = 5;
	static const int BOARD_WIDTH = CELL_WIDTH * CELL_COLUMNS;
	static const int BOARD_HEIGHT = CELL_HEIGHT * CELL_ROWS;
	static const std::string SPRITE_HIDDEN = "FishboardHidden";
	static const std::string SPRITE_SELECT = "FishboardSelect";
	static const std::string SPRITE_FISH = "FishboardFishRed";

	struct InternalFishboard
	{
		common::XY<int> xy;
		common::XY<int> hoverXy;
	};

	static std::vector<InternalFishboard> fishboards;
	static std::map<std::string, std::map<std::string, size_t>> fishboardTable;

	static common::XY<int> Plot(const InternalFishboard& fishboard, common::XY<int> location)
	{
		return { fishboard.xy.GetX() + location.GetX() * CELL_WIDTH, fishboard.xy.GetY() + location.GetY() * CELL_HEIGHT };
	}

	static void DrawSprite(const std::shared_ptr<application::Engine::Renderer>& renderer, const InternalFishboard& fishboard, const common::XY<int>& location, const std::string& spriteName)
	{
		auto plot = Plot(fishboard, location);
		visuals::Sprites::Draw(spriteName, renderer, plot, Colors::Read(game::Colors::WHITE));
	}


	static void DrawCells(const std::shared_ptr<application::Engine::Renderer>& renderer, const InternalFishboard& fishboard)
	{
		for (int column = 0; column < CELL_COLUMNS; ++column)
		{
			for (int row = 0; row < CELL_ROWS; ++row)
			{
				auto cell = game::Fishboard::Read({ column, row }).value();
				if (cell.revealed)
				{
					if (cell.fish)
					{
						DrawSprite(renderer, fishboard, { column, row }, SPRITE_FISH);
					}
				}
				else
				{
					DrawSprite(renderer, fishboard, { column, row }, SPRITE_HIDDEN);
				}
			}
		}
	}

	static void DrawCursor(const std::shared_ptr<application::Engine::Renderer>& renderer, const InternalFishboard& fishboard)
	{
		DrawSprite(renderer, fishboard, fishboard.hoverXy, SPRITE_SELECT);
	}


	static void DrawInternalFishboard(const std::shared_ptr<application::Engine::Renderer>& renderer, size_t fishboardIndex)
	{
		const auto& fishboard = fishboards[fishboardIndex];
		DrawCells(renderer, fishboard);
		DrawCursor(renderer, fishboard);
	}

	std::function<void(const std::shared_ptr<application::Engine::Renderer>&)> Fishboard::Internalize(const std::string& layoutName, const nlohmann::json& model)
	{
		auto fishboardIndex = fishboards.size();
		fishboards.push_back({
			common::XY<int>(model[visuals::data::Properties::X], model[visuals::data::Properties::Y]),
			{0,0}//hover cell
			});
		if (model.count(FISHBOARD_ID) > 0)
		{
			fishboardTable[layoutName][model[FISHBOARD_ID]] = fishboardIndex;
		}

		return [fishboardIndex](const std::shared_ptr<application::Engine::Renderer>& renderer)
		{
			DrawInternalFishboard(renderer, fishboardIndex);
		};
	}

	void Fishboard::HandleMouseMotion(const std::string& layoutName, const std::string& fishboardName, const common::XY<int>& position)
	{
		auto& fishboard = fishboards[fishboardTable[layoutName][fishboardName]];
		if (position.GetX() >= 0 && 
			position.GetY() >= 0 && 
			position.GetX() < BOARD_WIDTH && 
			position.GetY() < BOARD_HEIGHT)
		{
			fishboard.hoverXy = { position.GetX() / CELL_WIDTH, position.GetY() / CELL_HEIGHT };
		}
	}

	common::XY<int> Fishboard::ReadCursor(const std::string& layoutName, const std::string& fishboardName)
	{
		return fishboards[fishboardTable[layoutName][fishboardName]].hoverXy;
	}
}

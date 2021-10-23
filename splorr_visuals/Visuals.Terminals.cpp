#include <algorithm>
#include <Data.JSON.h>
#include "Visuals.Data.Properties.h"
#include "Visuals.Fonts.h"
#include "Visuals.Layouts.h"
#include "Visuals.Terminals.h"
namespace visuals
{
	struct InternalTerminalCell
	{
		std::string backgroundColor;
		std::string foregroundColor;
		char character;
	};

	struct InternalTerminal
	{
		std::string font;
		common::XY<int> xy;
		common::XY<int> terminalSize;
		common::XY<int> cellSize;
		std::string emptySprite;
		std::vector<InternalTerminalCell> cells;
		size_t readIndex;
		size_t writeIndex;
	};

	static std::vector<InternalTerminal> internalTerminals;
	static std::map<std::string, std::map<std::string, size_t>> terminalTable;

	static void DrawInternalTerminal(
		const std::shared_ptr<application::Engine::Renderer>& renderer, 
		size_t terminalIndex)
	{
		auto& terminal = internalTerminals[terminalIndex];
	}

	static const std::string PROPERTY_COLUMNS = "columns";
	static const std::string PROPERTY_ROWS = "rows";
	static const std::string PROPERTY_CELL_WIDTH = "cell-width";
	static const std::string PROPERTY_CELL_HEIGHT = "cell-height";
	static const std::string PROPERTY_CELL_BACKGROUND_SPRITE = "cell-background-sprite";

	std::function<void(const std::shared_ptr<application::Engine::Renderer>&)> Terminals::Internalize(
		const std::string& layoutName, 
		const nlohmann::json& model)
	{
		size_t terminalIndex = internalTerminals.size();
		InternalTerminal terminal = 
			{
				model[visuals::data::Properties::FONT],
				common::XY<int>(
					model[visuals::data::Properties::X],
					model[visuals::data::Properties::Y]),
				common::XY<int>(
					model[PROPERTY_COLUMNS],
					model[PROPERTY_ROWS]),
				common::XY<int>(
					model[PROPERTY_CELL_WIDTH],
					model[PROPERTY_CELL_HEIGHT]),
				model[PROPERTY_CELL_BACKGROUND_SPRITE],
				{},
				0,
				0
			};
		const size_t cellCount =
			terminal.terminalSize.GetX() *
			terminal.terminalSize.GetY();
		terminal.cells.reserve(cellCount);
		while (terminal.cells.size() < cellCount)
		{
			terminal.cells.push_back(
				InternalTerminalCell());
		}
		internalTerminals.push_back(terminal);
		if (model.count(visuals::data::Properties::TERMINAL_ID) > 0)
		{
			terminalTable[layoutName][model[visuals::data::Properties::TERMINAL_ID]] = terminalIndex;
		}
		return [terminalIndex](const std::shared_ptr<application::Engine::Renderer>& renderer)
		{
			DrawInternalTerminal(renderer, terminalIndex);
		};
	}
}
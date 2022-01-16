#include <algorithm>
#include <Data.JSON.h>
#include "Visuals.Data.Properties.h"
#include "Visuals.Fonts.h"
#include "Visuals.Layouts.h"
#include "Visuals.Sprites.h"
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
		std::string backgroundColor;
		std::string foregroundColor;
		std::vector<InternalTerminalCell> cells;
		size_t readIndex;
		size_t writeIndex;
	};

	static std::vector<InternalTerminal> internalTerminals;
	static std::map<std::string, std::map<std::string, size_t>> terminalTable;
	static const char SOLID_CHARACTER = (char)0xdb;

	static void DrawInternalTerminal(
		const std::shared_ptr<application::Engine::Renderer>& renderer,
		size_t terminalIndex)
	{
		auto& terminal = internalTerminals[terminalIndex];
		auto cellIter = terminal.cells.begin() + terminal.readIndex;
		for (size_t row = 0; row < terminal.terminalSize.GetY(); ++row)
		{
			for (size_t column = 0; column < terminal.terminalSize.GetX(); ++column)
			{
				common::XY<int> plot = { terminal.xy.GetX() + (int)column * terminal.cellSize.GetX(), terminal.xy.GetY() + (int)row * terminal.cellSize.GetY() };
				Fonts::WriteGlyph(terminal.font, renderer, plot, SOLID_CHARACTER, cellIter->backgroundColor);
				Fonts::WriteGlyph(terminal.font, renderer, plot, cellIter->character, cellIter->foregroundColor);
				cellIter++;
				if (cellIter == terminal.cells.end())
				{
					cellIter = terminal.cells.begin();
				}
			}
		}
	}

	static const std::string PROPERTY_BACKGROUND_COLOR = "background-color";
	static const std::string PROPERTY_FOREGROUND_COLOR = "foreground-color";
	static const std::string PROPERTY_COLUMNS = "columns";
	static const std::string PROPERTY_ROWS = "rows";
	static const std::string PROPERTY_CELL_WIDTH = "cell-width";
	static const std::string PROPERTY_CELL_HEIGHT = "cell-height";

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
			model[PROPERTY_BACKGROUND_COLOR],
			model[PROPERTY_FOREGROUND_COLOR],
			{},
			0,
			0
		};
		const size_t cellCount =
			(size_t)terminal.terminalSize.GetX() *
			(size_t)terminal.terminalSize.GetY();
		terminal.cells.reserve(cellCount);
		while (terminal.cells.size() < cellCount)
		{
			terminal.cells.push_back(
				InternalTerminalCell({ terminal.backgroundColor, terminal.foregroundColor, ' ' }));
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

	static void WriteGlyph(InternalTerminal& terminal, char glyph)
	{
		auto writeIter = terminal.cells.begin() + terminal.writeIndex;
		writeIter->character = glyph;
		writeIter->backgroundColor = terminal.backgroundColor;
		writeIter->foregroundColor = terminal.foregroundColor;
		writeIter++;
		terminal.writeIndex++;
		if (writeIter == terminal.cells.end())
		{
			writeIter = terminal.cells.begin();
			terminal.writeIndex = 0;
		}
		if (terminal.writeIndex == terminal.readIndex)
		{
			writeIter = terminal.cells.begin() + terminal.readIndex;
			for (size_t count = 0; count < terminal.terminalSize.GetX(); ++count)
			{
				writeIter->character = ' ';
				writeIter->backgroundColor = terminal.backgroundColor;
				writeIter->foregroundColor = terminal.foregroundColor;
				writeIter++;
			}
			terminal.readIndex += terminal.terminalSize.GetX();
			terminal.readIndex %= terminal.cells.size();
		}
	}

	void Terminals::WriteText(const std::string& layoutName, const std::string& terminalName, const std::string& text)
	{
		auto& terminal = internalTerminals[terminalTable[layoutName][terminalName]];
		for (auto ch : text)
		{
			WriteGlyph(terminal, ch);
		}
	}

	void Terminals::WriteLine(const std::string& layoutName, const std::string& terminalName, const std::string& text)
	{
		auto& terminal = internalTerminals[terminalTable[layoutName][terminalName]];
		WriteText(layoutName, terminalName, text);
		size_t spaceCount = (size_t)terminal.terminalSize.GetX() - (terminal.writeIndex % terminal.terminalSize.GetX());
		while (spaceCount > 0)
		{
			WriteText(layoutName, terminalName, " ");
			spaceCount--;
		}
	}

	void Terminals::SetForeground(const std::string& layoutName, const std::string& terminalName, const std::string_view& color)
	{
		auto& terminal = internalTerminals[terminalTable[layoutName][terminalName]];
		terminal.foregroundColor = color;
	}

	void Terminals::SetBackground(const std::string& layoutName, const std::string& terminalName, const std::string& color)
	{
		auto& terminal = internalTerminals[terminalTable[layoutName][terminalName]];
		terminal.backgroundColor = color;
	}

	void Terminals::Backspace(const std::string& layoutName, const std::string& terminalName)
	{
		auto& terminal = internalTerminals[terminalTable[layoutName][terminalName]];
		terminal.writeIndex = (terminal.writeIndex + (terminal.cells.size() - 1)) % terminal.cells.size();
		WriteGlyph(terminal, ' ');
		terminal.writeIndex = (terminal.writeIndex + (terminal.cells.size() - 1)) % terminal.cells.size();
	}
}
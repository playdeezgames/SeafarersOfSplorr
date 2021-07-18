#include "Visuals.Fonts.h"
#include "Data.JSON.h"
#include "Visuals.Sprites.h"
#include "Visuals.Colors.h"
#include "Data.Stores.h"
#include <format>
namespace visuals::Fonts
{
	static std::map<std::string, nlohmann::json> fontDataStore;

	static void InitializeFont(const std::string& fontName)
	{
		if (!fontDataStore.contains(fontName))
		{
			if (::data::Stores::GetStore(::data::Store::FONTS).count(fontName) > 0)
			{
				fontDataStore[fontName] = data::JSON::Load(::data::Stores::GetStore(::data::Store::FONTS)[fontName]);
			}
		}
	}

	const nlohmann::json empty = nlohmann::json();

	static const nlohmann::json& FontNameToDataStoreModel(const std::string fontName)
	{
		if (::data::Stores::GetStore(::data::Store::FONTS).count(fontName) > 0)
		{
			InitializeFont(fontName);
			return fontDataStore[fontName];
		}
		else
		{
			return empty;
		}
	}

	static std::map<std::string, std::map<char, std::optional<std::string>>> glyphSpriteNames;

	std::optional<std::string> GetGlyphSpriteName(const std::string& fontname, char ch)
	{
		auto fontIter = glyphSpriteNames.find(fontname);
		if (fontIter != glyphSpriteNames.end())
		{
			auto& font = fontIter->second;
			auto charIter = font.find(ch);
			if (charIter != font.end())
			{
				return charIter->second;
			}
		}
		std::optional<std::string> result;
		std::string key = std::format("{}", (int)ch);
		auto& data = FontNameToDataStoreModel(fontname);
		if (data.count(key) > 0)
		{
			std::string spriteName = data[key];
			glyphSpriteNames[fontname][ch] = std::optional<std::string>(spriteName);
			result = spriteName;
		}
		glyphSpriteNames[fontname][ch] = result;
		return result;
	}

	static common::XY<int> WriteGlyph(const std::string& fontname, const std::shared_ptr<common::Application::Renderer>& renderer, const common::XY<int>& xy, char ch, const std::string& color)
	{
		auto sprite = GetGlyphSpriteName(fontname, ch);
		Sprites::Draw(sprite.value(), renderer, xy, ::visuals::Colors::Read(color));
		return common::XY(xy.GetX() + Sprites::GetWidth(sprite.value()).value_or(0), xy.GetY());
	}

	void WriteTextLeft(const std::string& fontname, const std::shared_ptr<common::Application::Renderer>& renderer, const common::XY<int>& xy, const std::string& text, const std::string& color)
	{
		common::XY<int> temp = xy;
		for (auto ch : text)
		{
			temp = WriteGlyph(fontname, renderer, temp, ch, color);
		}
	}

	static int GetTextWidth(const std::string& fontname, const std::string& text)
	{
		int width = 0;
		for (auto ch : text)
		{
			const auto& sprite = GetGlyphSpriteName(fontname, ch);
			width += Sprites::GetWidth(sprite.value()).value_or(0);
		}
		return width;
	}

	static void WriteTextCentered(const std::string& fontname, const std::shared_ptr<common::Application::Renderer>& renderer, const common::XY<int>& xy, const std::string& text, const std::string& color)
	{
		auto adjustedXY = common::XY<int>(xy.GetX() - GetTextWidth(fontname, text) / 2, xy.GetY());
		WriteTextLeft(fontname, renderer, adjustedXY, text, color);
	}

	static void WriteTextRight(const std::string& fontname, const std::shared_ptr<common::Application::Renderer>& renderer, const common::XY<int>& xy, const std::string& text, const std::string& color)
	{
		auto adjustedXY = common::XY<int>(xy.GetX() - GetTextWidth(fontname, text), xy.GetY());
		WriteTextLeft(fontname, renderer, adjustedXY, text, color);
	}

	typedef std::function<void(const std::string&, const std::shared_ptr<common::Application::Renderer>&, const common::XY<int>&, const std::string&, const std::string&)> WriteFunction;

	const std::map<HorizontalAlignment, WriteFunction> writers =
	{
		{HorizontalAlignment::LEFT, WriteTextLeft},
		{HorizontalAlignment::RIGHT, WriteTextRight},
		{HorizontalAlignment::CENTER, WriteTextCentered}
	};

	void WriteText(const std::string& fontname, const std::shared_ptr<common::Application::Renderer>& renderer, const common::XY<int>& xy, const std::string& text, const std::string& color, const HorizontalAlignment& alignment)
	{
		writers.find(alignment)->second(fontname, renderer, xy, text, color);
	}
}
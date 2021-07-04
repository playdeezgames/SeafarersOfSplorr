#include "Visuals.Sprites.h"
#include "Visuals.Textures.h"
#include "Common.Data.Properties.h"
#include "Visuals.Data.Properties.h"
#include "Data.Stores.h"
#include <tuple>
#include "Common.Application.Internals.h"
namespace visuals::Sprites
{
	struct InternalSprite
	{
		std::string textureName;
		std::tuple<int, int, int, int> destination;
		std::tuple<int, int, int, int> source;
	};

	static std::map<std::string, InternalSprite> internalSprites;

	static void Initialize()
	{
		static bool initialized = false;
		if (!initialized)
		{
			auto& spriteStore = ::data::Stores::GetStore(::data::Store::SPRITES);
			for (auto& sprite : spriteStore.items())
			{
				auto& model = sprite.value();
				std::string textureName = model[visuals::data::Properties::TEXTURE];
				std::tuple<int, int, int, int> destination =
				{
					((model.count(data::Properties::OFFSET_X) > 0) ? ((int)model[data::Properties::OFFSET_X]) : (0)),
					((model.count(data::Properties::OFFSET_Y) > 0) ? ((int)model[data::Properties::OFFSET_Y]) : (0)),
					model[common::data::Properties::WIDTH],
					model[common::data::Properties::HEIGHT]
				};
				std::tuple<int,int,int,int> source = 
				{
					model[common::data::Properties::X],
					model[common::data::Properties::Y],
					model[common::data::Properties::WIDTH],
					model[common::data::Properties::HEIGHT]
				};
				internalSprites[sprite.key()] =
				{
					textureName,
					destination,
					source
				};
			}
			initialized = true;
		}
	}

	static void Render(
		const std::shared_ptr<common::Application::Renderer>& renderer, 
		const InternalSprite& internalSprite,
		std::tuple<unsigned char, unsigned char, unsigned char, unsigned char> color,
		const common::XY<int>& xy,
		double angle)
	{
		static SDL_Rect rcSource = {};
		static SDL_Rect rcDestination = {};
		auto texture = visuals::Textures::Read(renderer, internalSprite.textureName);
		SDL_SetTextureColorMod
		(
			texture.get(),
			std::get<0>(color),
			std::get<1>(color),
			std::get<2>(color)
		);
		SDL_SetTextureAlphaMod
		(
			texture.get(),
			std::get<3>(color)
		);
		rcDestination.x = std::get<0>(internalSprite.destination) + xy.GetX();
		rcDestination.y = std::get<1>(internalSprite.destination) + xy.GetY();
		rcDestination.w = std::get<2>(internalSprite.destination);
		rcDestination.h = std::get<3>(internalSprite.destination);
		rcSource.x = std::get<0>(internalSprite.source);
		rcSource.y = std::get<1>(internalSprite.source);
		rcSource.w = std::get<2>(internalSprite.source);
		rcSource.h = std::get<3>(internalSprite.source);
		renderer.get()->Copy(texture, &rcSource, &rcDestination, angle);
	}

	void Draw(const std::string& spriteName, const std::shared_ptr<common::Application::Renderer>& renderer, const ::common::XY<int>& xy, const std::tuple<unsigned char, unsigned char, unsigned char, unsigned char>& color, double angle)
	{
		Initialize();
		auto iter = internalSprites.find(spriteName);
		if (iter != internalSprites.end())
		{
			auto& internalSprite = iter->second;
			Render(renderer, internalSprite, color, xy, angle);
		}
	}

	std::optional<int> GetWidth(const std::string& spriteName)
	{
		Initialize();
		auto iter = internalSprites.find(spriteName);
		if (iter != internalSprites.end())
		{
			return std::get<2>(iter->second.destination);
		}
		return std::nullopt;
	}
}

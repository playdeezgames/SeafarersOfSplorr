#include "Common.Application.Internals.h"
#include "Common.Utility.h"
#include "Data.JSON.h"
#include "Data.Stores.h"
#include <map>
#include <memory>
#include <SDL_image.h>
#include "Visuals.Textures.h"
namespace visuals::Textures
{
	static std::map<std::string, std::shared_ptr<SDL_Texture>> table;

	static void Add(const std::string& name, const std::shared_ptr<SDL_Texture>& texture)
	{
		table[name] = texture;
	}

	static bool initialized = false;

	static void Initialize(const std::shared_ptr<common::Application::Renderer>& renderer)
	{
		if (!initialized)
		{
			auto& properties = data::Stores::GetStore(data::Store::TEXTURES);
			for (auto& entry : properties.items())
			{
				std::string imageFileName = entry.value();
				Add(entry.key(), std::shared_ptr<SDL_Texture>(IMG_LoadTexture(renderer.get()->renderer.get(), imageFileName.c_str()), SDL_DestroyTexture));
			}
			initialized = true;
		}
	}

	const std::shared_ptr<SDL_Texture>& Read(const std::shared_ptr<common::Application::Renderer>& renderer, const std::string& name)
	{
		Initialize(renderer);
		return table.find(name)->second;
	}
}
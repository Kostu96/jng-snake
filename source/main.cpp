#include "game_layer.hpp"

#include <jng/core/engine.hpp>
#include <jng/core/entry_point.hpp>

#include <imgui/imgui.h>

class Game :
	public jng::Engine
{
public:
	Game() :
		Engine{ {
			"Snake",
			1024, 768,
			RendererType::Renderer2D
		} }
	{
		ImGui::GetIO().Fonts->AddFontFromFileTTF("assets/fonts/lato.ttf", 20);

		getLayerStack().pushLayer(new GameLayer{});
	}
};

jng::Engine* createApp() {
	return new Game{};
}

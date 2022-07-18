#pragma once
#include "snake_controller.hpp"

#include <jng/core/layer.hpp>
#include <jng/scene/components.hpp>
#include <jng/scene/scene.hpp>
#include <jng/renderer/framebuffer.hpp>

class GameLayer :
	public jng::Layer
{
public:
	GameLayer();
	~GameLayer();

	void onUpdate(float dt) override;
	void onImGuiUpdate() override;
	void onEvent(jng::Event& event) override;
private:
	jng::Scene m_scene;
	SnakeController* m_snakeControllerScript = nullptr;
};

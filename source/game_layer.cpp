#include "game_layer.hpp"
#include "snake_controller.hpp"

#include <jng/renderer/renderer_api.hpp>
#include <jng/scene/entity.hpp>

#include <imgui/imgui.h>

GameLayer::GameLayer()
{
	auto camera = m_scene.createEntity("MainCamera");
	auto& cameraComponent = camera.addComponent<jng::CameraComponent>();
	cameraComponent.camera.setViewportSize(1024, 768);
	cameraComponent.camera.setOrthographicSize(768.f / 20.f);

	auto snake = m_scene.createEntity("Snake");
	snake.addComponent<jng::SpriteComponent>().color = { 0.1f, 0.8f, 0.1f, 1.f };
	auto& snakeControllerSC = snake.addComponent<jng::NativeScriptComponent>();
	snakeControllerSC.bind<SnakeController>();

	auto apple = m_scene.createEntity("Apple");
	apple.addComponent<jng::SpriteComponent>().color = { 0.8f, 0.1f, 0.1f, 1.f };

	auto topWall = m_scene.createEntity("TopWall");
	auto& topWallTC = topWall.getComponent<jng::TransformComponent>();
	topWallTC.scale.x = 1024.f / 20.f;
	topWallTC.translation.y += 19.f;
	topWall.addComponent<jng::SpriteComponent>().color = { 0.f, 0.f, 0.f, 1.f };

	auto bottomWall = m_scene.createEntity("bottomWall");
	auto& bottomWallTC = bottomWall.getComponent<jng::TransformComponent>();
	bottomWallTC.scale.x = 1024.f / 20.f;
	bottomWallTC.translation.y -= 19.f;
	bottomWall.addComponent<jng::SpriteComponent>().color = { 0.f, 0.f, 0.f, 1.f };

	auto leftWall = m_scene.createEntity("LeftWall");
	auto& leftWallTC = leftWall.getComponent<jng::TransformComponent>();
	leftWallTC.scale.y = 768.f / 20.f;
	leftWallTC.translation.x -= 26.f;
	leftWall.addComponent<jng::SpriteComponent>().color = { 0.f, 0.f, 0.f, 1.f };

	auto rightWall = m_scene.createEntity("RightWall");
	auto& rightWallTC = rightWall.getComponent<jng::TransformComponent>();
	rightWallTC.scale.y = 768.f / 20.f;
	rightWallTC.translation.x += 17.f;
	rightWall.addComponent<jng::SpriteComponent>().color = { 0.f, 0.f, 0.f, 1.f };

	m_scene.onCreate();

	m_snakeControllerScript = dynamic_cast<SnakeController*>(snakeControllerSC.instance);
	m_snakeControllerScript->m_appleEntity = apple;
}

GameLayer::~GameLayer()
{
	m_scene.onDestroy();
}

void GameLayer::onUpdate(float dt)
{
	jng::RendererAPI::clear({ 0.2f, 0.2f, 0.2f });

	m_scene.onUpdate(dt);
}

void GameLayer::onImGuiUpdate()
{
	static ImGuiWindowFlags flags = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;

	if (!m_snakeControllerScript->m_isStarted)
	{
		ImGui::SetNextWindowSize({ 270.f, 100.f });
		ImGui::SetNextWindowPos({ (1024.f - 162.f - 270.f) / 2.f, (768.f - 100.f) / 2.f - 150.f });
		ImGui::Begin("Welcome", nullptr, flags ^ ImGuiWindowFlags_NoBackground);
		ImGui::Text("Press arrow keys to start the game\n        and to change directions.\n\n            Use SPACE to pause.");
		ImGui::End();
	}

	if (m_snakeControllerScript->m_isPaused)
	{
		ImGui::SetNextWindowSize({ 200.f, 64.f });
		ImGui::SetNextWindowPos({ (1024.f - 162.f - 200.f) / 2.f, (768.f - 64.f) / 2.f - 150.f });
		ImGui::Begin("Pause", nullptr, flags ^ ImGuiWindowFlags_NoBackground);
		ImGui::Text("      Game is paused.\nPress SPACE to unpause.");
		ImGui::End();
	}

	if (m_snakeControllerScript->m_isGameOver)
	{
		ImGui::SetNextWindowSize({ 140.f, 64.f });
		ImGui::SetNextWindowPos({ (1024.f - 162.f - 140.f) / 2.f, (768.f - 64.f) / 2.f - 150.f });
		ImGui::Begin("GameOver", nullptr, flags ^ ImGuiWindowFlags_NoBackground);
		ImGui::Text("   Game Over!\nPress R to reset.");
		ImGui::End();
	}

	ImGui::SetNextWindowSize({ 162.f, 745.f });
	ImGui::SetNextWindowPos({ 1024.f - 162.f, 768.f - 755.f });
	ImGui::Begin("SidePanel", nullptr, flags);
	ImGui::Text("Segments: %d", m_snakeControllerScript->m_segments);
	ImGui::Text("Score:\n%d", m_snakeControllerScript->m_score);
	ImGui::End();
}

void GameLayer::onEvent(jng::Event& event)
{
	m_scene.onEvent(event);
}

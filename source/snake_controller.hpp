#pragma once
#include <jng/scene/components.hpp>
#include <jng/scripting/native_script.hpp>

class SnakeController :
	public jng::NativeScript
{
public:
	SnakeController() = default;

	void onCreate() override;
	void onUpdate(float dt) override;
	void onEvent(jng::Event& event) override;

	jng::Entity m_appleEntity;
	unsigned int m_score = 0;
	unsigned int m_segments = 1;
	bool m_isPaused = false;
	bool m_isStarted = false;
	bool m_isGameOver = false;
private:
	void reset();

	bool m_respawnApple = true;
	glm::ivec2 m_direction{ 0, 0 };
	glm::ivec2 m_applePosition{ 0, 0 };
	std::vector<glm::ivec2> m_snakePositions;
	std::vector<jng::TransformComponent*> m_segmentsTCs;
	std::vector<jng::Entity> m_entitiesToDeleteOnReset;
};

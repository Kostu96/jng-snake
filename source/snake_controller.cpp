#include "snake_controller.hpp"

#include <jng/core/event.hpp>
#include <jng/core/key_events.hpp>
#include <jng/platform/key_codes.hpp>

constexpr int LEFT = -25;
constexpr int RIGHT = 16;
constexpr int TOP = 18;
constexpr int BOTTOM = -18;

void SnakeController::onCreate()
{
	srand(time(0));
	reset();
}

void SnakeController::onUpdate(float dt)
{
	static auto& transform = getComponent<jng::TransformComponent>();
	static float moveTime = 1.f;
	static float timeAcc = 0.f;

	if (m_respawnApple)
	{
		m_respawnApple = false;

		bool posFound = false;
		do {
			m_applePosition.x = rand() % (RIGHT - LEFT + 1) + LEFT;
			m_applePosition.y = rand() % (TOP - BOTTOM + 1) + BOTTOM;

			posFound = true;
			for (const auto& pos : m_snakePositions)
				if (pos.x == m_applePosition.x && pos.y == m_applePosition.y)
					posFound = false;

		} while (!posFound);

		auto& transform = m_appleEntity.getComponent<jng::TransformComponent>();
		transform.translation.x = m_applePosition.x;
		transform.translation.y = m_applePosition.y;
	}

	if (m_isStarted && !m_isPaused && !m_isGameOver)
	{
		timeAcc += dt;
		if (timeAcc >= moveTime)
		{
			timeAcc -= moveTime;
		
			for (size_t i = m_snakePositions.size() - 1; i >= 1; --i)
			{
				m_snakePositions[i] = m_snakePositions[i - 1];
				m_segmentsTCs[i - 1]->translation.x = m_snakePositions[i].x;
				m_segmentsTCs[i - 1]->translation.y = m_snakePositions[i].y;
			}
			m_snakePositions[0].x += m_direction.x;
			m_snakePositions[0].y += m_direction.y;
			if (m_snakePositions[0].x == RIGHT + 1) m_snakePositions[0].x = LEFT;
			else if (m_snakePositions[0].x == LEFT - 1) m_snakePositions[0].x = RIGHT;
			else if (m_snakePositions[0].t == TOP + 1) m_snakePositions[0].y = BOTTOM;
			else if (m_snakePositions[0].t == BOTTOM - 1) m_snakePositions[0].y = TOP;
			transform.translation.x = m_snakePositions[0].x;
			transform.translation.y = m_snakePositions[0].y;
			
			for (size_t i = 1; i < m_snakePositions.size(); ++i)
				if (m_snakePositions[0].x == m_snakePositions[i].x && m_snakePositions[0].y == m_snakePositions[i].y)
					m_isGameOver = true;

			if (m_snakePositions[0].x == m_applePosition.x && m_snakePositions[0].y == m_applePosition.y)
			{
				++m_segments;
				m_score += 10 * 1.2f / moveTime;

				m_respawnApple = true;
				moveTime *= 0.93f;

				jng::Entity segment = createEntity("segment");
				m_entitiesToDeleteOnReset.push_back(segment);
				auto& segmentTC = segment.getComponent<jng::TransformComponent>();
				m_segmentsTCs.push_back(&segmentTC);
				glm::ivec2 segmentPos = { m_snakePositions[0].x, m_snakePositions[0].y };
				segmentTC.translation.x = segmentPos.x;
				segmentTC.translation.y = segmentPos.y;
				m_snakePositions.push_back(segmentPos);
				segment.addComponent<jng::SpriteComponent>().color = { 0.1f, 0.8f, 0.1f, 1.f };
			}
		}
	}
}

void SnakeController::onEvent(jng::Event& event)
{
	jng::EventDispatcher dispatcher{ event };

	dispatcher.dispatch<jng::KeyPressEvent>([this](jng::KeyPressEvent& event)
		{
			switch (event.getKeyCode())
			{
			case jng::Key::Space:
				if (m_direction.x == 0 && m_direction.y == 0)
					break;
				m_isPaused = !m_isPaused;
				break;
			case jng::Key::Up:
				if (m_direction.y == -1 || m_isPaused)
					break;
				m_isStarted = true;
				m_direction = { 0, 1 };
				break;
			case jng::Key::Down:
				if (m_direction.y == 1 || m_isPaused)
					break;
				m_isStarted = true;
				m_direction = { 0, -1 };
				break;
			case jng::Key::Left:
				if (m_direction.x == 1 || m_isPaused)
					break;
				m_isStarted = true;
				m_direction = { -1, 0 };
				break;
			case jng::Key::Right:
				if (m_direction.x == -1 || m_isPaused)
					break;
				m_isStarted = true;
				m_direction = { 1, 0 };
				break;
			case jng::Key::R:
				if (m_isGameOver)
					reset();
				break;
			}

			return true;
		});
}

void SnakeController::reset()
{
	m_score = 0;
	m_segments = 1;
	m_isGameOver = false;
	m_isStarted = false;
	m_respawnApple = true;
	m_direction = { 0, 0 };

	m_snakePositions.clear();
	m_snakePositions.push_back({ -5, 0 });
	auto& transform = getComponent<jng::TransformComponent>();
	transform.translation.x = m_snakePositions[0].x;
	transform.translation.y = m_snakePositions[0].y;

	for (auto entity : m_entitiesToDeleteOnReset)
		destroyEntity(entity);

	m_entitiesToDeleteOnReset.clear();
}

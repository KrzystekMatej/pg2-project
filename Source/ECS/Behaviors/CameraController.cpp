#include "ECS/Behaviors/CameraController.h"
#include "EventSystem/EventDispatcher.h"
#include "EventSystem/Events/KeyEvent.h"
#include "EventSystem/Events/CursorPositionEvent.h"
#include "Core/Time.h"
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include "EventSystem/Events/FrameBufferSizeEvent.h"


void CameraController::Awake(entt::entity entity, entt::registry& registry)
{
	m_Transform = &registry.get<Transform>(entity);
	m_Camera = &registry.get<CameraData>(entity);

	EventDispatcher::AddListener(Event::Type::Key, [this](const Event& event) { OnKeyPressed(event); });
	EventDispatcher::AddListener(Event::Type::CursorPosition, [this](const Event& event) { OnCursorPositionChanged(event); });
	EventDispatcher::AddListener(Event::Type::FrameBufferSize, [this](const Event& event) { OnFrameBufferSizeChanged(event); });
}

void CameraController::Update(entt::entity entity, entt::registry& registry)
{
	glm::vec3 forward = m_Transform->GetForwardVector();
	glm::vec3 right = glm::normalize(glm::cross(forward, Transform::WorldUp));
	glm::vec3 up = glm::normalize(glm::cross(right, forward));

	m_Transform->Position += Time::GetDeltaTime() * m_Speed * (forward * m_Input.z + up * m_Input.y + right * m_Input.x);
}

void CameraController::OnKeyPressed(const Event& event)
{
	const auto& keyEvent = static_cast<const KeyEvent&>(event);
	if (keyEvent.GetAction() == GLFW_RELEASE)
	{
		m_Input = glm::vec3(0.0f);
		return;
	}

    switch (keyEvent.GetKey())
    {
		case GLFW_KEY_W:
			m_Input.z = 1.0f;
			break;
		case GLFW_KEY_S:
			m_Input.z = -1.0f;
			break;
		case GLFW_KEY_D:
			m_Input.x = 1.0f;
			break;
		case GLFW_KEY_A:
			m_Input.x = -1.0f;
			break;
		case GLFW_KEY_R:
			m_Input.y = 1.0f;
			break;
		case GLFW_KEY_F:
			m_Input.y = -1.0f;
			break;
		default:
			break;
    }
}

void CameraController::OnCursorPositionChanged(const Event& event) const
{
	const auto& cursorEvent = static_cast<const CursorPositionEvent&>(event);
	glm::vec2 current = cursorEvent.GetPosition();

	glm::vec2 center = glm::vec2(static_cast<float>(m_Window.GetWidth()) / 2.0f, static_cast<float>(m_Window.GetHeight()) / 2.0f);

	static bool init = false;
	if (!init)
	{
		center = current;
		init = true;
	}

	glm::vec2 delta = (current - center) * m_MouseSensitivity;

	m_Transform->Rotation.y += delta.x;
	m_Transform->Rotation.x -= delta.y;

	m_Transform->Rotation.x = glm::clamp(m_Transform->Rotation.x, -(glm::half_pi<float>() - 0.1f), glm::half_pi<float>() - 0.1f);
}

void CameraController::OnFrameBufferSizeChanged(const Event& event) const
{
	const auto& frameBufferEvent = static_cast<const FrameBufferSizeEvent&>(event);
	m_Camera->AspectRatio = static_cast<float>(frameBufferEvent.GetWidth()) / static_cast<float>(frameBufferEvent.GetHeight());
}

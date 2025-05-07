#include "ECS/Behaviors/CameraController.h"
#include "EventSystem/EventDispatcher.h"
#include "EventSystem/Events/KeyEvent.h"
#include "EventSystem/Events/CursorPositionEvent.h"
#include "Core/Time.h"
#include <GLFW/glfw3.h>


void CameraController::Awake()
{
	EventDispatcher::AddListener(Event::Type::Key, [this](const Event& event) { OnKeyPressed(event); });
	EventDispatcher::AddListener(Event::Type::CursorPosition, [this](const Event& event) { OnCursorPositionChanged(event); });
}

void CameraController::Update()
{
	Transform& transform = m_Entity.GetComponent<Transform>();

	glm::vec3 forward = transform.GetForwardVector();
	glm::vec3 right = glm::normalize(glm::cross(forward, Transform::WorldUp));
	glm::vec3 up = glm::normalize(glm::cross(right, forward));

	transform.Position += Time::GetDeltaTime() * m_Speed * (forward * m_Input.z + up * m_Input.y + right * m_Input.x);
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

void CameraController::OnCursorPositionChanged(const Event& event)
{
	const auto& cursorEvent = static_cast<const CursorPositionEvent&>(event);
	glm::vec2 current = cursorEvent.GetPosition();

	glm::vec2 center = glm::vec2(static_cast<float>(m_Window->GetWidth()) / 2.0f, static_cast<float>(m_Window->GetHeight()) / 2.0f);

	static bool init = false;
	if (!init)
	{
		center = current;
		init = true;
	}

	glm::vec2 delta = (current - center) * m_MouseSensitivity;

	Transform& transform = m_Entity.GetComponent<Transform>();

	transform.Rotation.y += delta.x;
	transform.Rotation.x -= delta.y;
	transform.Rotation.x = glm::clamp(transform.Rotation.x, -(glm::half_pi<float>() - 0.1f), glm::half_pi<float>() - 0.1f);
}
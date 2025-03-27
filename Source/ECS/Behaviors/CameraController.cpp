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

void CameraController::OnKeyPressed(const Event& event) const
{
	const auto& keyEvent = static_cast<const KeyEvent&>(event);
	if (keyEvent.GetAction() == GLFW_RELEASE) return;

	glm::vec3 move(0);

    switch (keyEvent.GetKey())
    {
		case GLFW_KEY_W:
			move.z = m_Speed * Time::GetDeltaTime();
			break;
		case GLFW_KEY_S:
			move.z = -m_Speed * Time::GetDeltaTime();
			break;
		case GLFW_KEY_D:
			move.x = m_Speed * Time::GetDeltaTime();
			break;
		case GLFW_KEY_A:
			move.x = -m_Speed * Time::GetDeltaTime();
			break;
		default:
			return;
    }

	glm::vec3 perpVector;
	perpVector.x = sin(m_Transform->Rotation.x - glm::pi<float>() / 2.0f);
	perpVector.y = 0;
	perpVector.z = cos(m_Transform->Rotation.x - glm::pi<float>() / 2.0f);

	glm::vec3 rightVector = glm::normalize(glm::cross(m_Transform->GetFrontVector(), m_Transform->GetUpVector()));

	m_Transform->Position += m_Transform->GetFrontVector() * move.z + rightVector * move.x;
}

void CameraController::OnCursorPositionChanged(const Event& event) const
{
	/*
	const auto& cursorEvent = static_cast<const CursorPositionEvent&>(event);
	m_Transform->Rotation.x += (static_cast<float>(m_Camera->Width) / 2.f - cursorEvent.GetPosition().x) * m_MouseSensitivity;
	m_Transform->Rotation.y += (static_cast<float>(m_Camera->Height) / 2.f - cursorEvent.GetPosition().y) * m_MouseSensitivity;
	spdlog::info("Rotation x: {}, y: {}", m_Transform->Rotation.x, m_Transform->Rotation.y);

	if (m_Transform->Rotation.y < -glm::radians(89.9f)) m_Transform->Rotation.x = -glm::radians(89.9f);
	else if (m_Transform->Rotation.y > glm::radians(89.9f)) m_Transform->Rotation.x = glm::radians(89.9f);*/
}

void CameraController::OnFrameBufferSizeChanged(const Event& event) const
{
	const auto& frameBufferEvent = static_cast<const FrameBufferSizeEvent&>(event);
	m_Camera->Width = frameBufferEvent.GetWidth();
	m_Camera->Height = frameBufferEvent.GetHeight();
}

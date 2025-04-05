#pragma once
#include "Behavior.h"
#include "Core/Window.h"
#include "EventSystem/Events/Event.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/CameraData.h"

class CameraController : public Behavior
{
public:
	CameraController(float speed, float mouseSensitivity, const Window& window)
		: m_Speed(speed), m_MouseSensitivity(mouseSensitivity), m_Window(window), m_Input(0.0f), m_Transform(nullptr), m_Camera(nullptr) {}
	void Awake(entt::entity entity, entt::registry& registry) override;
	void Update(entt::entity entity, entt::registry& registry) override;
private:
	void OnKeyPressed(const Event& event);
	void OnCursorPositionChanged(const Event& event) const;
	void OnFrameBufferSizeChanged(const Event& event) const;

	float m_Speed;
	float m_MouseSensitivity;
	const Window& m_Window;

	glm::vec3 m_Input;

	Transform* m_Transform;
	CameraData* m_Camera;
};
#pragma once
#include "Behavior.h"
#include "Core/Window.h"
#include "EventSystem/Events/Event.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Camera.h"

class CameraController : public Behavior
{
public:
	CameraController(Entity entity, float speed, float mouseSensitivity, const Window* window)
		: Behavior(entity), m_Speed(speed), m_MouseSensitivity(mouseSensitivity), m_Window(window), m_Input(0.0f) {}
	void Awake() override;
	void Update() override;
private:
	void OnKeyPressed(const Event& event);
	void OnCursorPositionChanged(const Event& event);

	float m_Speed;
	float m_MouseSensitivity;
	const Window* m_Window;

	glm::vec3 m_Input;
};
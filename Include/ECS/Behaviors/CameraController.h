#pragma once
#include "Behavior.h"
#include "EventSystem/Events/Event.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/CameraData.h"

class CameraController : public Behavior
{
public:
	explicit CameraController(float speed) : m_Speed(speed), m_Transform(nullptr), m_Camera(nullptr) {}
	void Awake(entt::entity entity, entt::registry& registry) override;
	void Update(entt::entity entity, entt::registry& registry) override {}
private:
	void OnKeyPressed(const Event& event) const;
	void OnCursorPositionChanged(const Event& event) const;
	void OnFrameBufferSizeChanged(const Event& event) const;

	float m_Speed;
	Transform* m_Transform;
	CameraData* m_Camera;
};
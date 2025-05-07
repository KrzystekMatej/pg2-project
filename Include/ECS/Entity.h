#pragma once
#include <utility>
#include "Scene/Scene.h"

class Entity
{
public:
	Entity() = default;
	Entity(Scene* scene) : m_Scene(scene)
	{
		m_EntityHandle = m_Scene->GetRegistry().create();
	}
	Entity(const Entity& other) = default;

	template<typename T, typename... Args>
	T& AddComponent(Args&&... args)
	{
		return m_Scene->GetRegistry().emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
	}

	template<typename T, typename... Args>
	T& AddOrReplaceComponent(Args&&... args)
	{
		T& component = m_Scene->GetRegistry().emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
		return component;
	}

	template<typename T>
	T& GetComponent()
	{
		return m_Scene->GetRegistry().get<T>(m_EntityHandle);
	}

	template<typename T>
	const T& GetComponent() const
	{
		return m_Scene->GetRegistry().get<T>(m_EntityHandle);
	}

	template<typename T>
	bool HasComponent() const
	{
		return m_Scene->GetRegistry().all_of<T>(m_EntityHandle);
	}

	template<typename T>
	void RemoveComponent()
	{
		m_Scene->GetRegistry().remove<T>(m_EntityHandle);
	}

	entt::entity GetID() const
	{
		return m_EntityHandle;
	}

	bool IsValid() const
	{
		return m_Scene->GetRegistry().valid(m_EntityHandle);
	}

	operator bool() const { return m_EntityHandle != entt::null; }
	explicit operator entt::entity() const { return m_EntityHandle; }
	explicit operator uint32_t() const { return static_cast<uint32_t>(m_EntityHandle); }
	Entity& operator=(const Entity&) = default;

	bool operator==(const Entity& other) const
	{
		return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
	}

	bool operator!=(const Entity& other) const
	{
		return !(*this == other);
	}
private:
	entt::entity m_EntityHandle{ entt::null };
	Scene* m_Scene;
};

#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

class Component;
class Entity;

// ComponentID is std::size_t.
using ComponentID = std::size_t;

// 1-2-3-4-5 etc.....
inline ComponentID GetComponentTypeID()
{
	static ComponentID lastID = 0;

	return lastID++;
}

template <typename T>
inline ComponentID GetComponentTypeID() noexcept
{
	static ComponentID typeID = GetComponentTypeID();

	return typeID;
}
constexpr std::size_t maxComponents = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray = std::array<Component*, maxComponents>;

class Component
{
public:
	Entity* entity;

	virtual void init() {}
	virtual void update() {}
	virtual void draw() {}

	virtual ~Component() {}
};
class Entity
{
private:
	bool active = true;
	std::vector<std::unique_ptr<Component>> components;

	ComponentArray componentArray;
	ComponentBitSet componentBitSet;
public:
	void update()
	{
		for (auto& comp : components)
		{
			comp->update();
			
		}
	}
	void draw() 
	{
		for (auto& comp : components)
		{
			comp->update();

		}
	}

	bool isActive() const { return active; }
	void destroy() { active = false; }

	template <typename T>
	bool HasComponent() const
	{
		return componentBitSet[GetComponentTypeID<T>()];
	}
	template <typename T, typename... TArgs>
	T& AddComponent(TArgs&&... mArgs)
	{
		T* c(new T(std::forward<TArgs>()(mArgs)...));
		c->entity = this;
		std::unique_ptr<Component> uPtr{ c };
		components.emplace_back(std::move(uPtr));

		componentArray[GetComponentTypeID<T>()] = c;
		componentBitSet[GetComponentTypeID<T>()] = true;

		c->init();

		return *c;
	}
	template <typename T>
	T& GetComponent() const
	{
		auto ptr(componentArray[GetComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}

};

class Manager
{
private:
	std::vector<std::unique_ptr<Entity>> entities;

public:
	void update()
	{
		for (auto& e : entities)
		{
			e->update();
		}
	}
	void draw()
	{
		for (auto& e : entities)
		{
			e->draw();
		}
	}
	void refresh()
	{
		entities.erase(std::remove_if(std::begin(entities), std::end(entities),
			[](const std::unique_ptr<Entity> &mEntity)
		{
			return !mEntity->isActive();
		}),
			std::end(entities));
	}
	Entity& addEntity()
	{
		Entity* e = new Entity();
		std::unique_ptr<Entity> uPtr{ e };
		entities.emplace_back(std::move(uPtr));
		return *e;
	}
};
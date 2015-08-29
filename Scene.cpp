#include "Scene.h"
#include "EntityManager.h"

#include <queue>

using namespace Space;

namespace MEngineNS
{

Scene::Scene()
{
	m_bInitialized = false;
}

Scene::~Scene()
{
	for (Entity* pEntity : m_occupants)
	{
		delete pEntity;
	}
}

void Scene::Initialize(Graphics* pGraphics)
{
	m_pGraphics = pGraphics;
	m_bInitialized = true;
}

/*
const Entity::ConstEntitySet Scene::Occupants() const
{
	Entity::ConstEntitySet occupants;
	occupants.insert(m_occupants.begin(), m_occupants.end());
	return occupants;
}
*/

void Scene::Update(double deltaT)
{
	// Update entities
	for (Entity* pEntity : m_occupants)
	{
		if (pEntity == NULL)
		{
			throw GameException(GameExceptionNS::FATAL_ERROR,
				"Scene has a NULL entity");
		}

		pEntity->Update(deltaT);
	}

	DeleteFlaggedEntities();
}

void Scene::ConstUpdate()
{
	for (Entity* pEntity : m_occupants)
	{
		if (pEntity == NULL)
		{
			throw GameException(GameExceptionNS::FATAL_ERROR,
				"Scene has a NULL entity");
		}

		pEntity->ConstUpdate();
	}

	HandleCollisions();
}

void Scene::HandleCollisions()
{
	// For each entity, if it's a collider (just projectiles at the moment),
	// see if it collides with any hostile, damageable entities in the same sector.
	// If it collides, remove the collider.
	for (Entity* pEntity : m_occupants)
	{
		pEntity->CollisionUpdate();
	}
}



void Scene::AddEntity(Entity* entity)
{
	if (entity != NULL)
	{
		m_occupants.emplace(entity);
		entity->InitializeDrawable(m_pGraphics);
	}
}

void Scene::RemoveEntity(Entity* entity)
{
	if (entity != NULL)
	{
		m_toRemove.emplace(entity);
	}
}

void Scene::DestroyEntity(Entity* entity)
{
	if (entity != NULL)
	{
		m_toDelete.emplace(entity);
	}
}

void Scene::DeleteFlaggedEntities()
{
	for (Entity* entity : m_toDelete)
	{
		m_occupants.erase(entity);
		delete entity;
	}

	m_toDelete.clear();
}

void Scene::RemoveFlaggedEntities()
{
	for (Entity* entity : m_toRemove)
	{
		m_occupants.erase(entity);
	}
}

bool Scene::ContainsEntity(Entity* entity) const
{
	return m_occupants.count(entity) > 0;
}

void Scene::Render(double interp, Point2D<double>& offset)
{
	if (!m_bInitialized)
	{
		throw GameException(GameExceptionNS::FATAL_ERROR,
			"Attempted to render uninitialized scene");
	}

	// Render entities in the scene
	for (Entity* pEntity : m_occupants)
	{
		if (pEntity == NULL)
		{
			throw GameException(GameExceptionNS::FATAL_ERROR,
				"Scene has a NULL entity");
		}

		pEntity->Render(interp, offset);
	}
}

}


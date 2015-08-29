#pragma once

#include "Entity.h"
#include "EntityManager.h"
#include "CollisionPolygon.h"
#include "Vector2D.h"
#include <string>

using namespace Space;

namespace MEngineNS
{

Entity::Entity(const Space::Point2D<double>& inPos)
{
	m_ID = Entity::NextValidID();
	m_position = inPos;
	m_allegience = SWISS;
	m_dOrientation = 0;
	m_bCollider = false;
	m_bDamageable = false;
	m_pPolygon = NULL;

	EntityManager::Instance()->RegisterEntity(this);
}

Entity::Entity(const Space::Point2D<double>& inPos, Faction inAllegience)
{
	m_ID = Entity::NextValidID();
	m_position = inPos;
	m_allegience = inAllegience;
	m_dOrientation = 0;
	m_bCollider = false;
	m_bDamageable = false;
	m_pPolygon = NULL;

	EntityManager::Instance()->RegisterEntity(this);
}

bool Entity::Initialize(Graphics* pGraphics, int width, int height,
	int nCols, const char* texture)
{
	return Drawable::Initialize(pGraphics, width, height, nCols, texture);
}

Entity::~Entity()
{
	if (m_pPolygon != NULL)
	{
		delete m_pPolygon;
	}

	EntityManager::Instance()->RemoveEntity(m_ID);
}

void Entity::SetCollisionPolygon(std::vector<Point2D<double>>& vertices)
{
	if (m_pPolygon != NULL)
	{
		delete m_pPolygon;
	}

	m_pPolygon = new CollisionPolygon(vertices);
}

void Entity::Update(double deltaT)
{
	Drawable::UpdateAnimation(deltaT);
}

bool Entity::CanCollide(const Entity* other) const
{
	// If I ever implement a grid system for sectors, this method should
	// use it to see if collision is possible.

	// Could also override in children to check if the other entit is hostile or
	// something.

	return true;
}

bool Entity::IsHostile(const Entity* entity) const
{
	bool isHostile = false;

	if ((GetAllegience() == ENEMYFACTION) && (entity->GetAllegience() == PLAYERFACTION))
	{
		isHostile = true;
	}
	else if ((GetAllegience() == PLAYERFACTION) && (entity->GetAllegience() == ENEMYFACTION))
	{
		isHostile = true;
	}

	return isHostile;
}

bool Entity::CollidesWith(const Entity* other) const
{
	return CanCollide(other) ?
		CollisionPolygon::DetectCollision(
		*m_pPolygon,
		*other->m_pPolygon,
		other->m_position - m_position,
		GetVelocity(),
		other->GetVelocity(),
		m_dOrientation,
		other->m_dOrientation) :
		false;
}

void Entity::Render(float interpolation, Point2D<double>& offset)
{
	UpdateDrawable(interpolation, offset);
	Drawable::Draw(WHITE);
}

bool Entity::operator<(const Entity& other) const
{
	return (GetShipClass() < other.GetShipClass());
}

void Entity::UpdateDrawable(double interp, Space::Point2D<double>& offset)
{
	Point2D<double> interpPos = GetInterpPos(interp) + offset;
	Drawable::SetX(interpPos.GetX() - Drawable::GetWidth() / 2.0);
	Drawable::SetY(interpPos.GetY() - Drawable::GetHeight() / 2.0);
	Drawable::SetAngle(Vector2D::SimplifyAngle(m_dOrientation + HALF_PI));
}

}


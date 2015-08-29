#pragma once

#include <unordered_set>
#include <unordered_map>
#include <set>

#include "Data.h"
#include "Entity.h"
#include "Point2D.h"

namespace MEngineNS
{

/**
 * This class represents a scene of the game, where the game's entities exist.
 *
 * @author Markus Gemperle
 * @version 23/02/2014
 */
class Scene
{

public:

	/**
	 * Constructor for the Scene.
	 */
	Scene();

	~Scene();

	// Initialize the scene. Must be called before rendering.
	void Initialize(Graphics* pGraphics);

	// Update method called as often as possible which updates all entities
	// in the scene.
	// Receives time since previous update as parameter.
	void Update(double deltaT);

	// Update method for physics that is called at a constant frequency defined in
	// Data.h
	void ConstUpdate();

	/**
	 * This method adds the argued entity to the Scene.
	 *
	 * @param entity  the entity to be added to the Scene
	 */
	void AddEntity(Entity* entity);

	/**
	* This method removes the argued entity from the Scene if
	* it is present.
	*/
	void RemoveEntity(Entity* entity);

	// Returns true if the argued entity exists in this scene.
	bool ContainsEntity(Entity* entity) const;

	// Destroys the argued entity in the screen.
	void DestroyEntity(Entity* entity);

	// Returns set of entities that exist in this scene.
	const Entity::EntitySet Occupants() const { return m_occupants; }

	// Renders the scene with the argued interpolation and offset.
	void Render(double interpolation, Space::Point2D<double>& offset);

private:

	// Private instance fields

	Graphics* m_pGraphics;
	Entity::EntitySet m_occupants;

	std::set<Entity*> m_toDelete;
	std::set<Entity*> m_toRemove;

	// Private methods

	void DeleteFlaggedEntities();
	void RemoveFlaggedEntities();
	
	/** Check for and inform entities of any collisions **/
	void HandleCollisions();

	bool m_bInitialized;
};



}


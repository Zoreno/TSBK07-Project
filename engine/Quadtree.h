#pragma once
#include "EntityManager.h"
#include <vector>
#include "TransformComponent.h"
#include <stdexcept>
#include "EntityDestroyedEvent.h"
#include "ComponentAssignedEvent.h"


//Error class
/**
 * \brief Error class
 */
class Quadtree_error : public std::logic_error {
	using std::logic_error::logic_error;
};
class Quadroot;
class Quadleaf;

/**
 * \brief Outwards visible Quadtree class, calling the actual structure
 */
class Quadtree : public Subscriber<EntityDestroyedEvent>, public Subscriber<ComponentAssignedEvent<TransformComponent>>
{
public:
	/**
	 * \brief Constructor
	 * \param position Center position of quadtree
	 * \param width Width (x-width) of quadtree
	 * \param height Height (y-height) of quadtree
	 */
	Quadtree(EntityManager* entMan, EventManager* evMan, glm::vec2 position, uint32_t width, uint32_t height);
	~Quadtree();

	/**
	 * \brief Updates all placements of all entities in the tree 
	 */
	void update();

	/**
	 * \brief Pushes an entity into the tree, placing it correctly
	 * \param ent Handle to entity
	 */
	void pushEntity(EntityHandle ent);


	/**
	 * \brief Destroyed entity handler
	 * \param ev The recieved event to be handled
	 */
	void handleEvent(const EntityDestroyedEvent& ev) override;


	/**
	 * \brief TransformComponent assignment handler
	 * \param ev The recieved event to be handled
	 */
	void handleEvent(const ComponentAssignedEvent<TransformComponent>& ev) override;


	/**
	 * \brief Entitycounter
	 * \return The number of entities in the current quad
	 */
	uint16_t getEntCount();

	/**
	 * \brief Entitycounter
	 * \return The number of total entities in and below the current quad
	 */
	uint16_t getTotalEntCount();

private:
	/**
	 * \brief Vector holding the pending entities to be removed
	 */
	std::vector<std::pair<EntityHandle, uint32_t>> _entToRemove{};

	/**
	 * \brief The actual quadtree pointer
	 */
	Quadroot* _quadtree;

	/**
	 * \brief Pointer to the scenes' entityManager
	 */
	EntityManager* _enM;

	/**
	 * \brief Pointer to the scenes' eventManager
	 */
	EventManager* _evM;
};

class Quadroot
{
public:
	/**
	 * \brief Constructor of the root of the tree
	 * \param entMan Pointer to the entityManager
	 * \param evMan Pointer to the eventManager
	 * \param nw coordinates of the northwest corner
	 * \param ne coordinates of the northeast corner
	 * \param sw coordinates of the southwest corner
	 * \param se coordinates of the southeast corner
	 */
	Quadroot(EntityManager* entMan, EventManager* evMan, glm::vec2 nw = glm::vec2{}, glm::vec2 ne = glm::vec2{}, glm::vec2 sw = glm::vec2{}, glm::vec2 se = glm::vec2{});
	virtual ~Quadroot();

	/**
	* \brief Pushes an entity into the tree, placing it correctly
	* \param ent Handle to entity
	*/
	virtual void pushEnt(EntityHandle ent);

	/**
	 * \brief Places an entity in the quad
	 * \param ent Handle to entity
	 */
	void placeEnt(EntityHandle ent);

	/**
	* \brief Updates all placements of all entities in the tree
	*/
	virtual void update();

	/**
	 * \brief Deletes an entity from the tree, finding it based on pos
	 * \param pos Position of entity within the tree, using 3 bits per depth with '1xx' where xx is which quad below the level it is positioned 
	 * \param ent Handle to entity
	 */
	void delEnt(uint32_t pos, EntityHandle ent);

	/**
	 * \brief Checks whether to collapse the quad and remove its leaves
	 * \return Returns if the tree should be collapsed
	 */
	bool collapseCheck();

	/**
	 * \brief Gets all entities in and below current quad in a vector
	 * \return Vector of all entity handles in and below current quad
	 */
	std::vector<EntityHandle> getAllEntities();

	/**
	 * \brief Checks collision of all inhabitants of current quad and calls collisionCheck for any leaves
	 */
	virtual void collisionCheck();

	/**
	 * \brief Help function of collisionCheck which compares collision with a lower entity and ones of higher levels
	 * \param ent 
	 */
	virtual void collideUp(EntityHandle ent);

	/**
	 * \brief Determines whether ent1 and ent2 has any overlap (collision)
	 * \param ent1 Handle to an entity
	 * \param ent2 Handle to an entity
	 * \return bool value of whether the entities collided
	 */
	bool hasOverlap(EntityHandle ent1, EntityHandle ent2) const;

	/**
	 * \brief Coordinates of northwest corner
	 */
	glm::vec2 _nwCorn{};

	/**
	 * \brief Coordinates of northeast corner
	 */
	glm::vec2 _neCorn{};

	/**
	 * \brief Coordinates of southwest corner
	 */
	glm::vec2 _swCorn{};

	/**
	 * \brief Coordinates of southeast corner
	 */
	glm::vec2 _seCorn{};

	/**
	 * \brief x-width of the quad
	 */
	float _width{};

	/**
	 * \brief y-height of the quad
	 */
	float _height{};

	/**
	 * \brief The position in the quadtree, using 3 bits per depth with bits '1xx' where xx is which quad below the level it is positioned 
	 */
	uint32_t _treePosition{0};

	/**
	 * \brief Indicates at which depth the quad is located
	 */
	uint8_t _depth{0};

	/**
	* \brief Entitycounter
	* \return The number of entities in the current quad
	*/
	uint16_t getEntCount();

	/**
	* \brief Entitycounter
	* \return The total number of entities in and below the current quad
	*/
	uint16_t getTotalEntCount();
protected:
	/**
	 * \brief Checks which leaf the given position is within
	 * \param position coordinates of the point
	 * \return a number 1/2/3/4 with 1=northwest, 2=northeast, 3=southwest, 4=southeast
	 */
	uint8_t whichQuad(glm::vec3 position) const;

	/**
	 * \brief Checks if an entity is within the confines of the quad
	 * \param ent Handle to the entity
	 * \return True if it is within the quad, else false
	 */
	bool isInside(EntityHandle ent);

	/**
	 * \brief Splits the current quad and adds leaves (children) to it
	 */
	void split();
	
	/**
	 * \brief Pointer to entity manager
	 */
	EntityManager* _enM;

	/**
	 * \brief Pointer to event manager
	 */
	EventManager* _evM;

	/**
	 * \brief Vector of all entities in the quad
	 */
	std::vector<EntityHandle> _entities{};

	/**
	 * \brief The number of entities in the quad
	 */
	uint8_t _entCount{ 0 };

	/**
	 * \brief Pointer to northwest leaf (child)
	 */
	Quadleaf* _nw{ nullptr };

	/**
	* \brief Pointer to northeast leaf (child)
	*/
	Quadleaf* _ne{ nullptr };

	/**
	* \brief Pointer to southeast leaf (child)
	*/
	Quadleaf* _se{ nullptr };

	/**
	* \brief Pointer to southwest leaf (child)
	*/
	Quadleaf* _sw{ nullptr };
};

/**
 * \brief Class acting as leaves and subclass of the root
 */
class Quadleaf : public Quadroot
{
public:
	/**
	 * \brief Constructor
	 * \param entMan Pointer to entity manager
	 * \param evMan Pointer to event manager
	 * \param par Pointer to parent of the leaf
	 * \param quad Which quad the leaf is in the parent
	 */
	explicit Quadleaf(EntityManager* entMan, EventManager* evMan, Quadroot* par, uint8_t quad);
	~Quadleaf() = default;

	/**
	 * \brief Tries to move an entity down in the tree
	 * \param ent Handle to an entity
	 * \return Whether or not the down-move was successful
	 */
	bool moveDown(EntityHandle ent);


	/**
	* \brief Pushes an entity into the tree, placing it correctly
	* \param ent Handle to entity
	*/
	void pushEnt(EntityHandle ent) override;

	/**
	 * \brief Updates the positions of the entities and calls update of the children
	 */
	void update() override;


	/**
	 * \brief Called by parent and does collision check, calls the childrens collisionCheck
	 */
	void collisionCheck() override;

	/**
	 * \brief Does collision detection for an entity and the higher leves of the tree
	 * \param ent Handle to entity
	 */
	void collideUp(EntityHandle ent) override;

private:
	/**
	 * \brief Moves an entity up in the tree
	 * \param ent Handle to an entity
	 */
	void moveUp(EntityHandle ent);

	/**
	 * \brief Pointer to the parent of the quad
	 */
	Quadroot* _parent{ nullptr };
};

inline uint16_t Quadroot::getEntCount()
{
	return _entCount;
}
inline uint16_t Quadroot::getTotalEntCount()
{
	if(_sw != nullptr)
	{
		return _nw->getTotalEntCount() + _ne->getTotalEntCount() + _sw->getTotalEntCount() + _se->getTotalEntCount() + _entCount;
	}

	return _entCount;
}
inline uint16_t Quadtree::getEntCount()
{
	return _quadtree->getEntCount();
}
inline uint16_t Quadtree::getTotalEntCount()
{
	return _quadtree->getTotalEntCount();
}

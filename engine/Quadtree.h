#pragma once
#include "EntityManager.h"
#include <vector>
#include "TransformComponent.h"
#include <stdexcept>


//Error class
class Quadtree_error : public std::logic_error {
	using std::logic_error::logic_error;
};
class Quadroot;
class Quadleaf;

class Quadtree
{
public:
	Quadtree(EntityManager* entMan, glm::vec2 position, uint32_t width, uint32_t height);
	~Quadtree();
	void update();
	void pushEntity(EntityHandle ent);

	uint16_t getEntCount();
	uint16_t getTotalEntCount();

private:
	Quadroot* _quadtree;
	EntityManager* _enM;
};

class Quadroot
{
public:
	Quadroot(EntityManager* entMan, glm::vec2 nw = glm::vec2{}, glm::vec2 ne = glm::vec2{}, glm::vec2 sw = glm::vec2{}, glm::vec2 se = glm::vec2{});
	virtual ~Quadroot();

	virtual void pushEnt(EntityHandle ent);
	void placeEnt(EntityHandle ent);
	virtual void update();

	glm::vec2 _nwCorn{};
	glm::vec2 _neCorn{};
	glm::vec2 _swCorn{};
	glm::vec2 _seCorn{};
	float _width{};
	float _height{};
	uint32_t _treePosition{0};

	uint16_t getEntCount();
	uint16_t getTotalEntCount();
protected:
	uint8_t whichQuad(glm::vec3 position) const;
	bool isInside(EntityHandle ent);
	void split();
	

	EntityManager* _enM;

	std::vector<EntityHandle> _entities{};
	uint8_t _entCount{ 0 };
	Quadleaf* _nw{ nullptr };
	Quadleaf* _ne{ nullptr };
	Quadleaf* _se{ nullptr };
	Quadleaf* _sw{ nullptr };
};

class Quadleaf : public Quadroot
{
public:
	explicit Quadleaf(EntityManager* entMan, Quadroot* par, uint8_t quad);
	~Quadleaf() = default;

	bool moveDown(EntityHandle ent);

	void pushEnt(EntityHandle ent) override;

	void update() override;

private:
	void moveUp(EntityHandle ent);
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
#pragma once
#include "EntityManager.h"
#include <vector>
#include "TransformComponent.h"

class Quadroot;
class Quadleaf;

class Quadtree
{
public:
	Quadtree(EntityManager* entMan);
	~Quadtree();

private:
	Quadroot* _quadtree;
	EntityManager* _enM;
};

class Quadroot
{
public:
	Quadroot(EntityManager* entMan, glm::vec2 nw = glm::vec2{}, glm::vec2 ne = glm::vec2{}, glm::vec2 sw = glm::vec2{}, glm::vec2 se = glm::vec2{});
	virtual ~Quadroot() = default;

	virtual void pushEnt(Entity* ent);
	void placeEnt(Entity* ent);

	glm::vec2 _nwCorn{};
	glm::vec2 _neCorn{};
	glm::vec2 _swCorn{};
	glm::vec2 _seCorn{};
	float _width{};
	float _height{};
protected:
	uint8_t whichQuad(glm::vec3 position) const;
	bool isInside(Entity* ent);
	void split();
	virtual void update();

	EntityManager* _enM;

	std::vector<Entity*> _entities{};
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
	~Quadleaf();

	bool moveDown(Entity* ent);

	void pushEnt(Entity* ent) override;

	void update() override;

private:
	void moveUp(Entity* ent);
	Quadroot* _parent{ nullptr };
};


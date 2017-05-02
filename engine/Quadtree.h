#pragma once
#include "EntityManager.h"
#include <vector>

class Quadroot;
class Quadleaf;

class Quadtree
{
public:
	Quadtree();
	~Quadtree();

private:
	Quadroot* quadtree;
};


class Quadroot
{
public:
	Quadroot() = default;
	~Quadroot() = default;
protected:
	std::vector<Entity*> _entities{};
	Quadleaf* _nw{ nullptr };
	Quadleaf* _ne{ nullptr };
	Quadleaf* _se{ nullptr };
	Quadleaf* _sw{ nullptr };
};

class Quadleaf : Quadroot
{
public:
	explicit Quadleaf(Quadroot* par);
	~Quadleaf();
private:
	Quadroot* _parent{ nullptr };
};


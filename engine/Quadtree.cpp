#include "Quadtree.h"



Quadtree::Quadtree()
{
}


Quadtree::~Quadtree()
{
}

Quadleaf::Quadleaf(Quadroot* par) :
	Quadroot(),
	_parent{par}
{
}

Quadleaf::~Quadleaf()
{
	delete _sw;
	delete _se;
	delete _nw;
	delete _ne;
}

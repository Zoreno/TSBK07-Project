#include "Quadtree.h"

#define NW 1;
#define NE 2;
#define SW 3;
#define SE 4;

Quadtree::Quadtree(EntityManager* entMan) :
	_enM{entMan}
{
	_quadtree = new Quadroot(entMan);
}

Quadtree::~Quadtree()
{
	delete _quadtree;
}

Quadroot::Quadroot(EntityManager* entMan) :
	_enM{ entMan }
{
}

Quadleaf::Quadleaf(EntityManager* entMan, Quadroot* par) :
	Quadroot(entMan),
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

bool Quadleaf::moveDown(Entity * ent)
{
	return false;
}

void Quadleaf::pushEnt(Entity * ent)
{
	TransformComponent* tmpTransform = _enM->getComponent<TransformComponent>(ent->getID());
	glm::vec3 tmpPos = tmpTransform->position;

	if (isInside(ent))
	{
		if (_sw == nullptr)
		{
			if (_entCount > 6)
			{
				split();
				uint8_t quad = whichQuad(tmpPos);

				switch (quad)
				{
				case 1:
					_nw->pushEnt(ent);
					break;
				case 2:
					_ne->pushEnt(ent);
					break;
				case 3:
					_sw->pushEnt(ent);
					break;
				case 4:
					_se->pushEnt(ent);
					break;
				default:
					break;;
				}
			}
			else
			{
				placeEnt(ent);
			}
		}
		else
		{
			uint8_t quad = whichQuad(tmpPos);

			switch (quad)
			{
			case 1:
				_nw->pushEnt(ent);
				break;
			case 2:
				_ne->pushEnt(ent);
				break;
			case 3:
				_sw->pushEnt(ent);
				break;
			case 4:
				_se->pushEnt(ent);
				break;
			default:
				break;;
			}
		}
	}
	else
	{
		_parent->placeEnt(ent);
	}
}

void Quadleaf::update()
{
	if (_sw != nullptr)
	{
		_nw->update();
		_ne->update();
		_sw->update();
		_se->update();
	}

	std::vector<Entity*> entsToRemove{};

	for (auto i : _entities)
	{
		glm::vec3 tmpPos = _enM->getComponent<TransformComponent>(i->getID())->position;

		if (isInside(i))
		{
			if (_sw != nullptr)
			{
				uint8_t quad = whichQuad(tmpPos);

				switch (quad)
				{
				case 1:
					if (_nw->moveDown(i))
					{
						entsToRemove.push_back(i);
					}
					break;
				case 2:
					if (_ne->moveDown(i))
					{
						entsToRemove.push_back(i);
					}
					break;
				case 3:
					if (_sw->moveDown(i))
					{
						entsToRemove.push_back(i);
					}
					break;
				case 4:
					if (_se->moveDown(i))
					{
						entsToRemove.push_back(i);
					}
					break;
				default:
					break;
				}
			}
			else
			{
				// gˆr ingenting!
			}
		}
		else
		{
			moveUp(i);
			entsToRemove.push_back(i);
		}
	}

	for (auto i = _entities.begin(); i == _entities.end(); ++i)
	{
		for (auto j = _entities.begin(); j == _entities.end(); ++j)
		{
			if (*i._Ptr == *j._Ptr)
			{
				i = _entities.erase(i);
				--i;
				entsToRemove.erase(j);
				break;
			}
		}
	}
}

void Quadleaf::moveUp(Entity* ent)
{
	_parent->placeEnt(ent);
}

void Quadroot::pushEnt(Entity * ent)
{
	glm::vec3 tmpPos = _enM->getComponent<TransformComponent>(ent->getID())->position;

	if(isInside(ent))
	{
		if (_sw == nullptr)
		{
			if (_entCount > 6)
			{
				split();
				uint8_t quad = whichQuad(tmpPos);

				switch (quad)
				{
				case 1 :
					_nw->pushEnt(ent);
					break;
				case 2 :
					_ne->pushEnt(ent);
					break;
				case 3:
					_sw->pushEnt(ent);
					break;
				case 4:
					_se->pushEnt(ent);
					break;
				default:
					break;;
				}
			}
			else
			{
				placeEnt(ent);
			}
		}
		else
		{
			uint8_t quad = whichQuad(tmpPos);

			switch (quad)
			{
			case 1:
				_nw->pushEnt(ent);
				break;
			case 2:
				_ne->pushEnt(ent);
				break;
			case 3:
				_sw->pushEnt(ent);
				break;
			case 4:
				_se->pushEnt(ent);
				break;
			default:
				break;;
			}
		}
	}
	else
	{
		placeEnt(ent);
	}
}

void Quadroot::placeEnt(Entity * ent)
{
	_entities.push_back(ent);
	_entCount += 1;
}

uint8_t Quadroot::whichQuad(glm::vec3 position) const
{
	if (position.x > (_nwCorn.x + _neCorn.x) / 2)
	{
		if (position.z > (_neCorn.y + _seCorn.y) / 2)
		{
			return NE;
		}
		else
		{
			return SE;
		}
	}
	else
	{
		if (position.z > (_neCorn.y + _seCorn.y) / 2)
		{
			return NW;
		}
		else
		{
			return SW;
		}
	}
	return uint8_t();
}

bool Quadroot::isInside(Entity* ent)
{
	//Kod h‰‰‰‰‰‰‰r. TODO TODO!!
	return false;
}

void Quadroot::split()
{
	if (_sw == nullptr) return;
	_nw = new Quadleaf(_enM, this);
	_ne = new Quadleaf(_enM, this);
	_sw = new Quadleaf(_enM, this);
	_se = new Quadleaf(_enM, this);

	std::vector<Entity*> tmpEntities = _entities;
	_entities.clear();
	for (auto i = tmpEntities.begin(); i == tmpEntities.end(); ++i)
	{
		glm::vec3 tmpPos = _enM->getComponent<TransformComponent>((*i._Ptr)->getID())->position;

		uint8_t quad = whichQuad(tmpPos);
		switch (quad)
		{
		case 1:
			_nw->pushEnt((*i._Ptr));
			break;
		case 2:
			_ne->pushEnt((*i._Ptr));
			break;
		case 3:
			_sw->pushEnt((*i._Ptr));
			break;
		case 4:
			_se->pushEnt((*i._Ptr));
			break;
		default:
			break;;
		}
	}
}

void Quadroot::update()
{
	if(_sw != nullptr)
	{
		_nw->update();
		_ne->update();
		_sw->update();
		_se->update();
	}

	std::vector<Entity*> entsToRemove{};

	for (auto i : _entities)
	{
		glm::vec3 tmpPos = _enM->getComponent<TransformComponent>(i->getID())->position;
		
		if (isInside(i))
		{
			if (_sw != nullptr)
			{
				uint8_t quad = whichQuad(tmpPos);

				switch (quad)
				{
				case 1:
					if (_nw->moveDown(i))
					{
						entsToRemove.push_back(i);
					}
					break;
				case 2:
					if (_ne->moveDown(i))
					{
						entsToRemove.push_back(i);
					}
					break;
				case 3:
					if (_sw->moveDown(i))
					{
						entsToRemove.push_back(i);
					}
					break;
				case 4:
					if (_se->moveDown(i))
					{
						entsToRemove.push_back(i);
					}
					break;
				default:
					break;
				}
			}
			else
			{
				// gˆr ingenting!
			}
		}
		else
		{
			// Kan inte flytta upp den. ƒr i roten. LÂt vara.
		}
	}

	for (auto i = _entities.begin(); i == _entities.end(); ++i)
	{
		for(auto j = _entities.begin(); j == _entities.end(); ++j)
		{
			if(*i._Ptr == *j._Ptr)
			{
				i = _entities.erase(i);
				--i;
				entsToRemove.erase(j);
				break;
			}
		}
	}

}

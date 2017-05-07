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

Quadroot::Quadroot(EntityManager* entMan, glm::vec2 nw, glm::vec2 ne, glm::vec2 sw, glm::vec2 se) :
	_enM{ entMan },
	_nwCorn{ nw },
	_neCorn{ ne },
	_swCorn{ sw },
	_seCorn{ se },
	_width{ std::abs(_neCorn.x - _nwCorn.x)},
	_height{ std::abs(_nwCorn.y - _swCorn.y) }
{
}

Quadleaf::Quadleaf(EntityManager* entMan, Quadroot* par, uint8_t quad) :
	Quadroot(entMan),
	_parent{par}
{
	_width = par->_width / 2;
	_height = par->_height / 2;

	switch(quad)
	{
	case 1:
		_nwCorn = par->_nwCorn;
		_neCorn.x = par->_neCorn.x - _width;
		_neCorn.y = par->_neCorn.y;
		_swCorn.x = par->_swCorn.x;
		_swCorn.y = par->_swCorn.y + _height;
		_seCorn.x = par->_nwCorn.x + _width;
		_seCorn.y = par->_nwCorn.y - _height;
		break;
	case 2:
		_nwCorn.x = par->_nwCorn.x + _width;
		_nwCorn.y = par->_nwCorn.y;
		_neCorn = par->_neCorn;
		_swCorn.x = par->_swCorn.x + _width;
		_swCorn.y = par->_swCorn.y + _height;
		_seCorn.x = par->_nwCorn.x;
		_seCorn.y = par->_seCorn.y + _height;
		break;
	case 3:
		_nwCorn.x = par->_nwCorn.x;
		_nwCorn.y = par->_nwCorn.y - _height;
		_neCorn.x = par->_neCorn.x - _width;
		_neCorn.y = par->_neCorn.y - _height;
		_swCorn = par->_swCorn;
		_seCorn.x = par->_swCorn.x + _width;
		_seCorn.y = par->_seCorn.y;
		break;
	case 4:
		_nwCorn.x = par->_nwCorn.x + _width;
		_nwCorn.y = par->_nwCorn.y - _height;
		_neCorn.x = par->_neCorn.x;
		_neCorn.y = par->_neCorn.y - _height;
		_swCorn.x = par->_swCorn.x + _width;
		_swCorn.y = par->_swCorn.y;
		_seCorn = par->_seCorn;
		break;
	default:
		break;
	}

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
	if (isInside(ent))
	{
		glm::vec3 tmpPos = _enM->getComponent<TransformComponent>(ent->getID())->position;

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
				_nw->moveDown(ent);
				break;
			case 2:
				_ne->moveDown(ent);
				break;
			case 3:
				_sw->moveDown(ent);
				break;
			case 4:
				_se->moveDown(ent);
				break;
			default:
				break;;
			}
		}

		return true;
	}
	else
	{
		return false;
	}
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
				// gör ingenting!
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
	//Kod hääääääär. TODO TODO!!
	glm::vec3 tmpPos = _enM->getComponent<TransformComponent>(ent->getID())->position;

	if(tmpPos.x >= _nwCorn.x && tmpPos.x <= _neCorn.x &&
		tmpPos.z >= _swCorn.y && tmpPos.z <= _nwCorn.y)
	{
		return true;
	}

	return false;
}

void Quadroot::split()
{
	if (_sw == nullptr) return;
	_nw = new Quadleaf(_enM, this, 1);
	_ne = new Quadleaf(_enM, this, 2);
	_sw = new Quadleaf(_enM, this, 3);
	_se = new Quadleaf(_enM, this, 4);

	std::vector<Entity*> tmpEntities = _entities;
	_entities.clear();
	_entCount = 0;

	for (auto i : tmpEntities)
	{
		glm::vec3 tmpPos = _enM->getComponent<TransformComponent>((i)->getID())->position;

		uint8_t quad = whichQuad(tmpPos);
		switch (quad)
		{
		case 1:
			_nw->pushEnt((i));
			break;
		case 2:
			_ne->pushEnt((i));
			break;
		case 3:
			_sw->pushEnt((i));
			break;
		case 4:
			_se->pushEnt((i));
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
				// gör ingenting!
			}
		}
		else
		{
			// Kan inte flytta upp den. Är i roten. Låt vara.
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

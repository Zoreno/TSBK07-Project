#include "Quadtree.h"
#include "CollisionComponent.h"
#include "QuadtreeComponent.h"
#include "CollisionEvent.h"

#define NW 1;
#define NE 2;
#define SW 3;
#define SE 4;

Quadtree::Quadtree(EntityManager* entMan, EventManager* evMan, glm::vec2 pos, uint32_t width, uint32_t height) :
	_enM{entMan},
	_evM{evMan}
{
	_evM->addSubscriber<EntityDestroyedEvent>(this);
	_evM->addSubscriber<ComponentAssignedEvent<TransformComponent>>(this);

	_quadtree = new Quadroot(entMan, evMan, 
		glm::vec2{ pos.x - width / 2,pos.y + height / 2 },
		glm::vec2{ pos.x + width / 2,pos.y + height / 2 },
		glm::vec2{ pos.x - width / 2,pos.y - height / 2 },
		glm::vec2{ pos.x + width / 2,pos.y - height / 2 });
}

Quadtree::~Quadtree()
{
	delete _quadtree;
}

void Quadtree::update()
{
	_quadtree->update();
	_quadtree->collisionCheck();
}

void Quadtree::pushEntity(EntityHandle ent)
{
	if(_enM->hasComponent<QuadtreeComponent>(ent))
	{
		throw Quadtree_error(std::string("Duplicate entity '").append(std::to_string(ent)).append("' in quadtree"));
	}

	_enM->assignComponent<QuadtreeComponent>(ent);

	_quadtree->pushEnt(ent);
}

void Quadtree::handleEvent(const EntityDestroyedEvent& ev)
{
	if (_enM->hasComponent<QuadtreeComponent>(ev.entHandle))
	{
		_quadtree->delEnt(_enM->getComponent<QuadtreeComponent>(ev.entHandle)->getPosition(), ev.entHandle);
	}
}

void Quadtree::handleEvent(const ComponentAssignedEvent<TransformComponent>& ev)
{
	_enM->assignComponent<QuadtreeComponent>(ev.entHandle);
	_quadtree->pushEnt(ev.entHandle);
}

Quadroot::Quadroot(EntityManager* entMan, EventManager* evMan, glm::vec2 nw, glm::vec2 ne, glm::vec2 sw, glm::vec2 se) :
	_enM{ entMan },
	_evM{ evMan },
	_nw{ nullptr },
	_ne{ nullptr },
	_sw{ nullptr },
	_se{ nullptr },
	_nwCorn{ nw },
	_neCorn{ ne },
	_swCorn{ sw },
	_seCorn{ se },
	_width{ std::abs(_neCorn.x - _nwCorn.x)},
	_height{ std::abs(_nwCorn.y - _swCorn.y) },
	_treePosition{0}
{
}

Quadroot::~Quadroot()
{
	delete _sw;
	delete _se;
	delete _nw;
	delete _ne;
}

Quadleaf::Quadleaf(EntityManager* entMan, EventManager* evMan, Quadroot* par, uint8_t quad) :
	Quadroot(entMan, evMan),
	_parent{par}
{
	_treePosition = par->_treePosition*8 + 4 + (quad % 4);

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
		_seCorn.x = par->_neCorn.x;
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

bool Quadleaf::moveDown(EntityHandle ent)
{
	if (isInside(ent))
	{
		glm::vec3 tmpPos = _enM->getComponent<TransformComponent>(ent)->position;

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

		return true;
	}
	else
	{
		return false;
	}
}

void Quadleaf::pushEnt(EntityHandle ent)
{
	TransformComponent* tmpTransform = _enM->getComponent<TransformComponent>(ent);
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
	collapseCheck();

	if (_sw != nullptr)
	{
		_nw->update();
		_ne->update();
		_sw->update();
		_se->update();
	}

	std::vector<EntityHandle> entsToRemove{};

	for (auto i : _entities)
	{
		glm::vec3 tmpPos = _enM->getComponent<TransformComponent>(i)->position;

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

	for (auto i : entsToRemove)
	{
		for (auto j = _entities.begin(); j == _entities.end(); ++j)
		{
			if (i == *j)
			{
				_entities.erase(j);
				_entCount -= 1;
				break;
			}
		}
	}
}

void Quadleaf::collisionCheck()
{
	if (_sw != nullptr)
	{
		_nw->collisionCheck();
		_ne->collisionCheck();
		_sw->collisionCheck();
		_se->collisionCheck();
	}

	for (auto i = _entities.begin(); i != _entities.end(); ++i)
	{
		if (!_enM->hasComponent<CollisionComponent>(*i))
		{
			continue;
		}
		for (auto j = i + 1; j != _entities.end(); ++j)
		{
			if (!_enM->hasComponent<CollisionComponent>(*j))
			{
				continue;
			}

			if (hasOverlap(*i, *j))
			{
				_evM->postEvent(CollisionEvent(*i, *j));
			}
		}
		_parent->collideUp(*i);
	}
}

void Quadleaf::collideUp(EntityHandle ent)
{
	for (auto j = _entities.begin(); j != _entities.end(); ++j)
	{
		if (!_enM->hasComponent<CollisionComponent>(*j))
		{
			continue;
		}

		if (hasOverlap(ent, *j))
		{
			_evM->postEvent(CollisionEvent(ent, *j));
		}
	}

	_parent->collideUp(ent);
}

void Quadleaf::moveUp(EntityHandle ent)
{
	_parent->placeEnt(ent);
}

void Quadroot::pushEnt(EntityHandle ent)
{
	glm::vec3 tmpPos = _enM->getComponent<TransformComponent>(ent)->position;

	if(isInside(ent))
	{
		uint8_t quad = whichQuad(tmpPos);
		if (_sw == nullptr)
		{
			if (_entCount > 6)
			{
				split();
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

void Quadroot::placeEnt(EntityHandle ent)
{
	_entities.push_back(ent);
	_enM->getComponent<QuadtreeComponent>(ent)->setPosition(_treePosition);
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

bool Quadroot::isInside(EntityHandle ent)
{
	//Kod hääääääär. TODO TODO!!
	glm::vec3 tmpPos = _enM->getComponent<TransformComponent>(ent)->position;

	if(_enM->hasComponent<CollisionComponent>(ent))
	{
		float reach = _enM->getComponent<CollisionComponent>(ent)->getReach();
		//std::cout << _nwCorn.x << ", " << _nwCorn.y << "; " 
			//<< _neCorn.x << ", " << _neCorn.y << "; " 
			//<< _swCorn.x << ", " << _swCorn.y << "; " 
			//<< _seCorn.x << ", " << _seCorn.y << std::endl;
		if ((tmpPos.x - reach) >= _nwCorn.x && (tmpPos.x + reach) <= _neCorn.x &&
			(tmpPos.z - reach) >= _swCorn.y && (tmpPos.z + reach) <= _nwCorn.y)
		{
			return true;
		}
		return false;
	}
		
	if (tmpPos.x >= _nwCorn.x && tmpPos.x <= _neCorn.x &&
		tmpPos.z >= _swCorn.y && tmpPos.z <= _nwCorn.y)
	{
		return true;
	}
	return false;

}

void Quadroot::split()
{
	std::cout << "split!" << std::endl;

	if (_sw != nullptr) return;
	_nw = new Quadleaf(_enM, _evM, this, 1);
	_ne = new Quadleaf(_enM, _evM, this, 2);
	_sw = new Quadleaf(_enM, _evM, this, 3);
	_se = new Quadleaf(_enM, _evM, this, 4);

	std::vector<EntityHandle> tmpEntities = _entities;
	_entities.clear();
	_entCount = 0;

	for (auto i : tmpEntities)
	{
		glm::vec3 tmpPos = _enM->getComponent<TransformComponent>(i)->position;

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
	collapseCheck();

	if(_sw != nullptr)
	{
		_nw->update();
		_ne->update();
		_sw->update();
		_se->update();
	}

	std::vector<EntityHandle> entsToRemove{};

	for (auto i : _entities)
	{
		glm::vec3 tmpPos = _enM->getComponent<TransformComponent>(i)->position;
		
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
	std::cout << getEntCount() << std::endl;
	for (auto i : entsToRemove)
	{
		for(auto j = _entities.begin(); j == _entities.end(); ++j)
		{
			if(i == *j)
			{
				_entities.erase(j);
				_entCount -= 1;
				break;
			}
		}
	}
}

void Quadroot::delEnt(uint32_t pos, EntityHandle ent)
{
	if(pos == 0)
	{
		for (auto i = _entities.begin(); i != _entities.end(); ++i)
		{
			if (*i == ent)
			{
				_entities.erase(i);
				_entCount -= 1;
				return;
			}
		}

		throw Quadtree_error("Entity to be deleted not found in given quad position");
	}

	if (pos & 0b11 != 0 && _sw == nullptr) std::cout << "Illa illa, här ska vi inte vara!" << std::endl;

	switch(pos & 0b11) // directions % 4, so se = 0, nw = 1, ne = 2, sw = 3
	{
	case 0:
		_se->delEnt(pos >> 3, ent);
		break;
	case 1:
		_nw->delEnt(pos >> 3, ent);
		break;
	case 2:
		_ne->delEnt(pos >> 3, ent);
		break;
	case 3:
		_sw->delEnt(pos >> 3, ent);
		break;
	default:
		throw Quadtree_error("delEnt in quad other than 0,1,2,3. Should never happen");
		break;
	}
}

bool Quadroot::collapseCheck()
{
	if (_sw == nullptr) return false;

	if(getTotalEntCount() <= 3)
	{
		_entities = getAllEntities();
		for (auto i : _entities)
		{
			_enM->getComponent<QuadtreeComponent>(i)->setPosition(_treePosition);
		}
		_entCount = _entities.size();
		delete _nw;
		delete _ne;
		delete _sw;
		delete _se;
		_nw = nullptr;
		_ne = nullptr;
		_sw = nullptr;
		_se = nullptr;
		std::cout << "collapsing" << std::endl;
		return true;
	}
	return false;
}

std::vector<EntityHandle> Quadroot::getAllEntities()
{
	std::vector<EntityHandle> tmpVec = _entities;

	if (_sw == nullptr)
		return tmpVec;
	else
	{
		std::vector<EntityHandle> nwVec = _nw->getAllEntities();
		tmpVec.insert(tmpVec.end(), nwVec.begin(), nwVec.end());
		std::vector<EntityHandle> neVec = _ne->getAllEntities();
		tmpVec.insert(tmpVec.end(), neVec.begin(), neVec.end());
		std::vector<EntityHandle> swVec = _sw->getAllEntities();
		tmpVec.insert(tmpVec.end(), swVec.begin(), swVec.end());
		std::vector<EntityHandle> seVec = _se->getAllEntities();
		tmpVec.insert(tmpVec.end(), seVec.begin(), seVec.end());

		return tmpVec;
	}
}

void Quadroot::collisionCheck()
{
	if (_sw != nullptr)
	{
		_nw->collisionCheck();
		_ne->collisionCheck();
		_sw->collisionCheck();
		_se->collisionCheck();
	}
	int counter = 0;
	for (auto i = _entities.begin(); i != _entities.end(); ++i)
	{
		if(!_enM->hasComponent<CollisionComponent>(*i))
		{
			continue;
		}
		for (auto j = i + 1; j != _entities.end(); ++j)
		{
			if (!_enM->hasComponent<CollisionComponent>(*j))
			{
				continue;
			}

			if(hasOverlap(*i, *j))
			{
				++counter;
				if (counter % 100 == 0) std::cout << "another 100 collisions" << std::endl;
				_evM->postEvent(CollisionEvent(*i, *j));
			}
		}
	}
}

void Quadroot::collideUp(EntityHandle ent)
{
	for (auto j = _entities.begin(); j != _entities.end(); ++j)
	{
		if (!_enM->hasComponent<CollisionComponent>(*j))
		{
			continue;
		}

		if (hasOverlap(ent, *j))
		{
			_evM->postEvent(CollisionEvent(ent, *j));
		}
	}
}

bool Quadroot::hasOverlap(EntityHandle ent1, EntityHandle ent2) const
{
	glm::vec3 pos1 = _enM->getComponent<TransformComponent>(ent1)->position;
	float reach1 = _enM->getComponent<CollisionComponent>(ent1)->getReach();

	glm::vec3 pos2 = _enM->getComponent<TransformComponent>(ent2)->position;
	float reach2 = _enM->getComponent<CollisionComponent>(ent2)->getReach();

	if (pos1.x - reach1 < pos2.x + reach2 &&
		pos1.x + reach1 > pos2.x - reach2 &&
		pos1.z - reach1 < pos2.z + reach2 &&
		pos1.z + reach1 > pos2.z - reach2) 
	{
		return true;
	}

	return false;
}



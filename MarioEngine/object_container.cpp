#include "object_container.h"
#include <set>
using namespace mir;
using namespace std;
using namespace sf;

void StaticObjectGroup::_clean() {
	if (trashBag.size() == 0) return;
	auto iter = trashBag.begin();
	while (iter != trashBag.end()) {
		auto titer = objects.begin();
		while (titer != objects.end()) {
			if (*iter == *titer) {
				objects.erase(iter);
				objects.erase(titer);
				_clean();
				return;
			}
			titer++;
		}
		iter++;
	}
	trashBag.clear();
}

bool StaticObjectGroup::checkPt(Object * unit) {
	bool result = (unit != nullptr);
	if (!result) trashBag.push_back(unit);
	return result;
}

void mir::StaticObjectGroup::toTrashBag(Object * obj) {
	trashBag.push_back(obj);
}

StaticObjectGroup::StaticObjectGroup() {

}

mir::StaticObjectGroup::StaticObjectGroup(ObjectContainer & cont, bool link) {
	pushContainer(cont, link);
}

void mir::StaticObjectGroup::push(Object & obj, bool linkGroup) {
	if (linkGroup) obj.setGroup(this);
	objects.push_back(&obj);
}


bool StaticObjectGroup::erase(Object& unit) {
	_clean();
	auto iter = objects.begin();
	while (iter != objects.end()) {
		if (&unit == *iter) {
			toTrashBag(*iter); //objects.erase(iter);
			return true;
		}
		iter++;
	}
	return false;
}

void mir::StaticObjectGroup::push(Object * obj, bool linkGroup) {
	push(*obj, linkGroup);
}

bool mir::StaticObjectGroup::erase(Object * unit) {
	return  erase(*unit);
}

void StaticObjectGroup::clear() {
	objects.clear();
	trashBag.clear();
}

int mir::StaticObjectGroup::getSize() {
	return objects.size();
}

list<Object*>::iterator StaticObjectGroup::begin() {
	_clean();
	return objects.begin();
}

list<Object*>::iterator StaticObjectGroup::end() {
	_clean();
	return objects.end();
}

void StaticObjectGroup::iterateAll(const TimeStamp& time) {
	_clean();
	auto iter = objects.begin();
	while (iter != objects.end()) {
		if (!checkPt(*iter)) continue;
		(**iter++).iterate(time);
	}
}

void mir::StaticObjectGroup::drawAll(Engine & window) {
	set<int> depthList;
	int curDepth;
	foreach([&depthList](Object& obj) {
		depthList.insert(obj.getDepth());
	});
	auto iter = depthList.begin();
	while (iter != depthList.end()) {
		curDepth = *iter;
		foreach([&curDepth, &window](Object& obj) {
			if (curDepth == obj.getDepth()) {
				window.draw(obj);
			}
		});
		iter++;
	}
}


void StaticObjectGroup::foreach(void_obj_func func) {
	_clean();
	auto iter = begin();
	while (iter != end()) {
		if (!checkPt(*iter)) continue;
		func(**iter);
		iter++;
	}
}

list<Object*> StaticObjectGroup::find(bool_obj_func func) {
	_clean();
	list<Object*> find_objects;
	auto iter = begin();
	while (iter != end()) {
		if (!checkPt(*iter)) continue;
		if (func(**iter)) find_objects.push_back(*iter);
		iter++;
	}
	return find_objects;
}

unsigned StaticObjectGroup::iterate(const TimeStamp& time) {
	_clean();
	iterateAll(time);
	return true;
}

void mir::StaticObjectGroup::pushContainer(ObjectContainer & cont, bool link) {
	auto iter = cont.begin();
	while (iter != cont.end()) {
		push(*iter++, link);
	}
}

mir::ObjectContainer::ObjectContainer() {
	//Nothing too :)
}

Object* mir::ObjectContainer::push(const Object & obj) {
	objs.push_back(obj);
	return &*(objs.rbegin());
}

void mir::ObjectContainer::erase(list<Object>::iterator& iterator) {
	objs.erase(iterator);
}

void mir::ObjectContainer::clear() {
	objs.clear();
}

list<Object>::iterator mir::ObjectContainer::begin() {
	return objs.begin();
}

list<Object>::iterator mir::ObjectContainer::end() {
	return objs.end();
}

int mir::ObjectContainer::getSize() {
	return objs.size();
}

void mir::ObjectContainer::iterateAll(const TimeStamp & time) {
	auto iter = objs.begin();
	while (iter != objs.end()) {
		(*iter++).iterate(time);
	}
}

void mir::ObjectContainer::drawAll(Engine & window) {
	set<int> depthList;
	int curDepth;
	foreach([&depthList](Object& obj) {
		depthList.insert(obj.getDepth());
	});
	auto iter = depthList.begin();
	while (iter != depthList.end()) {
		curDepth = *iter;
		foreach([&curDepth, &window](Object& obj) {
			if (curDepth == obj.getDepth()) {
				window.draw(obj);
			}
		});
		iter++;
	}
}

void mir::ObjectContainer::foreach(void_obj_func func) {
	auto iter = begin();
	while (iter != end()) {
		func(*iter);
		iter++;
	}
}

list<Object*> mir::ObjectContainer::find(bool_obj_func func) {
	list<Object*> find_objects;
	auto iter = begin();
	while (iter != end()) {
		if (func(*iter)) find_objects.push_back(&*iter);
		iter++;
	}
	return find_objects;
}

unsigned mir::ObjectContainer::iterate(const TimeStamp & time) {
	iterateAll(time);
	return true;
}

void mir::DynamicObjectGroup::_clean() {
	if (trashBag.size() == 0) return;
	auto iter = trashBag.begin();
	while (iter != trashBag.end()) {
		auto titer = objects.begin();
		while (titer != objects.end()) {
			if (*iter == *titer) {
				objects.erase(titer);
				delete *iter;
				_clean();
				return;
			}
			titer++;
		}
		iter++;
	}
	trashBag.clear();
}

bool mir::DynamicObjectGroup::checkPt(Object * unit) {
	bool result = (unit != nullptr);
	if (!result) trashBag.push_back(unit);
	return result;
}

void mir::DynamicObjectGroup::toTrashBag(Object * obj) {
	trashBag.push_back(obj);
}

mir::DynamicObjectGroup::DynamicObjectGroup() {
	//nothing
}


void mir::DynamicObjectGroup::push(Object & obj, bool linkGroup) {
	if (linkGroup) obj.setGroup(this);
	objects.push_back(&obj);
}

bool mir::DynamicObjectGroup::erase(Object & unit) {
	auto iter = objects.begin();
	while (iter != objects.end()) {
		if (&unit == *iter) {
			toTrashBag(*iter);
			return true;
		}
		iter++;
	}
	return false;
}

void mir::DynamicObjectGroup::push(Object * obj, bool linkGroup) {
	push(*obj, linkGroup);
}

bool mir::DynamicObjectGroup::erase(Object * unit) {
	return erase(*unit);
}

void mir::DynamicObjectGroup::clear() {
	objects.clear();
	trashBag.clear();
}

int mir::DynamicObjectGroup::getSize() {
	return objects.size();
}

list<Object*>::iterator mir::DynamicObjectGroup::begin() {
	return objects.begin();
}

list<Object*>::iterator mir::DynamicObjectGroup::end() {
	return objects.end();
}

void mir::DynamicObjectGroup::iterateAll(const TimeStamp & time) {
	_clean();
	auto iter = objects.begin();
	while (iter != objects.end()) {
		if (!checkPt(*iter)) continue;
		if ((**iter).iterate(time) == PROC_DESTRUCT)
			toTrashBag(*iter);
		iter++;
	}
}

void mir::DynamicObjectGroup::drawAll(Engine & window) {
	set<int> depthList;
	int curDepth;
	foreach([&depthList](Object& obj) {
		depthList.insert(obj.getDepth());
	});
	auto iter = depthList.begin();
	while (iter != depthList.end()) {
		curDepth = *iter;
		foreach([&curDepth, &window](Object& obj) {
			if (curDepth == obj.getDepth()) {
				window.draw(obj);
			}
		});
		iter++;
	}
}

void mir::DynamicObjectGroup::drawAll() {
	if (getWindow()) {
		drawAll(*getEngine());
	}
}

void mir::DynamicObjectGroup::foreach(void_obj_func func) {
	auto iter = begin();
	while (iter != end()) {
		if (!checkPt(*iter)) continue;
		func(**iter);
		iter++;
	}
}

list<Object*> mir::DynamicObjectGroup::find(bool_obj_func func) {
	list<Object*> find_objects;
	auto iter = begin();
	while (iter != end()) {
		if (!checkPt(*iter)) continue;
		if (func(**iter)) find_objects.push_back(*iter);
		iter++;
	}
	return find_objects;
}

void mir::DynamicObjectGroup::findToRemove(bool_obj_func func) {
	auto toremove = find(func);
	auto iter = toremove.begin();
	while (iter != toremove.end()) {
		toTrashBag(*iter);
		iter++;
	}
}

unsigned mir::DynamicObjectGroup::iterate(const TimeStamp & time) {
	_clean();
	iterateAll(time);
	return true;
}

mir::DynamicObjectGroup::~DynamicObjectGroup() {
	auto iter = begin();
	while (iter != end()) {
		delete *iter;
		iter++;
	}
}

mir::Group::Group() {
	static int ids = 0;
	id = ids++;
	window = nullptr;
	level = nullptr;
}

void mir::Group::setEngine(Engine* window) {
	this->window = window;
}

Engine * mir::Group::getEngine() {
	return window;
}

RenderWindow * mir::Group::getWindow() {
	if (!window) return nullptr;
	return &window->getWindow();
}

void mir::Group::setId(int id) {
	this->id = id;
}

int mir::Group::getId() {
	return id;
}

void mir::Group::setLevel(Level * lvl) {
	level = lvl;
}

Level * mir::Group::getLevel() {
	return level;
}

void mir::GroupManager::push(ObjectGroup * group) {
	groups.push_back(group);
}

bool mir::GroupManager::erase(ObjectGroup * group) {
	auto iter = groups.begin();
	while (iter != groups.end()) {
		if (group == *iter) {
			groups.erase(iter);
			return true;
		}
		iter++;
	}
	return false;
}

void mir::GroupManager::clear() {
	groups.clear();
}

int mir::GroupManager::getSize() {
	return groups.size();
}

ObjectGroup * mir::GroupManager::getById(int id) {
	auto iter = groups.begin();
	while (iter != groups.end()) {
		if (id == (**iter).getId()) {
			return *iter;
		}
		iter++;
	}
	return nullptr;
}

list<ObjectGroup*>::iterator mir::GroupManager::begin() {
	return groups.begin();
}

list<ObjectGroup*>::iterator mir::GroupManager::end() {
	return groups.end();
}

void mir::GroupManager::iterateAll(const TimeStamp & time) {
	auto iter = groups.begin();
	while (iter != groups.end()) {
		(**iter).iterateAll(time);
		iter++;
	}
}

void mir::GroupManager::iterate(int id, const TimeStamp& time) {
	auto iter = groups.begin();
	while (iter != groups.end()) {
		if (id == (**iter).getId()) {
			(**iter).iterateAll(time);
			iter++;
		}
	}
}

void mir::GroupManager::drawAll(Engine & window) {
	auto iter = groups.begin();
	while (iter != groups.end()) {
		(**iter).drawAll(window);
		iter++;
	}
}

void mir::GroupManager::drawAll() {
	auto iter = groups.begin();
	while (iter != groups.end()) {
		(**iter).drawAll();
		iter++;
	}
}

void mir::GroupManager::draw(int id, Engine & window) {
	auto iter = groups.begin();
	while (iter != groups.end()) {
		if (id == (**iter).getId()) {
			(**iter).drawAll(window);
			iter++;
		}
	}
}

void mir::GroupManager::draw(int id) {
	auto iter = groups.begin();
	while (iter != groups.end()) {
		if (id == (**iter).getId()) {
			(**iter).drawAll();
			iter++;
		}
	}
}

void mir::GroupManager::foreach(int id, void_obj_func func) {
	auto iter = groups.begin();
	while (iter != groups.end()) {
		if (id == (**iter).getId()) {
			(**iter).foreach(func);
			iter++;
		}
	}
}

void mir::GroupManager::foreach(void_obj_func func) {
	auto iter = groups.begin();
	while (iter != groups.end()) {
		(**iter).foreach(func);
		iter++;
	}
}

void mir::GroupManager::findToRemove(bool_obj_func func) {
	auto iter = groups.begin();
	while (iter != groups.end()) {
		(**iter).findToRemove(func);
		iter++;
	}
}

list<Object*> mir::GroupManager::find(bool_obj_func func) {
	list<Object*> find_objects;
	auto iter = groups.begin();
	while (iter != groups.end()) {
		list<Object*> result = (**iter).find(func);
		find_objects.insert(result.begin(),result.begin(), result.end());
		result.clear();
		iter++;
	}
	return find_objects;
}

list<Object*> mir::GroupManager::find(int id, bool_obj_func func) {
	list<Object*> find_objects;
	auto iter = groups.begin();
	while (iter != groups.end()) {
		if (id != (**iter).getId()) continue;
		list<Object*> result = (**iter).find(func);
		find_objects.insert(result.begin(), result.begin(), result.end());
		result.clear();
		iter++;
	}
	return find_objects;
}

list<ObjectGroup*> mir::GroupManager::getGroupsById(int id) {
	list<ObjectGroup*> find_objects;
	auto iter = groups.begin();
	while (iter != groups.end()) {
		if (id != (**iter).getId()) continue;
		ObjectGroup* grp = *iter;
		find_objects.push_back(grp);
		iter++;
	}
	return find_objects;
}

unsigned mir::GroupManager::iterate(const TimeStamp & time) {
	auto iter = groups.begin();
	while (iter != groups.end()) {
		(**iter).iterate(time);
		iter++;
	}
	return PROC_VALID;
}

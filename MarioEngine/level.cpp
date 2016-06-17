#include "level.h"

using namespace mir;

void mir::Level::setEngine(Engine* engine) {
	this->engine = engine;
	globalGroup.setEngine(engine);
}

Engine * mir::Level::getEngine() {
	return engine;
}

GroupManager * mir::Level::getGlobalGroup() {
	return &globalGroup;
}

ObjectGroup * mir::Level::getGroup(int id) {
	auto result = globalGroup.getGroupsById(id);
	return *result.begin();
}

void mir::Level::pushGroup(ObjectGroup * group, int id) {
	if (id >= 0)
		group->setId(id);
	globalGroup.push(group);
}

void mir::Level::removeGroup(int id) {
	auto group = getGroup(id);
	globalGroup.erase(group);
}

void mir::Level::setViewSize(int w, int h) {
	viewSize.x = w;
	viewSize.y = h;
}

Vector2i mir::Level::getViewSize() {
	return viewSize;
}

void mir::Level::viewMove(Vector2f vec) {

	if (levelSize.x < viewPos.x + vec.x + viewSize.x) {
		_setViewPosition(Vector2f(levelSize.x - viewSize.x, viewPos.y));
	} else if (0 > viewPos.x + vec.x) {
		_setViewPosition(Vector2f(0, viewPos.y));
	} else {
		getEngine()->moveView(vec.x, 0);
		viewPos.x += vec.x;
	}

	if (levelSize.y < viewPos.y + vec.y + viewSize.y) {
		_setViewPosition(Vector2f(viewPos.x, levelSize.y - viewSize.y));
	} else if (0 > viewPos.y + vec.y) {
		_setViewPosition(Vector2f(viewPos.x, 0));
	} else {
		getEngine()->moveView(0, vec.y);
		viewPos.y += vec.y;
	}
}

void mir::Level::setViewPosition(Vector2f vec) {
	Vector2f pos = viewPos;

	if (levelSize.x < vec.x + viewSize.x) {
		_setViewPosition(Vector2f(levelSize.x - viewSize.x, viewPos.y));
	} else if (0 > viewPos.x + vec.x) {
		_setViewPosition(Vector2f(0, viewPos.y));
	} else {
		_setViewPosition(Vector2f(vec.x, viewPos.y));
	}

	if (levelSize.y <  vec.y + viewSize.y) {
		_setViewPosition(Vector2f(viewPos.x, levelSize.y - viewSize.y));
	} else if (0 > viewPos.y + vec.y) {
		_setViewPosition(Vector2f(viewPos.x, 0));
	} else {
		_setViewPosition(Vector2f(viewPos.x, vec.y));
	}
}

Vector2f mir::Level::getViewPos() {
	return viewPos;
}

void mir::Level::_setViewPosition(Vector2f vec) {
	viewPos = Vector2f(vec.x, vec.y);
	getEngine()->setViewCenter(vec.x + viewSize.x / 2, vec.y + viewSize.y / 2);
}


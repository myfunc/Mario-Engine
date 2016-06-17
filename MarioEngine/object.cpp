#include "object.h"
#include <functional>
#include <algorithm>
#include "geometric_math.h"
using namespace std;
using namespace sf;
using namespace mir;

void mir::Object::snapToFrame(int w, int h) {
	int height = getTextureSize().y;
	int width = getTextureSize().x;
	for (int i = 0; i < height/h; i++) {
		for (int j = 0; j < width/w; j++) {
			pushFrame(Frame(j*w, i*h, w, h));
		}
	}
}

void mir::Object::pushFrame(const Frame & frame) {
	frames.push(frame);
}

void mir::Object::setFrameIndex(int index) {
	frameIndex = index;
	if (index >= 0 && index < frames.getSize()) {
		_setTextureRect(frames.getFrame(index));
	} else if (index == FullFrame) {
		_setTextureRect(Frame(0, 0, getTextureSize().x, getTextureSize().y));
		frameIndex = FullFrame;
	} else {
		setTextureRect(Frame(0, 0, 0, 0));
		frameIndex = FailedFrame;
	}
}

int mir::Object::getFrameSize() {
	return frames.getSize();
}

int mir::Object::getFrameIndex() {
	return frameIndex;
}

void mir::Object::clearFrames() {
	frames.clear();
}

const mir::FrameContainer& mir::Object::getFrames() {
	return frames;
}

void mir::Object::loadFrames(const FrameContainer & frames) {
	this->frames = frames;
}

Frame mir::Object::getFrame(int index) {
	if (index == FullFrame) return getFullFrame();
	if (index == UnknownFrame) return getTextureRect();
	return frames.getFrame(index);
}

void mir::Object::setFrame(int index, const Frame & frame) {
	frames.setFrame(index, frame);
}

bool mir::Object::isValidFrame() {
	return !(frameIndex == UnknownFrame);
}

Frame mir::Object::getFullFrame() {
	return Frame(0, 0, getTextureSize().x, getTextureSize().y);
}

Frame mir::Object::getCurrentFrame() {
	return getFrame(getFrameIndex());
}

void mir::Object::reflectFrame() {
	Frame rect = getTextureRect();
	rect.left += rect.width;
	rect.width = -rect.width;
	setTextureRect(rect);
}

void mir::Object::setTextureRect(const Frame & frame) {
	Sprite::setTextureRect(frame);
	frameIndex = UnknownFrame;
}

void mir::Object::start() {
	if (anim)
		anim->start();
}

void mir::Object::pause() {
	if (anim)
		anim->pause();
}

void mir::Object::reload() {
	if (anim)
		anim->reload();
}

void mir::Object::setAnimation(Animation * anim) {
	this->anim = anim;
}

void mir::Object::setAnimation(Animation & anim) {
	this->anim = &anim;
}

void mir::Object::removeAnimation() {
	anim = nullptr;
}

void mir::Object::animate() {
	if (anim)
		setFrameIndex(anim->frame());
}

Frame mir::Object::getMask() {
	Frame mask = getTextureRect();//getCurrentFrame();
	Vector2f scale = getScale();
	mask.width = abs(mask.width);
	mask.height = abs(mask.height);
	mask.width *= scale.x;
	mask.height *= scale.y;
	return mask;
}

FloatRect mir::Object::getMaskedFrame() {
	Frame intmask = getTextureRect();//getCurrentFrame();
	FloatRect floatmask(intmask.left, intmask.top, intmask.width, intmask.height);
	Vector2f scale = getScale();
	Vector2f pos = getPosition();
	floatmask.left = pos.x;
	floatmask.top = pos.y;
	floatmask.width = abs(floatmask.width);
	floatmask.height = abs(floatmask.height);
	floatmask.width *= scale.x;
	floatmask.height *= scale.y;
	return floatmask;
}
void mir::Object::instance_destroy() {
	if (getGroup()) {
		getGroup()->findToRemove([this](Object& obj) {
			return (this == &obj);
		});
	}
}

void mir::Object::instance_create(Object * obj) {
	int curID = grp->getId();
	grp->getLevel()->getGroup(curID)->push(obj);
}

// Тут столкновение проиходит только с одним
// объектом. С множеством могут быть проблемы
// Медленный вариант функции, он ищет по всей группе
mir::Object* mir::Object::isMeeting(float x, float y) {
	if (!grp) return false;
	Object* result = nullptr;
	FloatRect mask = getMaskedFrame();
	mask.left = x;
	mask.top = y;
	float radius = MathUtil::getOutCircleRadius(mask);
	Vector2f center = MathUtil::getRectCenter(mask);
	grp->foreach([this, &mask, &result, &radius, &center](Object& obj) {
		if (&obj == this) return;
		if (!obj.isSolid()) return;
		FloatRect omask = obj.getMaskedFrame();
		float oradius = MathUtil::getOutCircleRadius(omask);
		Vector2f ocenter = MathUtil::getRectCenter(omask);
		float distance = MathUtil::getDistance(center, ocenter);
		if (distance > (radius + oradius)) return;
		if (mask.intersects(omask)) result = &obj;
	});
	return result;
}
// Медленный вариант функции, он ищет по всей группе
mir::Object * mir::Object::isMeetingIntersect(float x, float y, FloatRect& intersection) {
	if (!grp) return false;
	Object* result = nullptr;
	FloatRect intersectResult;
	FloatRect mask = getMaskedFrame();
	mask.left = x;
	mask.top = y;
	float radius = MathUtil::getOutCircleRadius(mask);
	Vector2f center = MathUtil::getRectCenter(mask);
	grp->foreach([this, &mask, &result, &intersectResult, &intersection, &radius, &center](Object& obj) {
		if (&obj == this) return;												
		if (!obj.isSolid()) return;
		FloatRect omask = obj.getMaskedFrame();
		float oradius = MathUtil::getOutCircleRadius(omask);
		Vector2f ocenter = MathUtil::getRectCenter(omask);
		float distance = MathUtil::getDistance(center, ocenter);
		if (distance > (radius + oradius)) return;
		if (mask.intersects(omask, intersectResult)) {
			result = &obj;
			intersection = intersectResult;
		}
	});
	return result;
}

void mir::Object::updateColliders(float range) {
	collObjects.clear();
	if (!grp) return;
	FloatRect mask = getMaskedFrame();
	Vector2f coords = getPosition();
	mask.left = coords.x;
	mask.top = coords.y;
	float radius = MathUtil::getOutCircleRadius(mask);
	Vector2f center = MathUtil::getRectCenter(mask);
	auto &collobjs = collObjects;
	grp->foreach([this, &mask, &radius, &center, &range, &collobjs](Object& obj) {
		if (&obj == this) return;
		if (!obj.isSolid()) return;
		FloatRect omask = obj.getMaskedFrame();
		float oradius = MathUtil::getOutCircleRadius(omask);
		Vector2f ocenter = MathUtil::getRectCenter(omask);
		float distance = MathUtil::getDistance(center, ocenter);
		if (range == -1) range = (radius + oradius + 1);
		if (distance < range) {
			collobjs.push_back(&obj);
		}
	});
}

Object * mir::Object::isCollide(float x, float y) {
	if (!grp) return false;
	Object* result = nullptr;
	FloatRect mask = getMaskedFrame();
	mask.left = x;
	mask.top = y;
	float radius = MathUtil::getOutCircleRadius(mask);
	Vector2f center = MathUtil::getRectCenter(mask);
	auto &collobjs = collObjects;
	for_each(collobjs.begin(), collobjs.end(),[this, &mask, &result, &radius, &center](Object* obj) {
		if (obj == this) return;
		if (!obj->isSolid()) return;
		FloatRect omask = obj->getMaskedFrame();
		float oradius = MathUtil::getOutCircleRadius(omask);
		Vector2f ocenter = MathUtil::getRectCenter(omask);
		float distance = MathUtil::getDistance(center, ocenter);
		if (distance > (radius + oradius)) return;
		if (mask.intersects(omask)) result = obj;
	});
	return result;
}

Object * mir::Object::isCollideIntersect(float x, float y, FloatRect & intersection) {
	if (!grp) return false;
	Object* result = nullptr;
	FloatRect intersectResult;
	FloatRect mask = getMaskedFrame();
	mask.left = x;
	mask.top = y;
	float radius = MathUtil::getOutCircleRadius(mask);
	Vector2f center = MathUtil::getRectCenter(mask);
	auto &collobjs = collObjects;
	for_each(collobjs.begin(), collobjs.end(),[this, &mask, &result, 
			 &intersectResult, &intersection, &radius, &center](Object* obj) {
		if (obj == this) return;
		if (!obj->isSolid()) return;
		FloatRect omask = obj->getMaskedFrame();
		float oradius = MathUtil::getOutCircleRadius(omask);
		Vector2f ocenter = MathUtil::getRectCenter(omask);
		float distance = MathUtil::getDistance(center, ocenter);
		if (distance > (radius + oradius)) return;
		if (mask.intersects(omask, intersectResult)) {
			result = obj;
			intersection = intersectResult;
		}
	});
	return result;
}

void mir::Object::_setTextureRect(const Frame & frame) {
	Sprite::setTextureRect(frame);
}

void mir::FrameContainer::push(const Frame & frame) {
	frames.push_back(frame);
}

int mir::FrameContainer::getSize() {
	return frames.size();
}

void mir::FrameContainer::clear() {
	frames.clear();
}

void mir::FrameContainer::loadFrames(const FrameContainer & frames) {
	this->frames = frames.frames;
}

Frame mir::FrameContainer::getFrame(int index) {
	Frame result;
	if (index >= frames.size() || index < 0) result = Frame();
	else result = frames[index];
	return result;
}

void mir::FrameContainer::setFrame(int index, const Frame & frame) {
	if (index >= frames.size() || index < 0) return;
	frames[index] = frame;
}

mir::Object::Object() {
	frameIndex = FullFrame;
	solid = false;
	depth = 0;
	grp = nullptr;
	anim = nullptr;
	type = "object";
}

mir::Object::Object(const Object & obj) {
	Sprite::operator=(obj);
	frameIndex = obj.frameIndex;
	frames = obj.frames;
	solid = obj.solid;
	depth = obj.depth;
	anim = nullptr;
	grp = nullptr;
	type = obj.type;
}

mir::Object::Object(const Sprite & sprite) {
	Sprite::operator=(sprite);
	frameIndex = FullFrame;
	solid = false;
	depth = 0;
	grp = nullptr;
	anim = nullptr;
	type = "object";
}

mir::Object::Object(const Texture & texture) {
	setTexture(texture);
	frameIndex = FullFrame;
	solid = false;
	depth = 0;
	grp = nullptr;
	anim = nullptr;
	type = "object";
}

mir::Object::Object(const Texture & texture, const Frame & rectangle) {
	setTexture(texture);
	setTextureRect(rectangle);
	frameIndex = UnknownFrame;
	solid = false;
	depth = 0;
	grp = nullptr;
	type = "object";
}

mir::Object & mir::Object::operator=(const Object & obj) {
	if(this == &obj) return *this;
	Sprite::operator=(obj);
	solid = obj.solid;
	depth = obj.depth;
	frameIndex = obj.frameIndex;
	frames = obj.frames;
	anim = obj.anim;
	type = obj.type;
	return *this;
}

string mir::Object::getType() {
	return type;
}

void mir::Object::setType(string type) {
	this->type = type;
}

bool mir::Object::isSolid() {
	return solid;
}

void mir::Object::setSolid(bool state) {
	solid = state;
}

int mir::Object::getDepth() {
	return depth;
}

void mir::Object::setDepth(int d) {
	depth = d;
}

void mir::Object::setGroup(mir::Group * grp) {
	this->grp = grp;
}

mir::Group * mir::Object::getGroup() {
	return grp;
}

Vector2f mir::Object::getScreenPosition() {
	if (!getGroup()) return Vector2f();
	if (!getGroup()->getLevel()) return Vector2f();
	Vector2f opos = getPosition();
	Vector2f vpos = getGroup()->getLevel()->getViewPos();
	opos.x -= vpos.x;
	opos.y -= vpos.y;
	return opos;
}

Sprite mir::Object::getSprite(int index) {
	Sprite spr = *this;
	if (index == CurrentFrame) {
		spr.setTextureRect(getFrame(getFrameIndex()));
		return spr;
	}
	spr.setTextureRect(getFrame(index));
	return spr;
}

Vector2u mir::Object::getTextureSize() {
	return getTexture()->getSize();
}

void mir::Object::setSprite(const Sprite & spr) {
	Sprite::operator=(spr);
	frames.clear();
	frameIndex = UnknownFrame;
}


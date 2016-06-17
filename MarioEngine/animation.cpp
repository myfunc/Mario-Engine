#include <vector>
#include <SFML/System.hpp>
#include "animation.h"

using namespace mir;

mir::Animation::Animation() {
	speed = 1000;
	state = ANIM_STOP;
	around = false;
}

mir::Animation::Animation(const Animation & obj) {
	frames = obj.frames;
	speed = obj.speed;
	timer = obj.timer;
	state = obj.state;
	around = false;
}

void mir::Animation::setDelay(int speed) {
	if (speed < 1) speed = 1;
	this->speed = speed;
}

int mir::Animation::getDelay() {
	return speed;
}

bool mir::Animation::isEnd() {
	if (around) return false;
	int index = timer.getTime() / speed;
	return index < getSize();
}

bool mir::Animation::isPlay() {
	return state == ANIM_PLAY;
}

bool mir::Animation::isStop() {
	return state == ANIM_STOP;
}

void mir::Animation::start() {
	timer.start();
	state = ANIM_PLAY;
}

void mir::Animation::pause() {
	timer.pause();
	state = ANIM_STOP;
}

void mir::Animation::reload() {
	timer.reload();
}

int mir::Animation::frame() {
	int result = timer.getTime() / speed;
	if (around)
		result %= getSize();
	else if (result >= getSize()) {
		state = ANIM_END;
		result = getSize() - 1;
	}
	return frames[result];
}

void mir::Animation::push(int num) {
	frames.push_back(num);
}

void mir::Animation::push(int * start, int count) {
	for (int i = 0; i < count; i++) {
		frames.push_back(*(start + i));
	}
}

void mir::Animation::erase(int id) {
	if (id < 0 && id >= frames.size()) return;
	auto iter = frames.begin()+id;
	frames.erase(iter);
}

void mir::Animation::clear() {
	frames.clear();
}

int mir::Animation::getSize() {
	return frames.size();
}

unsigned mir::Timer::getMs() {
	return clock.getElapsedTime().asMilliseconds();
}

mir::Timer::Timer() {
	tickrate = 0;
	tickrateStart = 0;
	tickratePause = 0;
	state = ANIM_STOP;
}

void mir::Timer::start() {
	update();
	state = ANIM_PLAY;
	tickrateStart = getMs();
	tickrate = tickratePause;
}

void mir::Timer::pause() {
	update();
	tickratePause = tickrate;
	state = ANIM_STOP;
}

void mir::Timer::reload() {
	tickratePause = 0;
	tickrate = 0;
	tickrateStart = getMs();
}

void mir::Timer::update() {
	if (state == ANIM_PLAY){
			tickrate = (getMs() - tickrateStart + tickratePause);
	}
}

unsigned mir::Timer::getTime() {
	update();
	return tickrate;
}

#include "processor.h"
#include <list>
#include <iostream>

using namespace std;
using namespace mir;
void mir::Processor::calcTime() {
	Time timeDelay = clock.getElapsedTime();
	timeStamp.tickrate = tickrateMin;

	//if (timeStamp.tickrate < tickrateMin)
	//	timeStamp.tickrate = tickrateMin;
	//if (timeStamp.tickrate > tickrateMax)
	//	timeStamp.tickrate = tickrateMax;
	timeStamp.tickrate /= tickMulti;
	if (timeDelay.asMilliseconds() < 1000/fpsLimit)
		sleep(milliseconds(1000 / fpsLimit - timeDelay.asMilliseconds()));
	clock.restart();
}
Processor::Processor() {
	tickrateMin = 10000;
	tickrateMax = 20000;
	tickMulti = 550000;
	fpsLimit = 60;
}
void Processor::push(Process & proc) {
	processes.push_back(&proc);
}

bool Processor::erase(Process & proc) {
	auto iter = processes.begin();
	while (iter != processes.end()) {
		if (&proc == *iter) {
			processes.erase(iter);
			return true;
		}
		iter++;
	}
	return false;
}

void Processor::clear() {
	processes.clear();
}

int Processor::getSize() {
	return processes.size();
}

bool Processor::process(){
	
	if (processes.size() == 0) return false;
	list<list<Process*>::iterator> toRemove;
	auto iter = processes.begin();
	while (iter != processes.end()) {
		if (!*iter) toRemove.push_back(iter); else
			if (!(*iter)->iterate(timeStamp)) toRemove.push_back(iter);
		iter++;
	}

	auto riter = toRemove.begin();
	while (riter != toRemove.end()) {
		processes.erase(*riter);
		riter++;
	}

	calcTime();
	return true;
}

const TimeStamp & mir::Processor::getTimeStamp() {
	return timeStamp;
}

const Clock & mir::Processor::getClock() {
	return clock;
}

int mir::Processor::getTickrateMin() {
	return tickrateMin;
}

void mir::Processor::setTickrateMin(int t) {
	tickrateMin = t;
}

int mir::Processor::getTickrateMax() {
	return tickrateMax;
}

void mir::Processor::setTickrateMax(int t) {
	tickrateMax = t;
}

void mir::Processor::setLimitFPS(int fps) {
	fpsLimit = fps;
}

int mir::Processor::getLimitFPS() {
	return fpsLimit;
}

list<Process*>::iterator Processor::begin() {
	return processes.begin();
}

list<Process*>::iterator Processor::end() {
	return processes.end();
}

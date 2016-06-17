#include "event_listener.h"
using namespace mir;
EventHandler::EventHandler(EventCallback func) {
	this->func = func;
}

void EventHandler::process(Event& ev) {
	func(ev);
}

void EventProcessor::clear() {
	events.clear();
}

void EventProcessor::push(EventHandler & ev) {
	events.push_back(ev);
}

void EventProcessor::push(EventCallback ev) {
	events.push_back(EventHandler(ev));
}

void EventProcessor::process(Event & ev) {
	auto iter = events.begin();
	while (iter != events.end()) {
		iter->process(ev);
		iter++;
	}
}

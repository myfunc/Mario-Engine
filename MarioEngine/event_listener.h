#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include <functional>
using namespace sf;
using namespace std;
namespace mir {
	typedef function<void(Event& ev)> EventCallback;

	class EventHandler {
		EventCallback func;
	public:
		EventHandler(EventCallback func);
		void process(Event& ev);

	};

	class EventProcessor {
		static list<EventHandler> events;
	public:
		void clear();
		void push(EventHandler& ev);
		void push(EventCallback ev);
		void process(Event& ev);
		// Нет времени. Позднее можно дорабатывать считывание, удаление и т.д.
		// Пока этого хватит.
	};
	
}
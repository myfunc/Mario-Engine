#pragma once
#include <SFML/Graphics.hpp>
#include "event_listener.h"
#include "processor.h"
#include <list>


using namespace sf;
namespace mir {
	static EventProcessor eventListener;

	class Engine {
	protected:
		Processor proc;
		RenderWindow window;
		list<Drawable*> todraw;
		EventProcessor& evl;
	public:
		Engine();
		Engine(VideoMode mode, char* label);
		Processor* processor();
		bool loop();
		virtual void iterate() = 0;
		virtual void eventProd();
		virtual void update();
		void draw(Drawable& drawable);
		RenderWindow& getWindow();

		//Window operations
		void moveView(float x, float y);
		void setViewCenter(float x, float y);
	};
}
#pragma once
#include <functional>
#include <string>
#include "engine.h"
#include "object_container.h"

using namespace sf;

namespace mir {

	/*
	В Level существует globalGroup, именно к нему будет доступ из Объекта.
	*/
	class Level : public Process {
	protected:
		GroupManager globalGroup;
		Engine* engine;
		Vector2i levelSize;
		Vector2i viewSize;
		Vector2f viewPos;
		
	public:
		
		virtual void loadFromFile(char* filename) = 0;
		virtual unsigned iterate(const TimeStamp& time) = 0;

		void setEngine(Engine* engine);
		Engine* getEngine();
		GroupManager* getGlobalGroup();
		ObjectGroup* getGroup(int id);
		// в Group записывается ссылка на текущий уровень. С любого объекта будет доступ к уровню.
		void pushGroup(ObjectGroup* group, int id = -1);
		void removeGroup(int id);

		void setViewSize(int w, int h);
		Vector2i getViewSize();
		void viewMove(Vector2f vec);
		void setViewPosition(Vector2f vec);
		Vector2f getViewPos();
	protected:
		void _setViewPosition(Vector2f vec);
	};

}
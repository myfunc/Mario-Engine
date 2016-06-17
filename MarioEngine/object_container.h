#pragma once
#include "object.h"
#include <list>
#include <functional>

using namespace sf;


namespace mir {
	typedef function<void(Object&)> void_obj_func;
	typedef function<bool(Object&)> bool_obj_func;

	class Level;
	class Group : public Process {
		int id;
		Engine* window;
		Level* level;
	public:
		Group();
		virtual void iterateAll(const TimeStamp& time) = 0;
		virtual void drawAll(Engine& window) = 0;
		virtual void drawAll() = 0;
		virtual void draw(int id, Engine& window) = 0;
		virtual void draw(int id) = 0;
		virtual void foreach(int id, void_obj_func func) = 0;
		// Алгоритмы для группы
		virtual void foreach(void_obj_func func) = 0;
		virtual list<Object*> find(bool_obj_func func) = 0;
		virtual void findToRemove(bool_obj_func func) = 0;
		virtual list<Object*> find(int id,bool_obj_func func) = 0;

		virtual unsigned iterate(const TimeStamp& time) = 0;

		void setEngine(Engine* window);
		Engine* getEngine();
		RenderWindow* getWindow();
		void setId(int id);
		int getId();

		void setLevel(Level* lvl);
		Level* getLevel();
	};

	class ObjectGroup : public Group  {
		
		
	public:
		virtual void push(Object& obj, bool linkGroup = true) = 0;
		virtual bool erase(Object& unit) = 0;
		virtual void push(Object* obj, bool linkGroup = true) = 0;
		virtual bool erase(Object* unit) = 0;
		virtual void clear() = 0;
		virtual int getSize() = 0;

		// Итераторы
		virtual list<Object*>::iterator begin() = 0;
		virtual list<Object*>::iterator end() = 0;

		virtual void iterateAll(const TimeStamp& time) = 0;
		virtual void drawAll(Engine& window) = 0;
		virtual void drawAll() = 0;
		// Алгоритмы для группы
		virtual void foreach(void_obj_func func) = 0;
		virtual void foreach(int id, void_obj_func func) = 0;
		virtual list<Object*> find(bool_obj_func func) = 0;
		virtual list<Object*> find(int id, bool_obj_func func) { return list<Object*>(); }
		virtual unsigned iterate(const TimeStamp& time) = 0;
		virtual void findToRemove(bool_obj_func func) {};

		
	};


	// Устаревший класс. Использовать небезопасно.
	class ObjectContainer {
		list<Object> objs;
	public:
		ObjectContainer();
		Object* push(const Object& obj);
		void erase(list<Object>::iterator& iterator);
		void clear();

		// Итераторы
		list<Object>::iterator begin();
		list<Object>::iterator end();
		int getSize();

		void iterateAll(const TimeStamp& time);
		void drawAll(Engine& window);
		// Алгоритмы для группы
		void foreach(void_obj_func func);
		list<Object*> find(bool_obj_func func);

		unsigned iterate(const TimeStamp& time);
	};
	// Устаревший класс. Использовать небезопасно.
	class StaticObjectGroup : public ObjectGroup {
		list<Object*> objects;
		list<Object*> trashBag;
	protected:
		void _clean();
		bool checkPt(Object* unit);
		void toTrashBag(Object* obj);

	public:
		StaticObjectGroup();
		StaticObjectGroup(ObjectContainer& cont, bool link);
		void push(Object& obj, bool linkGroup = true);
		bool erase(Object& unit);
		void push(Object* obj, bool linkGroup = true);
		bool erase(Object* unit);
		void clear();
		int getSize();

		// Итераторы
		list<Object*>::iterator begin();
		list<Object*>::iterator end();

		void iterateAll(const TimeStamp& time);
		void drawAll(Engine& window);
		virtual void draw(int id, Engine& window) {};
		virtual void draw(int id) {};
		virtual void foreach(int id, void_obj_func func) = 0;
		void foreach(void_obj_func func);
		list<Object*> find(bool_obj_func func);
		virtual void findToRemove(bool_obj_func func) = 0;
		unsigned iterate(const TimeStamp& time);
		void pushContainer(ObjectContainer& cont, bool link);
	};
	// Основной класс. Упор на безопасность
	class DynamicObjectGroup : public ObjectGroup {
		list<Object*> objects;
		list<Object*> trashBag;
	protected:
		// c _clean могут быть проблемы в удалении. Иметь в виду.
		void _clean();
		bool checkPt(Object* unit);
		void toTrashBag(Object* obj);

	public:
		DynamicObjectGroup();
		DynamicObjectGroup(const DynamicObjectGroup& obj) = delete;
		DynamicObjectGroup& operator=(const DynamicObjectGroup& obj) = delete;
		void push(Object& obj, bool linkGroup = true);
		bool erase(Object& unit);
		void push(Object* obj, bool linkGroup = true);
		bool erase(Object* unit);
		void clear();
		int getSize();

		// Итераторы
		list<Object*>::iterator begin();
		list<Object*>::iterator end();

		void iterateAll(const TimeStamp& time);
		void drawAll(Engine& window);
		void drawAll();
		virtual void draw(int id, Engine& window) {};
		virtual void draw(int id) {};
		virtual void foreach(int id, void_obj_func func) {};
		void foreach(void_obj_func func);
		list<Object*> find(bool_obj_func func);
		virtual void findToRemove(bool_obj_func func);

		unsigned iterate(const TimeStamp& time);

		~DynamicObjectGroup();
	};
	// Основной класс. Упор на безопасность
	class GroupManager : public Group{
		list<ObjectGroup*> groups;
		Engine* window;
	public:
		void push(ObjectGroup* group);
		bool erase(ObjectGroup* group);
		void clear();
		int getSize();

		ObjectGroup* getById(int id);
		// Итераторы
		list<ObjectGroup*>::iterator begin();
		list<ObjectGroup*>::iterator end();

		void iterateAll(const TimeStamp& time);
		void iterate(int id , const TimeStamp& time);
		void drawAll(Engine& window);
		void drawAll();
		virtual void draw(int id, Engine& window);
		virtual void draw(int id);
		virtual void foreach(int id, void_obj_func func);
		virtual void foreach(void_obj_func func);
		virtual void findToRemove(bool_obj_func func);
		virtual list<Object*> find(bool_obj_func func);
		virtual list<Object*> find(int id, bool_obj_func func);
		list<ObjectGroup*> getGroupsById(int id);

		virtual unsigned iterate(const TimeStamp& time);
	};
}
#include "level.h"
#pragma once
#include <list>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;
namespace mir {

	struct TimeStamp {
		float tickrate;
		Clock clock;
		TimeStamp() {
			tickrate = 0;
		}
	};

	enum ProcessResult{
		PROC_IDLE = 0,
		PROC_VALID,
		PROC_DESTRUCT,
		PROC_REMOVE
	};

	class Process {
	public:
		virtual unsigned iterate(const TimeStamp& time) = 0;
	};

	class ObjectProcess {
	public:
		virtual unsigned iterate(const TimeStamp& time) = 0;
	};

	class Processor {
		list<Process*> processes;
		int tickrateMin;
		int tickrateMax;
		int tickMulti;
		int fpsLimit;
		TimeStamp timeStamp;
		Clock clock;

		void calcTime();
	public:
		Processor();
		void push(Process& proc);
		bool erase(Process& proc);
		void clear();
		int getSize();
		bool process();
		const TimeStamp& getTimeStamp();
		const Clock& getClock();

		int getTickrateMin();
		void setTickrateMin(int t);
		int getTickrateMax();
		void setTickrateMax(int t);

		void setLimitFPS(int fps);
		int getLimitFPS();
		list<Process*>::iterator begin();
		list<Process*>::iterator end();
	};

}
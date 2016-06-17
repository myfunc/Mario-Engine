#pragma once
#include <vector>
#include <string>
#include <list>
#include <functional>
#include "engine.h"
#include "animation.h"
#include "processor.h"
using namespace std;
using namespace sf;



typedef Rect<int> Frame;

namespace mir {
	class FrameContainer {
		vector<Frame> frames;
	public:
		void push(const Frame& frame);
		int getSize();
		void clear();
		void loadFrames(const FrameContainer& frames);
		Frame getFrame(int index);
		void setFrame(int index, const Frame& frame);
	};
	class Group;

	class Object: public Sprite, public ObjectProcess {
	protected:
		FrameContainer frames;
		int frameIndex;
		bool solid;
		int depth;
		Group* grp;
		Animation* anim;
		string type;

		list<Object*> collObjects;
		
	public:
		unsigned iterate(const TimeStamp& time) { return PROC_IDLE; };
		virtual unsigned prepairIterate(const TimeStamp& time) { return PROC_IDLE; }
		enum {
			FullFrame = -1,
			UnknownFrame = -2,
			FailedFrame = -3,
			CurrentFrame = -4
		};
		void test() {
			
		}
		Object();
		Object(const Object& object);
		Object(const Sprite& sprite);
		Object(const Texture& texture);
		Object(const Texture& texture, const Frame& rectangle);

		Object& operator=(const Object& obj);

		string getType();
		void setType(string type);

		bool isSolid();
		void setSolid(bool state);

		int getDepth();
		void setDepth(int d);

		void setGroup(Group* grp);
		Group* getGroup();

		Vector2f getScreenPosition();

		Sprite getSprite(int index = CurrentFrame);

		Vector2u getTextureSize();

		void setSprite(const Sprite& spr);

		/* ������ ��� ������ � �������� ������� */
		void snapToFrame(int w, int h);
		void pushFrame(const Frame& frame);
		void setFrameIndex(int index);
		int getFrameSize();
		int getFrameIndex();
		void clearFrames();
		const FrameContainer& getFrames();
		void loadFrames(const FrameContainer& frames);
		Frame getFrame(int index);
		void setFrame(int index, const Frame& frame);
		bool isValidFrame();
		Frame getFullFrame();
		Frame getCurrentFrame();
		void reflectFrame();

		/* ���������������� ������ */
		void setTextureRect(const Frame& frame);

		virtual bool iteration(const TimeStamp& time) { return false; };

		// ��������� ������� ���� �������� � �������
		void start();
		void pause();
		void reload();

		void setAnimation(Animation* anim);
		void setAnimation(Animation& anim);
		void removeAnimation();

		void animate();

		Frame getMask();
		FloatRect getMaskedFrame();
		// ������ ���������� ������ GameMaker �����������. ��� ������ �������� ����� ������� ���� ������������.
		void instance_destroy();
		// ��������� ��������� �� ������ ��������� �����������. ��� ��������� �������� � ������.
		// ���� ������ ������������, ��� ��� ������ � ������. ���� ���, ����� ��������...
		void instance_create(Object *obj);

		
		Object* isMeeting(float x, float y);
		Object* isMeetingIntersect(float x, float y, FloatRect& intersection);
		void updateColliders(float range = -1);
		Object* isCollide(float x, float y);
		Object* isCollideIntersect(float x, float y, FloatRect& intersection);

	protected:
		void _setTextureRect(const Frame& frame);
	};
}
#include "object_container.h"
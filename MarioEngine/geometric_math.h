#pragma once
#include <SFML/Graphics.hpp>
#include <math.h>
using namespace sf;
using namespace std;
namespace mir {
	class MathUtil {
	public:
		template <typename T>
		static float getOutCircleRadius(Rect<T> rect) {
			float radius = sqrt(pow(rect.width, 2)+pow(rect.height, 2)) / 2;
			return radius;
		}

		template <typename T>
		static Vector2f getRectCenter(Rect<T> rect) {
			Vector2f center;
			center.x = rect.left + rect.width / 2;
			center.y = rect.top + rect.height / 2;
			return center;
		}
		template <typename T>
		static float getDistance(Vector2<T> a, Vector2<T> b) {
			float distance = sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
			return distance;
		}
	};
}
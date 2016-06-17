#include <SFML/Graphics.hpp>
#include <iostream>
#include "window.h"
#include "global_data.h"

using namespace mir;
using namespace sf;
using namespace std;
list<EventHandler> EventProcessor::events;

void main() {

	GameWindow game(VideoMode(768,480),"Mario Runner");
	while (game.loop()) {
	}
}	
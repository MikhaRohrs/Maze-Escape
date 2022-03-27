#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

int main()
{
  RenderWindow window(VideoMode(200, 200), "SFML works!");
  CircleShape shape(100.f);
  shape.setFillColor(sf::Color::Green);

  while (window.isOpen())
  {
      Event event{};
      while (window.pollEvent(event))
	  {
      if (event.type == sf::Event::Closed)
	  {
        window.close();
      }
    }
    window.clear();
    window.draw(shape);
    window.display();
  }
  return 0;
}
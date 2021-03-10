#include <SFML/Graphics.hpp>
#include <iostream>

// 	To compile:
//	clang++ main.cpp -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system

int main()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "McDaWindow");
    sf::CircleShape shape(100.0f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			
			case sf::Event::Resized:
				std::cout << event.size.width << ", " << event.size.height << '\n';
				break;

			case sf::Event::TextEntered:
				if(event.text.unicode < 128)
					std::cout << static_cast<char>(event.text.unicode) << '\n';
				break;
			}

            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}
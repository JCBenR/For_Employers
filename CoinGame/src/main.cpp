#include <SFML/Graphics.hpp>
#include <iostream>

/*
 1. character object
    a. location in window
    b. shape and color
    c. 
 2. goal objects
    a. location in window
    b. point value
    c. movement
    d. countdown
    e. speed
    f. is live
    --shape class
    --event class
    --time class
    --color
 3. window
    a. static size (window class)
    b. resizing window
 4. countdown clock
    a. start time
    b. countdown
 5. score counter
    a. total score
    b. add points method
    c. high score
    d. running total
 6. vector of objects
    a. amount of objects
    b. randomization
    c. generate objects

 */

int main()
{
    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");

    sf::CircleShape shape(50.f);
    sf::CircleShape shape2(25.f);
    
    // set the shape color to green
    shape.setFillColor(sf::Color(100, 250, 50));
    shape.setOutlineThickness(10.f);
    shape.setOutlineColor(sf::Color(250, 150, 100));
    
    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            
            //KEYBOARD MOVEMENTS
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                shape.move(-10, 0);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                shape.move(10, 0);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                shape.move(0, -10);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                shape.move(0, 10);
            }
        }

        // clear the window with black color
        window.clear(sf::Color::Black);




        
       


 window.draw(shape);
	// end the current frame
        window.display();
      
    }

    return 0;
}


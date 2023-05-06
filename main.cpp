#include <SFML/Graphics.hpp>
#include <iostream>
#include <tgmath.h>
std::string map =   "#####"
                    "#.#.#"
                    "#.#.#"
                    "#...#"
                    "#####";
const int mapSize = 5;
const float PI = 3.14159;
const float FOV = PI/2;
const int numRays = 100;
float playerX = 10;
float playerY = 20;
const float stepAngle = FOV/numRays;
float playerAngle = 3;
const int maxDepth = 50;
const float tileSize = 10;

const int windowWidth = 1920;
const int windowHeight = 1080;

sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "RC");
float radToDeg(float rad){
    return rad*(180/PI);
}
float degToRad(float deg){
    return deg/(180/PI);
}
void castRays(){
    float startAngle = playerAngle - FOV/2;
    //std::cout << "casting";
    for(int ray = 0; ray<numRays; ray++){
        for(int depth = 0; depth<maxDepth; depth++){
            float targx = playerX - sin(startAngle) * depth;
            float targy = playerY + cos(startAngle) * depth;

            int col = std::round((targx / tileSize));
            int row = std::round((targy / tileSize));
            
            int cursquare = row * mapSize + col;
            if(map[cursquare] != '.'){
                if(map[cursquare] == '#'){
                    float height = 2100 / (depth + 0.0001);
                    sf::RectangleShape rect(sf::Vector2f(windowWidth / numRays, height));
                    rect.setPosition(windowWidth/numRays*ray, (windowHeight/2) - height/2);
                    sf::Color color(255-depth*5, 255-depth*5, 255-depth*5);
                    rect.setFillColor(color);
                    window.draw(rect);
                    break;
                }
            }
        }
        startAngle += stepAngle;
    }
}
const float movStr = 150;
int main()
{
    std::cout << map;

    while (window.isOpen())
    {
        int col = std::round((playerX / tileSize));
        int row = std::round((playerY / tileSize));
            
        int cursquare = row * mapSize + col;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            
            playerX += -sin(playerAngle)/movStr;
            playerY += cos(playerAngle)/movStr;
            if(map[cursquare] != '.'){
                playerX += sin(playerAngle)/(movStr/2);
                playerY += -cos(playerAngle)/(movStr/2);
                std::cout << "collision";
            }
            std::cout << "moving " + std::to_string(playerY) + "\n";
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            playerX += sin(playerAngle)/movStr;
            playerY += -cos(playerAngle)/movStr;
            if(map[cursquare] != '.'){
                playerX += -sin(playerAngle)/(movStr/2);
                playerY += cos(playerAngle)/(movStr/2);
                std::cout << "collision";
            }
            std::cout << "moving " + std::to_string(playerY) + "\n";
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            playerAngle -= PI/2000;
            std::cout << std::to_string(radToDeg(playerAngle)) + "\n";
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            playerAngle += PI/2000;
            std::cout << std::to_string(radToDeg(playerAngle)) + "\n";
        }
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

        }
        window.clear();

        sf::Color floorc(100,100,100);
        sf::Color ceilc(55,55,55);
        sf::RectangleShape floor(sf::Vector2f(windowWidth, windowHeight/2));
        floor.setPosition(0,0);
        floor.setFillColor(floorc);
        window.draw(floor);

        sf::RectangleShape ceiling(sf::Vector2f(windowWidth, windowHeight/2));
        ceiling.setPosition(0,windowHeight/2);
        ceiling.setFillColor(ceilc);
        window.draw(ceiling);
        castRays();
        window.display();
    }
    return 0;
}   

#include <SFML/Graphics.hpp>
#include <iostream>
#include <tgmath.h>

std::chrono::high_resolution_clock::time_point start;
std::chrono::high_resolution_clock::time_point end;
float fps;

std::string map =   
".........###########"
"...#######.........#"
"...#.....#.........#"
"...#.....#.........#"
"...#.....#.........#"
"...#.....#.........#"
"...#.....#.........#"
".#####.###.........#"
".#.....#...........#"
".#.#.###.....#.#####"
".#...........#.#...."
"##.#.#########.#...."
"#..#.#.........#...."
"#..#.#.........#...."
"#..#.#.........#...."
"####.###########...."
"#......#............"
"#......#............"
"#......#............"
"########............";
std::string checkerTex = 
"xwxwxwxwxwxwxwxw"
"wxwxwxwxwxwxwxwx"
"xwxwxwxwxwxwxwxw"
"wxwxwxwxwxwxwxwx"
"xwxwxwxwxwxwxwxw"
"wxwxwxwxwxwxwxwx"
"xwxwxwxwxwxwxwxw"
"wxwxwxwxwxwxwxwx"
"xwxwxwxwxwxwxwxw"
"wxwxwxwxwxwxwxwx"
"xwxwxwxwxwxwxwxw"
"wxwxwxwxwxwxwxwx"
"xwxwxwxwxwxwxwxw"
"wxwxwxwxwxwxwxwx"
"xwxwxwxwxwxwxwxw"
"wxwxwxwxwxwxwxwx";
std::string brickTex =  
"xxxxxxxxxxxxxxxx"
"wwoxwwoxwwoxwwox"
"oooxoooxoooxooox"
"xxxxxxxxxxxxxxxx"
"oxwwoxwwoxwwoxoo"
"oxoooxoooxoooxoo"
"xxxxxxxxxxxxxxxx"
"wwoxwwoxwwoxwwox"
"oooxoooxoooxooox"
"xxxxxxxxxxxxxxxx"
"oxwwoxwwoxwwoxww"
"oxoooxoooxoooxoo"
"xxxxxxxxxxxxxxxx"
"wwoxwwoxwwoxwwox"
"oooxoooxoooxooox"
"xxxxxxxxxxxxxxxx";
std::string defaultWall = 
"xxxxxxxxxxxxxxxx"
"bbbbbbbbbbbbbbbb"
"bbbbbbbbbbbbbbbb"
"bbbbbbbbbbbbbbbb"
"bbbbbbbbbbbbbbbb"
"bbbbbbbbbbbbbbbb"
"bbbbbbbbbbbbbbbb"
"bbbbbbbbbbbbbbbb"
"bbbbbbbbbbbbbbbb"
"bbbbbbbbbbbbbbbb"
"bbbbbbbbbbbbbbbb"
"bbbbbbbbbbbbbbbb"
"bbbbbbbbbbbbbbbb"
"bbbbbbbbbbbbbbbb"
"bbbbbbbbbbbbbbbb"
"xxxxxxxxxxxxxxxx";
const unsigned int textureSize = 16;
const int mapSize = 20;
const float PI = 3.14159;
const float FOV = PI/4;
const int numRays = 480;
float playerX = 7;
float playerY = 4;
const float stepAngle = FOV/numRays;
float playerAngle = 3;
const int tileSize = 7;
const int maxDepth = (int)(mapSize * tileSize);
const float shadeStrength = 3;
const int windowWidth = 1920;
const int windowHeight = 1080;
const float precision = 0.1;
float deltaTime;
sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "RC");

float radToDeg(float rad){
    return rad*(180/PI);
}
float degToRad(float deg){
    return deg/(180/PI);    
}
int r;
int g;
int b;

void getRGB(char letter, int depth){
    switch (letter)
    {
        case 'w':
            r = 255;
            g = 255;
            b = 255;
            break;
        case 'x':
            r = 0;
            g = 0;
            b = 0;
            break;
        case 'r':
            r = 255;
            g = 0;
            b = 0;
            break;
        case 'y':
            r = 255;
            g = 255;
            b = 0;
            break;
        case 'g':
            r = 0;
            g = 255;
            b = 0;
            break;
        case 'b':
            r = 0;
            g = 0;
            b = 255;
            break;
        case 'm':
            r = 89;
            g = 17;
            b = 17;
            break;
        case 'o':
            r = 255;
            g = 150;
            b = 0;
            break;  
        case '0':
            r = 200;
            g = 200;
            b = 200;
            break;
        default:
            break;
    }
    r -= depth*shadeStrength;
    g -= depth*shadeStrength;
    b -= depth*shadeStrength;
    if(r < 0){
        r = 0;
    }else if(r > 255){
        r = 255;
    }
    if(g < 0){
        g = 0;
    }else if(g > 255){
        g = 255;
    }
    if(b < 0){
        b = 0;
    }else if(b > 255){
        b = 255;
    }


}
class sprite{
    public:
        int x;
        int y;
        float size;
        bool beenDrawn;
        sprite(int _x, int _y, float _size) { // Constructor with parameters
            x = _x*tileSize;
            y = _y*tileSize;
            size = _size;
        }
};
const int sprAmnt = 1;
sprite sprites[sprAmnt] = {sprite(4,4,50)};

void drawSprite(sprite spr, int x){
    if(spr.beenDrawn == false){
        float distance = sqrt(((spr.x-playerX)*(spr.x-playerX)) + ((spr.y-playerY)*(spr.y-playerY)));
        float scale = 1/distance*spr.size;
        sf::CircleShape defaultSprite(scale);
        defaultSprite.setPosition(x-scale,windowHeight/2-scale);
        window.draw(defaultSprite);
        spr.beenDrawn = true;
    }
    
}
int isSprite(int x,int y){
    for(int i = 0; i<sprAmnt;i++){
        if(sqrt(((x-sprites[i].x)*(x-sprites[i].x)) + ((y-sprites[i].y)*(y-sprites[i].y))) == 0){
            return i;
        }
    }
    return -1;
}
void castRays(){
    unsigned int currentCol = 0;
    float startAngle = playerAngle - FOV/2;
    for(int ray = 0; ray<numRays; ray++){
        for(float depth = 0; depth<maxDepth; depth += precision){
            float targx = playerX - sin(startAngle) * depth;
            float targy = playerY + cos(startAngle) * depth;

            int col = std::round((targx / tileSize));
            int row = std::round((targy / tileSize));

            int cursquare = row * mapSize + col;
            int isspr = isSprite(round(targx), round(targy)); //TODO: This is the problem, just remove the round() function calls
            //std::cout << std::to_string(isspr) + "\n";
            if(isspr != -1){
                drawSprite(sprites[isspr], windowWidth/numRays*ray);
                //std::cout <<"drew sprite at ray: " + std::to_string(ray) + " and depth, " + std::to_string(depth) + 
                //"\n" + "where targx, targy = " + std::to_string(targx) + "," + std::to_string(targy) + "\n";
            }
            if(map[cursquare] != '.'){
                if(map[cursquare] == '#'){
                    depth *= cos(playerAngle - startAngle);
                    float height = 2100 / (depth + 0.0001);
                    for(int z = 0; z<textureSize;z++){
                        int rowcol = z * textureSize + currentCol;
                        sf::RectangleShape rect(sf::Vector2f(windowWidth / numRays, height/textureSize));
                        rect.setPosition(windowWidth/numRays*ray, (windowHeight/2) - height/2 + (height/textureSize)*z);
                        getRGB(defaultWall[rowcol],depth);
                        sf::Color color(r, g, b);
                        rect.setFillColor(color);
                        window.draw(rect);
                    }
                    break;
                }
            }
        }
        startAngle += stepAngle;
        if(currentCol == textureSize-1){
            currentCol = 0;
        }
        else{
            currentCol++;
        }
    }
    for(int i = 0; i<sprAmnt;i++){
        sprites[i].beenDrawn = false;
    }
}
const float movOffset = 0;
const float movStr = 10;
const float turnStr = 1;
float lastTime = 0;
int main()
{
    playerX *= tileSize;
    playerY *= tileSize;

    while (window.isOpen())
    {
        if(abs(radToDeg(playerAngle)) > 360){
            playerAngle = degToRad(0);
        }
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            int col = std::round(((playerX + -sin(playerAngle-degToRad(movOffset))/movStr) / tileSize));
            int row = std::round(((playerY + cos(playerAngle-degToRad(movOffset))/movStr) / tileSize));
            
            int cursquare = row * mapSize + col;
            if(map[cursquare] == '.'){
                
                playerX += -sin(playerAngle-degToRad(movOffset))/movStr;
                playerY += cos(playerAngle-degToRad(movOffset))/movStr;
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            int col = std::round(((playerX + sin(playerAngle-degToRad(movOffset))/movStr) / tileSize));
            int row = std::round(((playerY + -cos(playerAngle-degToRad(movOffset))/movStr) / tileSize));
            
            int cursquare = row * mapSize + col;
            if(map[cursquare] == '.'){
                playerX += sin(playerAngle-degToRad(movOffset))/movStr;
                playerY += -cos(playerAngle-degToRad(movOffset))/movStr;
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            playerAngle -= PI*turnStr/100;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            playerAngle += PI*turnStr/100;
        }
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

        }
        window.clear();
        start = std::chrono::high_resolution_clock::now();
        sf::Color floorc(50,50,150);
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

        
        end = std::chrono::high_resolution_clock::now();

        fps = (float)1e9/(float)std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();
        deltaTime = abs(lastTime-fps);
        window.setTitle(std::to_string(deltaTime));
        lastTime = fps;
    }
    return 0;
}   

#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

    const int width(800), height(800);
    const int cellSize(40);
    const int allCells(width / cellSize * (height / cellSize));
    const int x = allCells / (width / cellSize);
    const int y = allCells / (height / cellSize);

void verifyNeighbor(int table[][y], int copyTable[][y])
{
    int neightbor = 0;

    for (int k(0); k < x; k++)
        for (int f(0); f < y; f++)
        {
            if (k > 0)
            {
                if (table[k - 1][f - 1] == 1 && f > 0)neightbor++; 
                if (table[k - 1][f + 1] == 1 && f < y - 1)neightbor++;
                if (table[k - 1][f] == 1)neightbor++;            
            }

            if (k < x - 1)
            {
                if (f > 0) if (table[k + 1][f - 1] == 1) neightbor++; 
                if (table[k + 1][f] == 1)neightbor++;
                if (table[k + 1][f + 1] == 1 && f < y - 1)neightbor++;          
            }

            if (f > 0) if (table[k][f - 1] == 1)neightbor++;
            if (f < y - 1) if (table[k][f + 1] == 1)neightbor++; 

            if ((neightbor < 2 || neightbor > 3) && table[k][f] == 1)copyTable[k][f] = -1;
            if (neightbor == 3)copyTable[k][f] = 1;
            neightbor = 0;
        }

}

void addGeneration(int table[][y], int copyTable[][y])
{
    for (int k(0); k < x; k++)
        for (int f(0); f < y; f++)
        {
            table[k][f] += copyTable[k][f];
            if (table[k][f] > 1)table[k][f] = 1;
        }
}

void clearTable(int table[][y])
{
    for (int k(0); k < x; k++)
        for (int f(0); f < y; f++)
            table[k][f] = 0;
}

int main()
{
    float scale = float(cellSize) / 20;
    const int footer = 100;

    sf::RenderWindow window(sf::VideoMode(width, height+footer), "Game of life");
   
    Texture backgroundTexture, cellTexture, startTexture, pauseTexture, reloadTexture;
    backgroundTexture.loadFromFile("images/white.png");
    startTexture.loadFromFile("images/start.png");
    pauseTexture.loadFromFile("images/pause.png");
    reloadTexture.loadFromFile("images/reload.png");
    cellTexture.loadFromFile("images/red.png");

    Sprite startButton(startTexture), pauseButton(pauseTexture), reloadButton(reloadTexture);
  
    startButton.setPosition((width-300)/4, height + footer - 75);
    pauseButton.setPosition(((width - 300) / 4) * 2 + 100, height + footer - 75);
    reloadButton.setPosition(((width - 300) / 4) * 3 + 200, height + footer - 75);

    auto background = new Sprite[x][y];
    auto table = new int[x][y];
    auto copyTable = new int[x][y];

    bool startGame = false;
    bool pause = false;
    bool reload = false;
 
    clearTable(table);
    clearTable(copyTable);
         
    Clock clock;
    float timer = 0, delay = 0.5;

    while (window.isOpen())
    {
        Vector2i pos = Mouse::getPosition(window);

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        float time = clock.getElapsedTime().asSeconds();
        timer += time;
        clock.restart();

        window.clear();

        for (int k(0); k < width/cellSize; k++)
            for (int f(0); f < height / cellSize; f++)
            {
                if (!table[k][f])background[k][f].setTexture(backgroundTexture);
                else background[k][f].setTexture(cellTexture);
                background[k][f].setScale(scale, scale);
                background[k][f].setPosition(k * cellSize, f * cellSize);

                if (event.type == Event::MouseButtonPressed)
                    if (event.key.code == Mouse::Left)
                    {

                        if (background[k][f].getGlobalBounds().contains(pos.x, pos.y))table[k][f] = 1;

                        if (startButton.getGlobalBounds().contains(pos.x, pos.y))
                        {
                            startGame = true;
                            pause = false;
                        }
                        if (pauseButton.getGlobalBounds().contains(pos.x, pos.y)) pause = true;
                        
                        if (reloadButton.getGlobalBounds().contains(pos.x, pos.y))reload = true;

                        if (reload)
                        {
                            startGame = false;
                            clearTable(table);
                            reload = false;
                        }
                    }

                if (startGame && timer > delay && !pause)
                {
                    verifyNeighbor(table,copyTable);
                    addGeneration(table, copyTable);
                    clearTable (copyTable);
                    timer = 0;
                }
  
                window.draw(background[k][f]);
            }
            
        window.draw(startButton);
        window.draw(pauseButton);
        window.draw(reloadButton);
        window.display();
    }

    return 0;
}
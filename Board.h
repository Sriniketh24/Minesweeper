#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "Tile.h"           // <--- include Tile

class Board {
public:
    Board(int cols, int rows, int mines, float yOffset);
    void reset();
    void draw(sf::RenderWindow& window) const;
    void revealAt(int mx, int my);
    void flagAt(int mx, int my);
    bool isWin() const;
    bool isGameOver() const;
    int  remainingMines() const;

    // <--- new methods
    void revealAllMines();
    Tile&       getTile(int x, int y);
    const Tile& getTile(int x, int y) const;

private:
    int cols, rows, mineCount;
    float yOffset;
    bool  gameOver;
    std::vector<std::vector<Tile>> tiles;
    void placeMines();
    void linkNeighbors();
};

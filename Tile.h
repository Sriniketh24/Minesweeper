#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Tile {
public:
    enum State { Hidden, Revealed, Flagged };

    Tile();
    void init(int gridX, int gridY, int size, float yOffset);
    void setMine(bool m);
    bool isMine() const;
    void addNeighbor(Tile* t);
    void reveal();
    void toggleFlag();
    void draw(sf::RenderWindow& window) const;
    bool isRevealed() const;
    bool isFlagged() const;

    // *** NEW getters used by Board::draw and revealAt ***
    int getAdjacentMines() const;
    sf::Vector2f getPosition()   const;

private:
    int x, y, size;
    float yOffset;
    bool mine;
    int adjacentMines;
    State state;
    std::vector<Tile*> neighbors;
    sf::Sprite sprite;
};

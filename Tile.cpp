#include "Tile.h"
#include "TextureManager.h"

Tile::Tile()
        : mine(false)
        , adjacentMines(0)
        , state(Hidden)
{}

void Tile::init(int gx, int gy, int sz, float yo) {
    x = gx; y = gy; size = sz; yOffset = yo;
    mine = false;
    adjacentMines = 0;
    state = Hidden;
    sprite.setTexture(TextureManager::get("tile_hidden.png"));
    sprite.setPosition(x * size, y * size + yOffset);
}

void Tile::setMine(bool m) { mine = m; }
bool Tile::isMine() const   { return mine; }

int Tile::getAdjacentMines() const   { return adjacentMines; }
sf::Vector2f Tile::getPosition() const { return sprite.getPosition(); }

void Tile::addNeighbor(Tile* t) {
    neighbors.push_back(t);
}

void Tile::reveal() {
    if (state != Hidden) return;
    state = Revealed;

    if (mine) {
        // only reveal this bomb
        sprite.setTexture(TextureManager::get("mine.png"));
        return;
    }

    // count neighbors
    adjacentMines = 0;
    for (auto* n : neighbors)
        if (n->isMine())
            ++adjacentMines;

    // grey revealed background
    sprite.setTexture(TextureManager::get("tile_revealed.png"));

    // **no** recursive calls here anymore
}

void Tile::toggleFlag() {
    if (state == Hidden) {
        state = Flagged;
        sprite.setTexture(TextureManager::get("flag.png"));
    }
    else if (state == Flagged) {
        state = Hidden;
        sprite.setTexture(TextureManager::get("tile_hidden.png"));
    }
}

void Tile::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}

bool Tile::isRevealed() const { return state == Revealed; }
bool Tile::isFlagged()  const { return state == Flagged; }

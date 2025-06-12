#include "Board.h"
#include "TextureManager.h"
#include <algorithm>
#include <numeric>
#include <chrono>
#include <random>
#include <queue>

Board::Board(int c, int r, int m, float yo)
        : cols(c), rows(r), mineCount(m), yOffset(yo), gameOver(false)
{
    tiles.resize(rows, std::vector<Tile>(cols));
    reset();
}

void Board::reset() {
    gameOver = false;
    for (int y = 0; y < rows; ++y)
        for (int x = 0; x < cols; ++x)
            tiles[y][x].init(x, y, 32, yOffset);
    placeMines();
    linkNeighbors();
}

void Board::placeMines() {
    std::vector<int> idx(cols * rows);
    std::iota(idx.begin(), idx.end(), 0);
    unsigned seed = static_cast<unsigned>(
            std::chrono::system_clock::now().time_since_epoch().count()
    );
    std::shuffle(idx.begin(), idx.end(), std::default_random_engine(seed));
    for (int i = 0; i < mineCount; ++i) {
        int id = idx[i];
        tiles[id / cols][id % cols].setMine(true);
    }
}

void Board::linkNeighbors() {
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    if (dx == 0 && dy == 0) continue;
                    int nx = x + dx, ny = y + dy;
                    if (nx >= 0 && nx < cols && ny >= 0 && ny < rows)
                        tiles[y][x].addNeighbor(&tiles[ny][nx]);
                }
            }
        }
    }
}

void Board::draw(sf::RenderWindow& window) const {
    for (const auto& row : tiles) {
        for (const auto& t : row) {

            if (!t.isRevealed() && t.isFlagged()) {
                sf::Sprite bg(TextureManager::get("tile_revealed.png"));
                bg.setPosition(t.getPosition());
                window.draw(bg);
                sf::Sprite f(TextureManager::get("flag.png"));
                f.setPosition(t.getPosition());
                window.draw(f);
            }
            else if (!t.isRevealed()) {
                t.draw(window);
            }
            else {
                sf::Sprite bg(TextureManager::get("tile_revealed.png"));
                bg.setPosition(t.getPosition());
                window.draw(bg);

                if (t.isMine()) {
                    sf::Sprite m(TextureManager::get("mine.png"));
                    m.setPosition(t.getPosition());
                    window.draw(m);
                } else {
                    int n = t.getAdjacentMines();
                    if (n > 0) {
                        sf::Sprite num(
                                TextureManager::get("number_" + std::to_string(n) + ".png")
                        );
                        num.setPosition(t.getPosition());
                        window.draw(num);
                    }
                }
            }
        }
    }
}

void Board::revealAt(int mx, int my) {
    if (gameOver) return;
    if (mx < 0 || mx >= cols || my < 0 || my >= rows) return;

    Tile& start = tiles[my][mx];
    if (start.isRevealed() || start.isFlagged())
        return;

    if (start.isMine()) {
        gameOver = true;
        revealAllMines();
        return;
    }

    std::queue<std::pair<int,int>> q;
    q.emplace(mx, my);

    while (!q.empty()) {
        auto [cx, cy] = q.front(); q.pop();
        Tile& t = tiles[cy][cx];
        if (t.isRevealed() || t.isFlagged())
            continue;

        t.reveal();

        if (t.getAdjacentMines() == 0) {
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    if (dx == 0 && dy == 0) continue;
                    int nx = cx + dx, ny = cy + dy;
                    if (nx >= 0 && nx < cols && ny >= 0 && ny < rows) {
                        Tile& nt = tiles[ny][nx];
                        if (!nt.isRevealed() && !nt.isMine()) {
                            q.emplace(nx, ny);
                        }
                    }
                }
            }
        }
    }
}

void Board::flagAt(int mx, int my) {
    if (gameOver) return;
    if (mx < 0 || mx >= cols || my < 0 || my >= rows) return;
    tiles[my][mx].toggleFlag();
}

bool Board::isWin() const {
    int count = 0;
    for (const auto& row : tiles)
        for (const auto& t : row)
            if (!t.isMine() && t.isRevealed())
                ++count;
    return count == (cols * rows - mineCount);
}

bool Board::isGameOver() const {
    return gameOver;
}

int Board::remainingMines() const {
    int flags = 0;
    for (const auto& row : tiles)
        for (const auto& t : row)
            if (t.isFlagged())
                ++flags;
    return mineCount - flags;
}

void Board::revealAllMines() {
    for (int y = 0; y < rows; ++y)
        for (int x = 0; x < cols; ++x)
            if (tiles[y][x].isMine() && !tiles[y][x].isRevealed())
                tiles[y][x].reveal();
}

Tile& Board::getTile(int x, int y) {
    return tiles[y][x];
}
const Tile& Board::getTile(int x, int y) const {
    return tiles[y][x];
}

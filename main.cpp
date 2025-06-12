#include <SFML/Graphics.hpp>
#include <chrono>
#include <cmath>
#include <sstream>
#include <iomanip>
#include "Config.h"
#include "TextureManager.h"
#include "Board.h"
#include "WelcomeWindow.h"
#include "LeaderboardWindow.h"

using Clock    = std::chrono::high_resolution_clock;
using TimePoint= Clock::time_point;

static void drawCounter(sf::RenderWindow& win, int rem, int rows) {
    sf::Texture& digits = TextureManager::get("digits.png");
    int x0 = 33;
    int y0 = rows * 32 + 16;
    bool neg = rem < 0;
    int v = std::abs(rem);
    int d1 = (v / 10) % 10, d2 = v % 10;

    for (int i = 0; i < 3; ++i) {
        sf::Sprite s(digits);
        if (i == 0) {
            int idx = neg ? 10 : 11;
            s.setTextureRect({21*idx, 0, 21, 32});
        } else {
            int dig = (i==1? d1 : d2);
            s.setTextureRect({21*dig, 0, 21, 32});
        }
        s.setPosition(x0 + 21*i, y0);
        win.draw(s);
    }
}

static void drawTimer(sf::RenderWindow& win, int elapsed, int cols, int rows, sf::Font& font) {
    int m = elapsed / 60, s = elapsed % 60;
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << m
        << ":" << std::setw(2) << std::setfill('0') << s;
    sf::Text txt(oss.str(), font, 18);
    txt.setPosition(cols*32 - 100, rows*32 + 32);
    win.draw(txt);
}

int main() {
    std::string player = showWelcome();
    if (player.empty()) return 0;

    Config cfg("config.cfg");
    int W = cfg.cols * 32;
    int H = cfg.rows * 32 + 100;

    TimePoint startTime = Clock::now();

    sf::RenderWindow window(sf::VideoMode(W, H), "Minesweeper", sf::Style::Close);
    sf::Font timerFont;
    if (!timerFont.loadFromFile("font.ttf"))
        throw std::runtime_error("Cannot load font.ttf");

    Board board(cfg.cols, cfg.rows, cfg.mines, /*yOffset=*/0.f);

    sf::Sprite face    (TextureManager::get("face_happy.png"));
    face   .setPosition(W/2 - 16, cfg.rows*32 + 32);

    sf::Sprite debugBtn(TextureManager::get("debug.png"));
    debugBtn.setPosition(W - 304, cfg.rows*32 + 32);

    sf::Sprite pauseBtn(TextureManager::get("pause.png"));
    pauseBtn.setPosition(W - 240, cfg.rows*32 + 32);

    sf::Sprite lbBtn   (TextureManager::get("leaderboard.png"));
    lbBtn   .setPosition(W - 176, cfg.rows*32 + 32);

    bool debugMode = false;
    bool paused    = false;
    TimePoint pauseTime;
    bool   wasPausedBeforeLB = false;

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                window.close();
            }

            if (e.type == sf::Event::MouseButtonPressed) {
                int mx = e.mouseButton.x / 32;
                int my = e.mouseButton.y / 32;

                if (e.mouseButton.button == sf::Mouse::Left) {
                    if (face.getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y)) {
                        board.reset();
                        face.setTexture(TextureManager::get("face_happy.png"));
                        paused = false;
                        pauseBtn.setTexture(TextureManager::get("pause.png"));
                        startTime = Clock::now();
                    }
                    else if (debugBtn.getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y)
                             && !board.isGameOver())
                    {
                        debugMode = !debugMode;
                    }
                    else if (pauseBtn.getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y)
                             && !board.isGameOver())
                    {
                        paused = !paused;
                        if (paused) {
                            pauseTime = Clock::now();
                            pauseBtn.setTexture(TextureManager::get("play.png"));
                        } else {
                            startTime += (Clock::now() - pauseTime);
                            pauseBtn.setTexture(TextureManager::get("pause.png"));
                        }
                    }
                    else if (lbBtn.getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y)) {
                        wasPausedBeforeLB = paused;
                        paused = true;
                        pauseBtn.setTexture(TextureManager::get("play.png"));

                        int elapsed = std::chrono::duration_cast<std::chrono::seconds>(
                                Clock::now() - startTime
                        ).count();

                        showLeaderboard(elapsed, player);

                        paused = wasPausedBeforeLB;
                        pauseBtn.setTexture(
                                TextureManager::get(paused ? "play.png" : "pause.png")
                        );
                    }
                    else if (!paused && mx >= 0 && mx < cfg.cols && my >= 0 && my < cfg.rows) {
                        board.revealAt(mx, my);
                    }
                }

                if (e.mouseButton.button == sf::Mouse::Right && !paused) {
                    if (mx >= 0 && mx < cfg.cols && my >= 0 && my < cfg.rows) {
                        board.flagAt(mx, my);
                    }
                }
            }
        }

        if (!board.isGameOver() && board.isWin()) {
            face.setTexture(TextureManager::get("face_win.png"));
        }
        if (board.isGameOver()) {
            face.setTexture(TextureManager::get("face_lose.png"));
        }

        window.clear(sf::Color::White);

        if (paused) {
            sf::Texture& revealed = TextureManager::get("tile_revealed.png");
            for (int y = 0; y < cfg.rows; ++y) {
                for (int x = 0; x < cfg.cols; ++x) {
                    sf::Sprite s(revealed);
                    s.setPosition(x * 32.f, y * 32.f);
                    window.draw(s);
                }
            }
        }
        else {
            board.draw(window);

            if (debugMode && !board.isGameOver()) {
                sf::Texture& mine = TextureManager::get("mine.png");
                for (int y = 0; y < cfg.rows; ++y) {
                    for (int x = 0; x < cfg.cols; ++x) {
                        if (board.getTile(x,y).isMine() && !board.getTile(x,y).isRevealed()) {
                            sf::Sprite m(mine);
                            m.setPosition(x * 32.f, y * 32.f);
                            window.draw(m);
                        }
                    }
                }
            }
        }

        window.draw(face);
        window.draw(debugBtn);
        window.draw(pauseBtn);
        window.draw(lbBtn);

        drawCounter(window, board.remainingMines(), cfg.rows);

        {

            int elapsed = std::chrono::duration_cast<std::chrono::seconds>(
                    Clock::now() - startTime
            ).count();
            if (elapsed > 999) elapsed = 999;

            int d2 = elapsed / 100;
            int d1 = (elapsed / 10) % 10;
            int d0 = elapsed % 10;

            sf::Texture& digits = TextureManager::get("digits.png");
            float tx = W - 70.f;
            float ty = cfg.rows * 32.f + 16.f;

            int digs[3] = { d2, d1, d0 };
            for (int i = 0; i < 3; ++i) {
                sf::Sprite s(digits);
                s.setTextureRect({21 * digs[i], 0, 21, 32});
                s.setPosition(tx + 21 * i, ty);
                window.draw(s);
            }
        }

        window.display();
    }

    return 0;
}

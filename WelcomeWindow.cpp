#include "WelcomeWindow.h"
#include "Config.h"
#include <SFML/Graphics.hpp>
#include <stdexcept>
#include <cctype>

static void centerText(sf::Text& txt, float x, float y) {
    auto bounds = txt.getLocalBounds();
    txt.setOrigin(bounds.left + bounds.width / 2.f,
                  bounds.top  + bounds.height / 2.f);
    txt.setPosition(x, y);
}

std::string showWelcome() {
    // load board dimensions so we size the window correctly
    Config cfg("config.cfg");
    int W = cfg.cols * 32;
    int H = cfg.rows * 32 + 100;

    sf::RenderWindow window(sf::VideoMode(W, H), "Welcome to Minesweeper",
                            sf::Style::Close);

    // load font
    sf::Font font;
    if (!font.loadFromFile("font.ttf"))
        throw std::runtime_error("Cannot load font");

    // title text
    sf::Text title("WELCOME TO MINESWEEPER!", font, 24);
    title.setStyle(sf::Text::Bold | sf::Text::Underlined);
    centerText(title, W / 2.f, H / 2.f - 150.f);

    // prompt text
    sf::Text prompt("Enter your name:", font, 20);
    centerText(prompt, W / 2.f, H / 2.f - 75.f);

    // user‐input text (with cursor)
    sf::Text input("", font, 18);
    input.setFillColor(sf::Color::Yellow);
    centerText(input, W / 2.f, H / 2.f - 45.f);

    std::string name;

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                window.close();
                return "";
            }

            // letter input
            if (e.type == sf::Event::TextEntered) {
                char c = static_cast<char>(e.text.unicode);
                if (std::isalpha(static_cast<unsigned char>(c)) && name.size() < 10) {
                    // first letter uppercase, rest lowercase
                    name.push_back(name.empty()
                                   ? static_cast<char>(std::toupper(c))
                                   : static_cast<char>(std::tolower(c)));
                    input.setString(name + '|');
                    centerText(input, W / 2.f, H / 2.f - 45.f);
                }
            }

            // backspace & enter
            if (e.type == sf::Event::KeyPressed) {
                if (e.key.code == sf::Keyboard::BackSpace) {
                    if (!name.empty()) {
                        name.pop_back();
                        input.setString(name + '|');
                        centerText(input, W / 2.f, H / 2.f - 45.f);
                    }
                }
                else if (e.key.code == sf::Keyboard::Enter ||
                         e.key.code == sf::Keyboard::Return) {
                    if (!name.empty()) {
                        window.close();
                        return name;
                    }
                }
            }
        }

        window.clear(sf::Color::Blue);
        window.draw(title);
        window.draw(prompt);
        window.draw(input);
        window.display();
    }

    return "";
}

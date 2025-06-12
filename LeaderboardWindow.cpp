
#include "LeaderboardWindow.h"
#include <SFML/Graphics.hpp>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <stdexcept>

struct Entry {
    int timeSec;
    std::string name;
    bool isNew;
};

static int parseTime(const std::string& t) {
    int m = std::stoi(t.substr(0,2));
    int s = std::stoi(t.substr(3,2));
    return m*60 + s;
}

static std::string formatTime(int sec) {
    int m = sec/60, s = sec%60;
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << m
        << ':' << std::setw(2) << std::setfill('0') << s;
    return oss.str();
}

static void centerText(sf::Text& txt, float x, float y) {
    auto b = txt.getLocalBounds();
    txt.setOrigin(b.left + b.width/2.f, b.top + b.height/2.f);
    txt.setPosition(x, y);
}

void showLeaderboard(int elapsedSec, const std::string& playerName) {
    const int LBW = 400, LBH = 300;
    sf::RenderWindow win(
            sf::VideoMode(LBW, LBH),
            "Leaderboard",
            sf::Style::Titlebar | sf::Style::Close
    );

    sf::Font font;
    if (!font.loadFromFile("font.ttf"))
        throw std::runtime_error("Cannot load font.ttf");

    std::vector<Entry> list;
    {
        std::ifstream in("leaderboard.txt");
        std::string line;
        while (std::getline(in, line)) {
            auto p = line.find(',');
            std::string t = line.substr(0, p);
            std::string n = line.substr(p+1);
            list.push_back({ parseTime(t), n, false });
        }
    }

    list.push_back({ elapsedSec, playerName, true });
    std::sort(list.begin(), list.end(),
              [](const Entry &a, const Entry &b){
                  return a.timeSec < b.timeSec;
              });
    if (list.size() > 5) list.resize(5);

    {
        std::ofstream out("leaderboard.txt");
        for (size_t i = 0; i < list.size(); ++i) {
            out << formatTime(list[i].timeSec)
                << ',' << list[i].name;
            if (i + 1 < list.size()) out << "\n";
        }
    }

    sf::Text title("LEADERBOARD", font, 20);
    title.setStyle(sf::Text::Bold | sf::Text::Underlined);
    title.setFillColor(sf::Color::White);
    centerText(title, LBW/2.f, 40.f);

    std::ostringstream oss;
    for (size_t i = 0; i < list.size(); ++i) {
        oss << (i+1) << ".\t"
            << formatTime(list[i].timeSec) << "\t"
            << list[i].name
            << (list[i].isNew ? "*" : "");
        if (i + 1 < list.size()) oss << "\n\n";
    }
    sf::Text content(oss.str(), font, 18);
    content.setFillColor(sf::Color::White);
    centerText(content, LBW/2.f, LBH/2.f);

    while (win.isOpen()) {
        sf::Event e;
        while (win.pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                win.close();
        }

        win.clear(sf::Color(0, 0, 192));
        win.draw(title);
        win.draw(content);
        win.display();
    }
}

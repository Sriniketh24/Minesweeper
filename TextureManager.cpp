#include "TextureManager.h"
#include <stdexcept>

std::map<std::string, sf::Texture> TextureManager::textures;

sf::Texture& TextureManager::get(const std::string& filename) {
    auto it = textures.find(filename);
    if (it != textures.end()) return it->second;
    sf::Texture tex;
    if (!tex.loadFromFile("images/" + filename))
        throw std::runtime_error("Failed to load texture: " + filename);
    textures[filename] = tex;
    return textures[filename];
}

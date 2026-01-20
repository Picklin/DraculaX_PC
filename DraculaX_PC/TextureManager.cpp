#include "TextureManager.h"

TextureManager::TextureManager()
{
    pixel = Texture();
    pixel.loadFromFile("images/pixel.png", TEXTURE_PIXEL_FORMAT_RGBA);
    pixel.setMagFilter(GL_NEAREST);
    addTexture("pixel", &pixel);

    orb = Texture();
    orb.loadFromFile("images/gui&items/orb.png", TEXTURE_PIXEL_FORMAT_RGBA);
    orb.setMagFilter(GL_NEAREST);
    addTexture("orb", &orb);
}

TextureManager& TextureManager::instance()
{
    static TextureManager TexManager;
    return TexManager;
}

void TextureManager::addTexture(const string& entityName, Texture* tex)
{
    textures.emplace(entityName, tex);
}

void TextureManager::removeTexture(const string& entityName)
{
    if (textures[entityName] != NULL && entityName != "pixel")
    {
        textures[entityName]->free();
        delete textures[entityName];
    }
    textures.erase(entityName);
}

Texture* TextureManager::getTexture(const string& entityName)
{
    return textures.at(entityName);
}

bool TextureManager::exists(const string& entityName)
{
    bool exists = false;
    auto it = textures.find(entityName);
    if (it != textures.end()) exists = true;
    return exists;
}

void TextureManager::free()
{
    for (auto& pair : textures)
    {
        string name = pair.first;
        Texture* t = pair.second;
        if (name != "pixel" && t != NULL) {
            t->free();
            delete t;
        }
    }
    textures.clear();
}
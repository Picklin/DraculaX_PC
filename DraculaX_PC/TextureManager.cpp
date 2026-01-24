#include "TextureManager.h"

TextureManager::TextureManager()
{
    Texture* pixel = new Texture();
    pixel->loadFromFile("images/pixel.png", TEXTURE_PIXEL_FORMAT_RGBA);
    pixel->setMagFilter(GL_NEAREST);
    addTexture("pixel", pixel);

    Texture* orb = new Texture();
    orb->loadFromFile("images/gui&items/orb.png", TEXTURE_PIXEL_FORMAT_RGBA);
    orb->setMagFilter(GL_NEAREST);
    addTexture("orb", orb);

    Texture* lvlTitle = new Texture();
    lvlTitle->loadFromFile("images/levels/leveltitle.png", TEXTURE_PIXEL_FORMAT_RGBA);
    lvlTitle->setMagFilter(GL_NEAREST);
    addTexture("lvltitle", lvlTitle);

    Texture* dialogueFont = new Texture();
    dialogueFont->loadFromFile("images/fonts/font_intro.png", TEXTURE_PIXEL_FORMAT_RGBA);
    dialogueFont->setMagFilter(GL_NEAREST);
    addTexture("dialogue", dialogueFont);
    Texture* stageClearFont = new Texture();
    stageClearFont->loadFromFile("images/fonts/BigLetters.png", TEXTURE_PIXEL_FORMAT_RGBA);
    stageClearFont->setMagFilter(GL_NEAREST);
    addTexture("BigLetters", stageClearFont);
    Texture* lettersAndNumsFont = new Texture();
    lettersAndNumsFont->loadFromFile("images/fonts/Letters&Nums.png", TEXTURE_PIXEL_FORMAT_RGBA);
    lettersAndNumsFont->setMagFilter(GL_NEAREST);
    addTexture("Letters&Nums", lettersAndNumsFont);

    Texture* subweapons = new Texture();
    subweapons->loadFromFile("images/subweapons/axe.png", TEXTURE_PIXEL_FORMAT_RGBA);
    subweapons->setMagFilter(GL_NEAREST);
    addTexture("subweapons", subweapons);
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
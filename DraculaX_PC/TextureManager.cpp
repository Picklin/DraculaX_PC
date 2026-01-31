#include "TextureManager.h"

TextureManager::TextureManager()
{
    Texture* pixel = new Texture();
    pixel->loadFromFile("images/pixel.png", TEXTURE_PIXEL_FORMAT_RGBA);
    addTexture("pixel", pixel);

    Texture* orb = new Texture();
    orb->loadFromFile("images/gui&items/orb.png", TEXTURE_PIXEL_FORMAT_RGBA);
    addTexture("orb", orb);

    Texture* lvlTitle = new Texture();
    lvlTitle->loadFromFile("images/levels/leveltitle.png", TEXTURE_PIXEL_FORMAT_RGBA);
    addTexture("lvltitle", lvlTitle);

    Texture* dialogueFont = new Texture();
    dialogueFont->loadFromFile("images/fonts/font_intro.png", TEXTURE_PIXEL_FORMAT_RGBA);
    addTexture("dialogue", dialogueFont);
    Texture* stageClearFont = new Texture();
    stageClearFont->loadFromFile("images/fonts/BigLetters.png", TEXTURE_PIXEL_FORMAT_RGBA);
    addTexture("BigLetters", stageClearFont);
    Texture* lettersAndNumsFont = new Texture();
    lettersAndNumsFont->loadFromFile("images/fonts/Letters&Nums.png", TEXTURE_PIXEL_FORMAT_RGBA);
    addTexture("Letters&Nums", lettersAndNumsFont);
    Texture* menuFont = new Texture();
    menuFont->loadFromFile("images/fonts/menu_font.png", TEXTURE_PIXEL_FORMAT_RGBA);
    addTexture("MenuText", menuFont);

    Texture* subweapons = new Texture();
    subweapons->loadFromFile("images/subweapons/axe.png", TEXTURE_PIXEL_FORMAT_RGBA);
    addTexture("subweapons", subweapons);

    Texture* introTexs1 = new Texture();
    introTexs1->loadFromFile("images/cinematics/intro/introTexs1.png", TEXTURE_PIXEL_FORMAT_RGBA);
    addTexture("introTexs", introTexs1);
    Texture* introTexs2 = new Texture();
    introTexs2->loadFromFile("images/cinematics/intro/introTexs2.png", TEXTURE_PIXEL_FORMAT_RGBA);
    addTexture("introTexs2", introTexs2);

    Texture* dracula = new Texture();
    dracula->loadFromFile("images/cinematics/intro/Dracula_intro.png", TEXTURE_PIXEL_FORMAT_RGBA);
    addTexture("dracula_intro", dracula);
    Texture* bat = new Texture();
    bat->loadFromFile("images/cinematics/intro/bat_intro.png", TEXTURE_PIXEL_FORMAT_RGBA);
    addTexture("bat_intro", bat);
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
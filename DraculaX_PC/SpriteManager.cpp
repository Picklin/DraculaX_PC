#include "SpriteManager.h"

SpriteManager& SpriteManager::instance()
{
    static SpriteManager spriteManager;
    return spriteManager;
}

void SpriteManager::addAnimations(const string& entityName, const vector<AnimKeyframes>& anims)
{
    animationsCollection.emplace(entityName, anims);
}

void SpriteManager::removeAnimations(const string& entityName)
{
    animationsCollection.erase(entityName);
}

vector<AnimKeyframes> SpriteManager::getAnimations(const string& entityName)
{
    return animationsCollection.at(entityName);
}

bool SpriteManager::exists(const string& entityName)
{
    bool exists = false;
    auto it = animationsCollection.find(entityName);
    if (it != animationsCollection.end())
    {
        exists = true;
    }
    return exists;
}

void SpriteManager::free()
{
    animationsCollection.clear();
}

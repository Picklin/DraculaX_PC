#pragma once
#ifndef _EFFECTSMANAGER_INCLUDE
#define _EFFECTSMANAGER_INCLUDE

#include <functional>
#include <vector>
#include "Fueguito.h"
#include "Fuegote.h"
#include "ShaderProgram.h"

class EffectsManager
{
public:
	static EffectsManager& instance();
	void init(const glm::ivec2& tileMapDispl, ShaderProgram& shader);
	void update(int deltaTime);
	void render();
	void free();
	void freeTextures();
	void createFueguito(const glm::vec2& pos, const glm::vec3& color, bool background);
	void createFuegote(const glm::vec2& pos, const glm::vec3& color, bool background);
	void createExplosions(const glm::vec2* pos, const glm::vec2& entityOffset, int numExplosions, int timeBetweenExplosions, int area, const glm::vec3& color);
	void createMiniExplosions(const glm::vec2* pos, const glm::vec2& entityOffset, int numExplosions, int timeBetweenExplosions, int area, const glm::vec3& color);
	const vector<Effect*>& getBackgroundEffectsList() const { return backgroundEffects; }

private:
	EffectsManager();
	void initEffect(Effect* e, const glm::vec2& pos, const glm::vec3& color);
	void updateExplosions(int deltaTime);

private:
	vector<Effect*> effects;
	vector<Effect*> backgroundEffects;
	glm::ivec2 tileMapDispl;
	ShaderProgram* shader = nullptr;

	struct ExplosionClusterSettings
	{
		int explosionsTimer = 0;
		int numExplosions = 0;
		int offset = 32;
		bool bigExplosion = false;
		bool background = false;
		glm::vec3 explosionColor;
		const glm::vec2* entityPos = nullptr;
		glm::vec2 entityOffset;	//offset para que se sitúe en el centro de la entidad. Ej: colocarse en el centro de un enemigo
		int timeBetweenExplosions = 125;	//en milisegundos
	};
	vector<ExplosionClusterSettings> explSet;

	Texture fragsTex;
	Texture pointsTex;
};

#endif // !_EFFECTSMANAGER_INCLUDE
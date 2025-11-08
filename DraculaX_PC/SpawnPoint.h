#pragma once

#include <glm/glm.hpp>
#include "Hitbox.h"

struct InfSpawnPoint
{
	InfSpawnPoint(const glm::vec2& min, const glm::vec2& max, int enemyId, int millisecs, const glm::vec2& spawnPos)
	{
		this->area.min = min;
		this->area.max = max;
		this->enemyId = enemyId;
		millisecsBetweenSpawns = millisecs;
		this->spawnPos = spawnPos;
	}
	Hitbox area;
	glm::vec2 spawnPos;
	int enemyId;
	int spawnTime = 0;
	int millisecsBetweenSpawns;
};
struct SpawnPoint
{
	SpawnPoint(const glm::vec2& min, const glm::vec2& max, int enemyId, const glm::vec2& spawnPos)
	{
		this->area.min = min;
		this->area.max = max;
		this->enemyId = enemyId;
		this->spawnPos = spawnPos;
	}
	Hitbox area;
	glm::vec2 spawnPos;
	int enemyId;
};

struct TriggerArea
{
	TriggerArea(const glm::vec2& min, const glm::vec2& max, int eventId)
	{
		this->area.min = min;
		this->area.max = max;
		this->eventId = eventId;
	}
	Hitbox area;
	int eventId;
};
#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include <vector>
#include "Texture.h"
#include "ShaderProgram.h"
#include "Hitbox.h"


// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class TileMap
{

private:
	TileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& shader);

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap* createTileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& shader);

	~TileMap();

	void render() const;
	void free();

	int getTileSize() const { return tileSize; }
	const glm::ivec2& getMapSize() const { return mapSize; }

	bool collisionMoveLeft(const Hitbox& hitbox, float* posX) const;
	bool collisionMoveRight(const Hitbox& hitbox, float* posX) const;
	bool collisionMoveDown(const Hitbox& hitbox, float* posY, int sizey) const;
	bool collisionMoveDown(const Hitbox& hitbox) const;
	int collisionMoveDownWithTileNum(const Hitbox& hitbox, float* posY, int sizey) const;
	bool collisionMoveUp(const Hitbox& hitbox, float* posY) const;
	bool tileLeft(const Hitbox& hitbox) const;
	bool tileRight(const Hitbox& hitbox) const;


	void addCollision(const Hitbox& hitbox);
	void removeCollision(const Hitbox& hitbox);
	void setColor(const glm::vec4& color) { this->color = color; }

private:
	bool loadLevel(const string& levelFile);
	void prepareArrays(const glm::vec2& minCoords, ShaderProgram& shader);

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	int nTiles;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSize, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int* map;
	ShaderProgram* shaderProgram;
	glm::vec4 color;
};


#endif // _TILE_MAP_INCLUDE



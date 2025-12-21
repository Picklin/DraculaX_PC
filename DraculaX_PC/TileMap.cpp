#include <iostream>
#include <fstream>
#include <sstream>
#include "TileMap.h"


TileMap* TileMap::createTileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& shader)
{
	TileMap* map = new TileMap(levelFile, minCoords, shader);

	return map;
}

TileMap* TileMap::createTerrainMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& shader)
{
	TileMap* map = new TileMap(levelFile, minCoords, shader);
	for (int j = 0; j < map->mapSize.y; j++)
	{
		map->map[j * map->mapSize.x] = -1;
	}
	return map;
}

TileMap::TileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& shader)
{
	loadLevel(levelFile);
	prepareArrays(minCoords, shader);
	this->shaderProgram = &shader;
	//color = glm::vec4(1.f);
}

TileMap::~TileMap()
{
	if (map != NULL)
		delete map;
}

void TileMap::render() const
{
	shaderProgram->setUniform2f("texCoordDispl", 0.f, 0.f);
	tilesheet.use();
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 6 * nTiles);
}

void TileMap::free()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string& levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	char tile;

	fin.open(levelFile.c_str());
	if (!fin.is_open())
		return false;
	getline(fin, line);
	if (line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize >> blockSize;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);

	map = new int[mapSize.x * mapSize.y];
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			fin.get(tile);
			if (tile == ' ')
				map[j * mapSize.x + i] = 0;
			else
				map[j * mapSize.x + i] = tile - int('0');
		}
		fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close();

	return true;
}

void TileMap::prepareArrays(const glm::vec2& minCoords, ShaderProgram& shader)
{
	int tile;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;

	nTiles = 0;
	//halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if (tile != 0)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				texCoordTile[0] = glm::vec2(float((tile - 1) % tilesheetSize.x) / tilesheetSize.x, float((tile - 1) / tilesheetSize.x) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				//texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = shader.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = shader.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

// Collision tests for axis aligned bounding boxes.
// They also correct X or Y coordinates if the box is
// already intersecting a tile.

bool TileMap::collisionMoveLeft(const Hitbox& hitbox, float* posX) const
{

	int x, y0, y1;
	x = (int)hitbox.min.x / tileSize;
	y0 = (int)hitbox.min.y / tileSize;
	y1 = (int)hitbox.max.y / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if (map[y * mapSize.x + x] != 0)
		{
			*posX -= hitbox.min.x - (x * tileSize + tileSize);
			return true;
		}
	}

	return false;
}

bool TileMap::collisionMoveRight(const Hitbox& hitbox, float* posX) const
{

	int x, y0, y1;
	x = (int)hitbox.max.x / tileSize;
	y0 = (int)hitbox.min.y / tileSize;
	y1 = (int)hitbox.max.y / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if (map[y * mapSize.x + x] != 0)
		{
			*posX -= hitbox.max.x - (x * tileSize)+1;
			return true;
		}
	}

	return false;
}

bool TileMap::collisionMoveDown(const Hitbox& hitbox, float* posY, int sizey) const
{
	int x0, x1, y;
	x0 = (int)hitbox.min.x / tileSize;
	x1 = (int)hitbox.max.x / tileSize;
	y = (int)hitbox.max.y / tileSize;
	float height = hitbox.max.y - *posY;
	for (int x = x0; x <= x1; x++)
	{
		if (map[y * mapSize.x + x] != 0)
		{
			if (*posY - tileSize * y + height <= 5)
			{
				*posY = float(tileSize * y - sizey);
				return true;
			}
		}
	}
	return false;
}

bool TileMap::collisionMoveDown(const Hitbox& hitbox) const
{
	int x0, x1, y;
	x0 = (int)hitbox.min.x / tileSize;
	x1 = (int)hitbox.max.x / tileSize;
	y = (int)hitbox.max.y / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (map[y * mapSize.x + x] != 0) return true;
	}
	return false;
}

int TileMap::collisionMoveDownWithTileNum(const Hitbox& hitbox, float* posY, int sizey) const
{
	int x0, x1, y;
	x0 = (int)hitbox.min.x / tileSize;
	x1 = (int)hitbox.max.x / tileSize;
	y = (int)hitbox.max.y / tileSize;
	float height = hitbox.max.y - *posY;
	for (int x = x0; x <= x1; x++)
	{
		int tile = map[y * mapSize.x + x];
		if (tile != 0)
		{
			if (*posY - tileSize * y + height <= 5)
			{
				*posY = float(tileSize * y - sizey);
				return tile;
			}
		}
	}

	return -1;
}

int TileMap::collisionMoveDownWithTileNum(const Hitbox& hitbox) const
{
	int x0, x1, y;
	x0 = (int)hitbox.min.x / tileSize;
	x1 = (int)hitbox.max.x / tileSize;
	y = (int)hitbox.max.y / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		int tile = map[y * mapSize.x + x];
		if (tile != 0)
		{
			return tile;
		}
	}

	return -1;
}

bool TileMap::collisionMoveUp(const Hitbox& hitbox, float* posY) const
{

	int x0, x1, y;
	x0 = (int)hitbox.min.x / tileSize;
	x1 = (int)hitbox.max.x / tileSize;
	y = (int)hitbox.min.y / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (map[y * mapSize.x + x] != 0)
		{
			*posY -= hitbox.min.y - (y * tileSize + tileSize);
			return true;
		}
	}

	return false;
}

bool TileMap::tileLeft(const Hitbox& hitbox) const
{
	int x, y0, y1;
	x = (int)(hitbox.min.x - 8) / tileSize;
	y0 = (int)hitbox.min.y / tileSize;
	y1 = (int)hitbox.max.y / tileSize;
	for (int y = y0; y < y1; y++)
	{
		if (map[y * mapSize.x + x] != 0) return true;
	}
	return false;
}

bool TileMap::tileRight(const Hitbox& hitbox) const
{
	int x, y0, y1;
	x = (int)(hitbox.max.x + 8) / tileSize;
	y0 = (int)hitbox.min.y / tileSize;
	y1 = (int)hitbox.max.y / tileSize;
	for (int y = y0; y < y1; y++)
	{
		if (map[y * mapSize.x + x] != 0) return true;
	}
	return false;
}

int TileMap::distanceFromStairTile(const Hitbox& hitbox, int& dist) const
{
	int x0, x1, y0, y1;
	x0 = (int)hitbox.min.x / tileSize;
	x1 = (int)hitbox.max.x / tileSize;
	y0 = (int)hitbox.min.y / tileSize;
	y1 = (int)hitbox.max.y / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		for (int y = y0; y <= y1; y++)
		{
			int tile = map[y * mapSize.x + x];
			if (tile % 8 == 1)
			{
				dist = (int)hitbox.max.x - (x * tileSize);
				return 1;
			}
			else if (tile % 8 == 2)
			{
				dist = (int)hitbox.min.x - (x * tileSize+tileSize)-4;
				return 2;
			}
		}
	}
	return -1;
}

int TileMap::distanceFromBelowStairTile(const Hitbox& hitbox, int& dist) const
{
	int x0, x1, y0, y1;
	x0 = (int)hitbox.min.x / tileSize;
	x1 = (int)hitbox.max.x / tileSize;
	y0 = (int)hitbox.min.y / tileSize;
	y1 = (int)hitbox.max.y / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		for (int y = y0; y <= y1 && y < mapSize.y; y++)
		{
			int tile = map[y * mapSize.x + x];
			if (tile % 8 == 1)
			{
				dist = (int)hitbox.min.x - x * tileSize - 1;
				return 1;
			}
			else if (tile % 8 == 2)
			{
				dist = (int)hitbox.max.x - (x * tileSize + tileSize) -3;
				return 2;
			}
		}
	}
	return -1;
}

void TileMap::addCollision(const Hitbox& hitbox)
{
	int x0, x1, y0, y1;
	x0 = (int)hitbox.min.x / tileSize;
	x1 = (int)(hitbox.max.x - 1) / tileSize;
	y0 = (int)hitbox.min.y / tileSize;
	y1 = (int)(hitbox.max.y - 1) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		for (int y = y0; y <= y1; y++)
		{
			map[y * mapSize.x + x] = -1;
		}
	}
}

void TileMap::removeCollision(const Hitbox& hitbox)
{
	int x0, x1, y0, y1;
	x0 = (int)hitbox.min.x / tileSize;
	x1 = (int)(hitbox.max.x - 1) / tileSize;
	y0 = (int)hitbox.min.y / tileSize;
	y1 = (int)(hitbox.max.y - 1) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		for (int y = y0; y <= y1; y++)
		{
			map[y * mapSize.x + x] = 0;
		}
	}
}


#pragma once

#include <vector>
#include "mesh.h"
#include "vertex.h"
#include "shader.h"
#include "vertex_array.h"

struct Shader;
struct Mesh;
class VertexArray;

class Topography {

public:
	Topography(unsigned char seed, float scale, bool type);
	void generate();
	Mesh* getMesh();
	float getMaxY();
	float getMinY();
	int getY(int x, int z);
	unsigned char getSeed();
	bool getType();
	float getScale();
	int getSize();
	void setSeed(unsigned char seed);
	void setType(int type);
	void setScale(float scale);

private:
	std::vector<Vertex> generateVertices();
	std::vector<uint32_t> generateIndices();
	void calculateNormals(std::vector<Vertex>& vertices, std::vector<uint32_t> indices);

	Mesh* mesh;
	int size;
	float maxHeight;
	float scale;
	float spacing;
	unsigned char seed; // the seed for this specific topography
	bool type;

	//non normalized minimum and maximum height of the topography
	float minY;
	float maxY;
};
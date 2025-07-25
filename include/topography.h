#pragma once

#include <vector>
#include "mesh.h"
#include "vertex.h"
#include "shader.h"
#include "vertex_array.h"
#include "search_configuration.h"

struct Shader;
struct Mesh;
class VertexArray;

class Topography {

public:
	Topography(int width, int length, float maxHeight, float scale, float spacing);
	void generate();
	Mesh* getMesh();
	float getMaxY();
	float getMinY();
	float getY(int x, int z);
	unsigned char getSeed();
	void setSeed(unsigned char seed);

private:
	std::vector<Vertex> generateVertices();
	std::vector<uint32_t> generateIndices();
	void calculateNormals(std::vector<Vertex>& vertices, std::vector<uint32_t> indices);

	Mesh* mesh;
	int width;
	int length;
	float maxHeight;
	float scale;
	float spacing;
	unsigned char seed; // the seed for this specific topography

	//non normalized minimum and maximum height of the topography
	float minY;
	float maxY;
};
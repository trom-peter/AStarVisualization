#pragma once

#include <vector>
#include <limits>
#include "opengl/mesh.h"
#include "opengl/vertex.h"
#include "opengl/shader.h"
#include "opengl/vertex_array.h"

struct Shader;
struct Mesh;
class VertexArray;

class Topography {

public:
	Topography(unsigned char seed, float scale, bool type, int size, int amplitude);
	~Topography();

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
	int amplitude;
	float scale;
	float spacing;
	unsigned char seed; // the seed for this specific topography
	bool type;

	
	float minY; //non normalized minimum and maximum height of the topography
	float maxY;
};
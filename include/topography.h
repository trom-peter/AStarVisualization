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
	Topography(unsigned char seed, float scale, int type, int size, int amplitude);
	~Topography();

	void setMinMaxHeight();

	float getMaxY();
	float getMinY();
	int getY(int x, int z);
	unsigned char getSeed();
	int getType();
	float getScale();
	int getSize();
	int getAmplitude();
	void setSeed(unsigned char seed);
	void setType(int type);
	void setScale(float scale);
	int size;
	int amplitude;
	float scale;
	float spacing;
	unsigned char seed; // the seed for this specific topography
	int type;

	float minY; //non normalized minimum and maximum height of the topography
	float maxY;

private:

};
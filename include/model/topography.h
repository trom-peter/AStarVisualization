#pragma once
#include <vector>
#include <limits>

class Topography {

public:
	Topography(unsigned char seed, float scale, int type, int size, int amplitude);
	~Topography();

	void setMinMaxHeight();

	float getMaxY();
	float getMinY();
	int getY(int x, int z);

	unsigned char getSeed();
	void setSeed(unsigned char seed);

	int getType();
	void setType(int type);

	float getScale();
	void setScale(float scale);

	int getSize();
	int getAmplitude();
	float getSpacing();

private:
	int size;
	int amplitude;
	float scale;
	float spacing;
	unsigned char seed; // the seed for this specific topography
	int type;

	float minY; //non normalized minimum and maximum height of the topography
	float maxY;
};
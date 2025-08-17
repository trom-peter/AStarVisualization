#pragma once
#include <vector>
#include <limits>

class Topography {

public:
	Topography(const unsigned char seed, const float scale, 
		const int type, const int size, const int amplitude);

	~Topography();

	void setMinMaxHeight();

	float getMaxY() const;
	float getMinY() const;
	int getY(const int x, const int z) const;

	unsigned char getSeed() const;
	void setSeed(unsigned char seed);

	int getType() const;
	void setType(const int type);

	float getScale() const;
	void setScale(const float scale);

	int getSize() const;
	int getAmplitude() const;
	float getSpacing() const;

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
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
	int size; // Topography size in meters
	int amplitude; // Topography amplitude in meters
	float scale; // The higher this parameter, the larger the terrain area. Also known as terrain complexity
	float spacing; // Spacing between generated vertices
	unsigned char seed; // The seed for this topography
	int type; // Topography type (0/1)

	float minY; // Non normalized minimum height of the topography
	float maxY; // Non normalized maximum height of the topography
};

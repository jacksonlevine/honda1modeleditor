
#include "IntTup.hpp"
#include <functional>

bool intTup::operator==(const intTup& other) const {
	return (x == other.x) && (y == other.y) && (z == other.z);
}

intTup& intTup::operator+=(const intTup& other)
{
	x += other.x;
	y += other.y;
	z += other.z;

	return *this;
}

intTup operator+(intTup first,
	const intTup& second)
{
	first.x += second.x;
	first.y += second.y;
	first.z += second.z;

	return first; 
}

intTup::intTup(int x, int y, int z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

intTup::intTup(int x, int z)
{
	this->x = x;
	this->y = 0;
	this->z = z;
}





size_t intTupHash::operator()(const intTup& tup) const {
	size_t hash = 0;
	hash ^= std::hash<int>{}(tup.x) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
	hash ^= std::hash<int>{}(tup.y) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
	hash ^= std::hash<int>{}(tup.z) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
	return hash;
}

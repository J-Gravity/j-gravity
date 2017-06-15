
#include "engine.h"

Vector::Vector()
{
}

Vector::Vector(double xc, double yc, double zc)
{
	x = xc;
	y = yc;
	z = zc;
	w = 0;
}

Vector::Vector(double xc, double yc, double zc, double wc)
{
	x = xc;
	y = yc;
	z = zc;
	w = wc;
}

double			Vector::magnitude()
{
	return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

Vector*			Vector::normalize()
{
	double		mag;

	mag = this->magnitude();
	return new Vector(this->x / mag, this->y / mag, this->z / mag, this->w);
}

Vector*			Vector::add(Vector *other)
{
	return new Vector(this->x + other->x, this->y + other->y,
		this->z + other->z);
}

Vector*			Vector::sub(Vector *other)
{
	return new Vector(this->x - other->x, this->y - other->y,
		this->z - other->z);
}

Vector*			Vector::opposite()
{
	return new Vector(-this->x, -this->y, -this->z);
}

Vector*			Vector::scalar_mult(double k)
{
	return new Vector(this->x * k, this->y * k, this->z * k);
}

double			Vector::dot_product(Vector *other)
{
	return this->x * other->x + this->y * other->y + this->z * other->z;
}

double			Vector::cos(Vector *other)
{
	return this->dot_product(other) / (this->magnitude() * other->magnitude());
}

Vector*			Vector::cross_product(Vector *other)
{
	return new Vector(
		this->y * other->z - this->z * other->y,
		this->z * other->x - this->x * other->z,
		this->x * other->y - this->y * other->x);
}

Vector::~Vector()
{
}

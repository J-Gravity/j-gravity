
#ifndef VECTOR_H
# define VECTOR_H

# include "engine.h"

class				Vector
{
public:
	double			x;
	double			y;
	double			z;
	double			w;

	Vector();
	Vector(double xc, double yc, double zc);
	Vector(double xc, double yc, double zc, double wc);
	double			magnitude();
	Vector*			normalize();
	Vector*			add(Vector *other);
	Vector*			sub(Vector *other);
	Vector*			opposite();
	Vector*			scalar_mult(double k);
	double			dot_product(Vector *other);
	double			cos(Vector *other);
	Vector*			cross_product(Vector *other);
	~Vector();
};

#endif

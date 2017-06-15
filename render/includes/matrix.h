
#ifndef MATRIX_H
# define MATRIX_H

# include "engine.h"

typedef enum		e_mtype
{
	IDENTITY,
	SCALE,
	TRANSLATION,
	ROTATION,
	PROJECTION
}					t_mtype;

class				Matrix
{
public:
	double			mat[4][4];
	t_mtype			type;

	Matrix(t_mtype type); // Identity Matrix
	Matrix(t_mtype type, double scale); // Scale Matrix
	Matrix(t_mtype type, double a, double b, double c);
	Matrix(t_mtype type, double fov, double near, double far, double ratio);
	void		make_translation(double xshift, double yshift, double zshift);
	void		make_rotation(double xrot, double yrot, double zrot);
	Matrix*		mult(Matrix *other); // Returns new Matrix
	Matrix*		reflection();
	Vector*		transform(Vector *vec); // Modifies same Vector
	void		mod_angles(double th, double ph, double ps);
	void		mod_location(double xmod, double ymod, double zmod);
	~Matrix();

private:
	double			alpha;
	double			beta;
	double			gamma;
};

#endif

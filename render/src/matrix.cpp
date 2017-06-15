
#include "engine.h"

Matrix::Matrix(t_mtype type)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			this->mat[i][j] = (i == j) ? 1 : 0;
	this->type = type;
}

Matrix::Matrix(t_mtype type, double scale)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			this->mat[i][j] = (i == j) ? ((i == 3) ? 1 : scale) : 0;
	this->type = type;
}

Matrix::Matrix(t_mtype type, double a, double b, double c)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			this->mat[i][j] = (i == j) ? 1 : 0;
	this->type = type;
	if (type == TRANSLATION)
		this->make_translation(a, b, c);
	else if (type == ROTATION)
		this->make_rotation(a, b, c);
}

Matrix::Matrix(t_mtype type, double fov, double near, double far, double ratio)
{
	double		top;
	double		bottom;
	double		right;
	double		left;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			this->mat[i][j] = (i == j) ? 1 : 0;
	top = tan((M_PI / 180) * (fov / 2)) * near;
	bottom = -top;
	right = top * ratio;
	left = -top * ratio;
	this->mat[0][0] = 2 * near / (right - left);
	this->mat[1][1] = 2 * near / (top - bottom);
	this->mat[2][2] = -((far + near) / (far - near));
	this->mat[2][3] = -((2 * far * near) / (far - near));
	this->mat[3][2] = -1;
	this->type = type;
}

void			Matrix::make_translation(double xshift, double yshift,
	double zshift)
{
	this->mat[0][3] = xshift;
	this->mat[1][3] = yshift;
	this->mat[2][3] = zshift;
	this->alpha = xshift;
	this->beta = yshift;
	this->gamma = zshift;
}

static void		h_rotx(double tmat[4][4], double theta)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			tmat[i][j] = (i == j) ? 1 : 0;
	tmat[1][1] = cos(theta);
	tmat[1][2] = sin(theta);
	tmat[2][1] = -sin(theta);
	tmat[2][2] = cos(theta);
}

static void		h_roty(double tmat[4][4], double phi)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			tmat[i][j] = (i == j) ? 1 : 0;
	tmat[0][0] = cos(phi);
	tmat[0][2] = -sin(phi);
	tmat[2][0] = sin(phi);
	tmat[2][2] = cos(phi);
}

static void		h_rotz(double tmat[4][4], double psi)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			tmat[i][j] = (i == j) ? 1 : 0;
	tmat[0][0] = cos(psi);
	tmat[0][1] = sin(psi);
	tmat[1][0] = -sin(psi);
	tmat[1][1] = cos(psi);
}

static void		h_mat_mult(double mat1[4][4], double mat2[4][4],
	double dest[4][4])
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			dest[i][j] = mat1[i][0] * mat2[0][j]
				+ mat1[i][1] * mat2[1][j]
				+ mat1[i][2] * mat2[2][j]
				+ mat1[i][3] * mat2[3][j];
}

void			Matrix::make_rotation(double theta, double phi, double psi)
{
	double		rotx[4][4];
	double		roty[4][4];
	double		rotz[4][4];
	double		mat1[4][4];
	double		mat2[4][4];

	this->alpha = theta;
	this->beta = phi;
	this->gamma = psi;
	h_rotx(rotx, theta);
	h_roty(roty, phi);
	h_rotz(rotz, psi);
	h_mat_mult(this->mat, roty, mat1);
	h_mat_mult(mat1, rotx, mat2);
	h_mat_mult(mat2, rotz, this->mat);
}

Matrix*			Matrix::mult(Matrix *other)
{
	Matrix		*out;

	out = new Matrix(IDENTITY);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			out->mat[i][j] = this->mat[i][0] * other->mat[0][j]
				+ this->mat[i][1] * other->mat[1][j]
				+ this->mat[i][2] * other->mat[2][j]
				+ this->mat[i][3] * other->mat[3][j];
	return out;
}

Matrix*			Matrix::reflection()
{
	Matrix		*out;

	out = new Matrix(IDENTITY);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			out->mat[i][j] = this->mat[j][i];
	return out;
}

Vector*			Matrix::transform(Vector *vec)
{
	double		tx, ty, tz, tw;

	tx = vec->x;
	ty = vec->y;
	tz = vec->z;
	tw = vec->w;
	vec->x = tx * this->mat[0][0] + ty * this->mat[0][1] +
		tz * this->mat[0][2] + tw * this->mat[0][3];
	vec->y = tx * this->mat[1][0] + ty * this->mat[1][1] +
		tz * this->mat[1][2] + tw * this->mat[1][3];
	vec->z = tx * this->mat[2][0] + ty * this->mat[2][1] +
		tz * this->mat[2][2] + tw * this->mat[2][3];
	vec->w = tx * this->mat[3][0] + ty * this->mat[3][1] +
		tz * this->mat[3][2] + tw * this->mat[3][3];
	return vec;
}

void			Matrix::mod_angles(double th, double ph, double ps)
{
	double		rotx[4][4];
	double		roty[4][4];
	double		rotz[4][4];
	double		mat1[4][4];
	double		mat2[4][4];

	if (this->type != ROTATION)
		return;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			this->mat[i][j] = (i == j) ? 1 : 0;
	this->alpha += th;
	this->beta += ph;
	this->gamma += ps;
	h_rotx(rotx, this->alpha);
	h_roty(roty, this->beta);
	h_rotz(rotz, this->gamma);
	h_mat_mult(this->mat, roty, mat1);
	h_mat_mult(mat1, rotx, mat2);
	h_mat_mult(mat2, rotz, this->mat);
}

void			Matrix::mod_location(double xmod, double ymod, double zmod)
{
	if (this->type != TRANSLATION)
		return;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			this->mat[i][j] = (i == j) ? 1 : 0;
	this->alpha += xmod;
	this->beta += ymod;
	this->gamma += zmod;
	this->mat[0][3] = this->alpha;
	this->mat[1][3] = this->beta;
	this->mat[2][3] = this->gamma;
}

Matrix::~Matrix()
{
}


#include "engine.h"

Camera::Camera(int width, int height, Vector *orig, Matrix *orientation,
	double fov, double near, double far)
{
	this->origin = orig;
	this->translation = new Matrix(TRANSLATION, -orig->x, -orig->y, -orig->z);
	this->rotation = orientation;
	this->view = this->rotation->mult(this->translation);
	this->projection = new Matrix(PROJECTION, fov, near, far, width / height);
	this->width = width;
	this->height = height;
}

void			Camera::watch_vector(Vector *vec)
{
	this->view->transform(vec);
	this->projection->transform(vec);
	vec->x = (this->width * (vec->x + 1)) / 2;
	vec->y = (this->height * (vec->y + 1)) / 2;
}

void			Camera::mod_angles(double th, double ph, double ps)
{
	this->rotation->mod_angles(th, ph, ps);
	delete this->view;
	this->view = this->rotation->mult(this->translation);
}

void			Camera::mod_location(double x, double y, double z)
{
	this->translation->mod_location(x, y, z);
	delete this->view;
	this->view = this->rotation->mult(this->translation);
}

Camera::~Camera()
{
	delete this->origin;
	delete this->translation;
	delete this->rotation;
	delete this->projection;
	delete this->view;
}

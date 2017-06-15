
#ifndef CAMERA_H
# define CAMERA_H

# include "engine.h"

class				Camera
{
public:
	Camera(int width, int height, Vector *orig, Matrix *orientation, double fov,
		double near, double far);
	void			watch_vector(Vector *vec);
	void			mod_angles(double th, double ph, double ps);
	void			mod_location(double x, double y, double z);
	~Camera();

private:
	Vector			*origin;
	Matrix			*translation;
	Matrix			*rotation;
	Matrix			*projection;
	Matrix			*view;
	int				width;
	int				height;
};

#endif

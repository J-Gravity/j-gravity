#include <checker.h>

double	c = 299792.458;
#define C c
#define CSQ (C * C)
#define BETA(V) (V / C)
#define GAMMA(V) (1 / (1 - (BETA * BETA)))
#define ALPHA(V) (1 / GAMMA(V))

//v is the magnitude of the relative velocity note that for some fucktard reason an italicized v is used to rperesent the magnitude while a non-italicized v is used to represent the relative velocity

//So e = mc^2 is actually refers to relativistic mass and therefore is wrong for rest mass with is what we store and most physicists do math with. We can derive the relativistic mass though.
//e = m * c^2 = sqrt(p^2 * c^2 + m^2 * c^4)
//and then MR = e / c^2
//p means momentum
double		sr_relativistic_mass(double m, double p)
{
	double	e;
	e = sqrt(p * p * C * C + m * m * C * C * C * C);
	return (e / CSQ);
}
double		sr_mass_increase(double m, double v)
{
	//mass equals 1 - v squared over C squared to the inverse half power times mass at rest frame. (or atomic mass, they both use the same fucking symbol)
	return (pow((1 - ((v * v)/ CSQ)), -0.5) * m);
}

//https://en.wikipedia.org/wiki/Velocity-addition_formula
double		sr_velocity_addition(double u_prime, double v)
{
	return ((v + u_prime)/(1 + (v * u_prime / CSQ)));
}

//distances be non-static yo
double		sr_length_contraction(double length, double v)
{
	return (pow((1 - ((v * v) / CSQ)), 0.5) * length);
}

double		sr_time_dilation(double deltaT_rest, double v)
{
	return (deltaT_rest / (pow(1 - ((v * v) / CSQ), 0.5)));
}

double		sr_momentum(double m, double v)
{
	return ((m * v) / pow((1 - ((v * v) / CSQ)), 0.5));
}

//for an observer in frame F prime which is defined by its relative velocity v to frame F, an observer movinf in F prime defines events using T prime, x prime, y prime, z prime. x and x prime are equal at t = t prime = 0 with the same holding for the relationship between y and y prime as well as z and z prime.
//for an event in frame F with coordinates t, x, y, z the same event in F prime corresponds to t prime, x prime, y prime, and z prime with the values thereof being defined by a lorentz transformation which uses the lorentz factor gamma.A lorentz transformation in  a particular direction can be defined as a lorentz boost. Note that all Lorentz boost will affect T prime regardless of which other coordinates they effect.
/*double		sr_lorentz_boost_direction(double d, double dvel, double *t)
{
		double	ret;
		//returns coordinate d prime where d prime is any coordinate within frame F prime.
		//devl is defined as the relative velocity between frames F and F prime in direction d
		//this equation only works int the special case that the other two direction axises are parallel, use the more general equation in the case that they are not.
		//this is purely for educational purposes don't use this one if that wasn't clear.	
		ret = (GAMMA(dvel) * (d - dvel * (*t)));
		*t = GAMMA(dvel) * (*t - (v * d / CSQ));
		return (ret);
}

void		sr_lorentz_transformation(double t, double *tprime, t_vector vector, t_vector *vectorprime)
{
		double		tp = *tprime;
		t_vector	vp = *vectorprime;

		//unit3 vecor u in direction v is v/|v|
		//so the components of r are r_parallel + r_perpendicular
		//defined in order as:
		//r_parallel = u(u dot r)
		//r_perpendicular = r - u(u dot r)
		//I don't actually understand what the fuck thats supposed to mean but it doesn't matter
		//here's the important bit:
		//vector R from observer O to event E is the sum of the vector from O to O Prime and the vector form O Prime to E.
		//O to Oprime is just v * t
		//O prime to E is R prime henceforce written as RP
		//RP when seen in F, has its parallel component contracted
		//we can define this as LR prime
		//defined as LRP = (1 + ((1 - gamma)/gamma)uu dot)RP
		//LRP = (1/gamma)u(u dot RP) + RP - u(u dot RP)
		//so R = vt + LRP
		//we also need the inverse or L^-1RP
		//L^-1RP = gamma * u(u dot RP) + RP - u(u dot RP)

		//viewed from F Prime
		//RP = -v(t prime) + LR
		//F Prime sees E as the sum of the vector from O prime to O and the vector from O to E
		//the latter vector in F is R so in F Prime it's parallel component is contracted
		
		//now for the Lorentz transformation:
		//are ready for this https://youtu.be/DJ6CcEOmlYU
		//RP = L^-1(R - vt)

		//anyways Lorentz boost in direction n with magnitude v:
		//TP = gamma(t - (v * n dot R / c^2))
		//RP = R + (gamma - 1)(R dot n)n - gamma * t * v * n
		//I'm just assuming the pointers you are sending are gonna be stack memory. If not add free() here.
		*tprime = tp;
		*vectorprime = vp;
		return ; //included for flow.
}*/

		//okay so you have three-vector a to represent acceleration right?
		//note in the following v represents the relative velocity while v sub m represents the magnitude thereof
		//so a Prime (henceforth known as AP) = (A / ((gamma^2 (1 - ((v dot u) / c^2))^2))) - (((a dot v)v(gamma - 1)) / v^2 * gamma^3 * (1 - ((v dot u)/c^2))^3) + ((a dot v)u/(c^2 * gamma^2 * (1 - ((v dot u)/c^2))^3)))
		//you know what: https://en.wikipedia.org/wiki/Acceleration_(special_relativity)#math_1b
		//thats what I wrote but layed out better.
		//but anyways just apply the above forumala to a sub d to get AP sub d
//alright so here a is our acceleration, u is the velocity of the object and v is the relative velocity of the reference frames.
/*t_vector	sr_acceleration(t_vector a, t_vector u, t_vector v)
{
	return (a);
}*/


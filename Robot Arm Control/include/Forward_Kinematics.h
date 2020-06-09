#define FORWARD_KINEMATICS_H

#include<cstdio>
#include<cstring>

#ifndef TRANSFORMATION_MATRIX_H
#include "Transformation_Matrix.h"
#endif

struct Forward{
	double alpha, a, theta, d;
	// constructor
	Forward(){}
	Forward(double alphai, double ai, double thetai ,double di){alpha = alphai; a = ai; theta = thetai; d = di;}
	// Assign
	Forward& operator=(const Forward& b){alpha = b.alpha; a = b.a; theta = b.theta; d = b.d; return *this;}
	// Equal
	bool operator==(const Forward& b){return (alpha == b.alpha) && (a == b.a) && (theta == b.theta) && (d == b.d); }
	// printout
	void printout(){
		printf("Forward:\n\talpha=%.4lf/rad,%.4lf/deg\ta=%.4lf\ttheta=%.4lf/rad,%.4lf/deg\td=%.4lf\n\n",
		alpha,alpha*180/PI,a,theta,theta*180/PI,d);
	}
};

// TransMatrix(RotMatrix Ri, Vector3 Pi, Vector3 Perspi, double scalei){ R=Ri; P=Pi; Persp=Perspi; scale=scalei; }
// RotMatrix make_RotMatrix(char axis, double rad)
TransMatrix TransFromForward(Forward b){
	Vector3 perspx(0,0,0), nonepx(0,0,0), ax(b.a, 0, 0), dx(0, 0, b.d);
	RotMatrix alphax = make_RotMatrix('x', b.alpha), thetax = make_RotMatrix('z', b.theta);
	return 	TransMatrix(alphax, nonepx, perspx, 1)*TransMatrix(Unit_Matrix(), ax, perspx, 1)
			*TransMatrix(thetax, nonepx, perspx, 1)*TransMatrix(Unit_Matrix(), dx, perspx, 1);
}
TransMatrix TransFromForward_Fast(Forward b){
	Vector3 v1(cos(b.theta), sin(b.theta)*cos(b.alpha), sin(b.theta)*sin(b.alpha)), 
			v2(-sin(b.theta), cos(b.theta)*cos(b.alpha), cos(b.theta)*sin(b.alpha)), 
			v3(0,-sin(b.alpha), cos(b.alpha)), 
			vp(b.a, -sin(b.alpha)*b.d, cos(b.alpha)*b.d), vpersp(0,0,0);
	return TransMatrix(RotMatrix(v1,v2,v3), vp, vpersp, 1);
}

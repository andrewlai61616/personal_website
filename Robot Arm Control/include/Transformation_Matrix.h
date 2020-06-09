#define TRANSFORMATION_MATRIX_H

#include<cstdio>
#include<cstring>

#ifndef RPY_ROTMATRIX_EQVAXISANGLE_QUATERNION_H
#include "RPY_RotMatrix_EqvAxisAngle_Quaternion.h"
#endif

struct TransMatrix{
	RotMatrix R;
	Vector3 P,Persp; // P for point
	double scale;
	// Constructor
	TransMatrix(){}
	TransMatrix(RotMatrix Ri, Vector3 Pi, Vector3 Perspi, double scalei){ R=Ri; P=Pi; Persp=Perspi; scale=scalei; }
	// assign
	TransMatrix operator=(const TransMatrix& b){ R=b.R; P=b.P; Persp=b.Persp; scale=b.scale; return *this; }
	// equal
	bool operator==(const TransMatrix& b){ return (R==b.R)&&(P==b.P)&&(Persp==b.Persp)&&(scale==b.scale); }
	// Multiplication
	TransMatrix operator*(const TransMatrix& b){
		RotMatrix TR = ~R;
		return TransMatrix(
			(R*b.R) + RotMatrix( P*b.Persp.v[1] , P*b.Persp.v[2] , P*b.Persp.v[3] ),
			Vector3( (TR.c[1]^b.P)+P.v[1]*b.scale , (TR.c[2]^b.P)+P.v[2]*b.scale , (TR.c[3]^b.P)+P.v[3]*b.scale ), 
			Vector3( (Persp^b.R.c[1])+b.Persp.v[1]*scale , (Persp^b.R.c[2])+b.Persp.v[2]*scale , (Persp^b.R.c[3])+b.Persp.v[3]*scale ),
			(Persp^b.P) + scale*b.scale
		);
	}
	// Multiply POINT
	Vector3 operator*(const Vector3& b){
		RotMatrix TR = ~R;
		return Vector3( (TR.c[1]^b)+P.v[1] , (TR.c[2]^b)+P.v[2] , (TR.c[3]^b)+P.v[3] );
	}
	// Inverse Transformation Matrix
	TransMatrix operator!(){
		return TransMatrix( ~(*this).R , (~(*this).R)*(*this).P*(-1.0) , Vector3(0,0,0) , 1 );
	}
	// Print T Matrix
	void printout(){
		printf("Transformation Matrix:\n");
		printf("¢z\t%.10lf\t%.10lf\t%.10lf\t|\t%.10lf\t¢{\n", R.c[1].v[1], R.c[2].v[1], R.c[3].v[1], P.v[1] );
		printf("¢x\t%.10lf\t%.10lf\t%.10lf\t|\t%.10lf\t¢x\n", R.c[1].v[2], R.c[2].v[2], R.c[3].v[2], P.v[2] );
		printf("¢x\t%.10lf\t%.10lf\t%.10lf\t|\t%.10lf\t¢x\n", R.c[1].v[3], R.c[2].v[3], R.c[3].v[3], P.v[3] );
		printf("¢x\t------------\t------------\t------------\t-\t------------\t¢x\n");
		printf("¢|\t%.10lf\t%.10lf\t%.10lf\t|\t%.10lf\t¢}\n\n", Persp.v[1], Persp.v[2], Persp.v[3], scale);
	}
};

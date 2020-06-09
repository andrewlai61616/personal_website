
#define RPY_ROTMATRIX_EQVAXISANGLE_QUATERNION_H

#include<cstdio>
#include<cstring>
#include<cmath>
#define PI 3.14159265359
#define torad *PI/180
#define todeg *180/PI

struct Vector3;
struct EqvAA;
struct RotMatrix;
struct RPY;
struct Quaternion;
double abs(double x){
	return x>0? x: -x;
}
void swap(double *a, double *b){
	double tmp = *a;
	*a = *b;
	*b = tmp;
}
struct Vector3{
	// v[1] v[2] v[3]
	double v[4];
	// Constructor
	Vector3(){}
	Vector3(double v1, double v2, double v3){v[1] = v1; v[2] = v2; v[3] = v3;}
	// Assign (element wise)
	Vector3& operator=(const double& b){ v[1]=v[2]=v[3]=b; return *this; }
	Vector3& operator=(const Vector3& b){ v[1]=b.v[1]; v[2]=b.v[2]; v[3]=b.v[3]; return *this; }
	// Equal
	bool operator==(const double& b){ return (v[1]==b)&&(v[2]==b)&&(v[3]==b); }
	bool operator==(const Vector3& b){ return (v[1]==b.v[1])&&(v[2]==b.v[2])&&(v[3]==b.v[3]); }
	// Shift Right
	Vector3 operator>>(const int& b){ return b ? Vector3(v[3],v[1],v[2])>>(b-1) : *this ; }
	// Shift Left
	Vector3 operator<<(const int& b){ return b ? Vector3(v[2],v[3],v[1])<<(b-1) : *this ; }
	// Element operation
	Vector3 operator+(const double& b){ return Vector3(v[1]+b,v[2]+b,v[3]+b); }
	Vector3 operator-(const double& b){ return Vector3(v[1]-b,v[2]-b,v[3]-b); }
	Vector3 operator*(const double& b){ return Vector3(v[1]*b,v[2]*b,v[3]*b); }
	Vector3 operator/(const double& b){ return Vector3(v[1]/b,v[2]/b,v[3]/b); }
	// Dot Product
	double operator^(const Vector3& b){ return v[1]*b.v[1]+v[2]*b.v[2]+v[3]*b.v[3]; }
	// Element operation
	Vector3 operator+(const Vector3& b){ return Vector3( v[1]+b.v[1], v[2]+b.v[2], v[3]+b.v[3] ); }
	Vector3 operator-(const Vector3& b){ return Vector3( v[1]-b.v[1], v[2]-b.v[2], v[3]-b.v[3] ); }
	Vector3 operator/(const Vector3& b){ return Vector3( v[1]/b.v[1], v[2]/b.v[2], v[3]/b.v[3] ); }
	Vector3 operator&(const Vector3& b){ return Vector3( v[1]*b.v[1], v[2]*b.v[2], v[3]*b.v[3] ); }
	
	// Cross Product
	Vector3 operator*(const Vector3& b){ 
		Vector3 tmp = b;
		return (((*this)<<1)&(tmp>>1)) - (((*this)>>1)&(tmp<<1));
	}
	
	// Print out Vector
	void printout(){
		printf("Vector3:\n\t%lf\t%lf\t%lf\n\n",v[1],v[2],v[3]);
	}
};
RotMatrix make_RotMatrix(char axis, double rad);
struct RotMatrix{
	// c1.v[1] c2.v[1] c3.v[1]
	// c1.v[2] c2.v[2] c3.v[2]
	// c1.v[3] c2.v[3] c3.v[3]
	Vector3 c[4];
	// Constructor
	RotMatrix(){}
	RotMatrix(Vector3 v1, Vector3 v2, Vector3 v3){ c[1]=v1; c[2]=v2; c[3]=v3; }
	// assign
	RotMatrix operator=(const RotMatrix& b){ c[1]=b.c[1]; c[2]=b.c[2]; c[3]=b.c[3]; return *this; }
	// equal
	bool operator==(const RotMatrix& b){ return c[1]==b.c[1]&&c[2]==b.c[2]&&c[3]==b.c[3]; }
	// Shift Right
	RotMatrix operator>>(const int& b){ return b ? RotMatrix(c[3],c[1],c[2])>>(b-1) : *this ; }
	// Shift Left
	RotMatrix operator<<(const int& b){ return b ? RotMatrix(c[2],c[3],c[1])<<(b-1) : *this ; }
	// Transpose
	RotMatrix operator~(){
		RotMatrix tmp = *this;
		swap(&tmp.c[1].v[2], &tmp.c[2].v[1]); swap(&tmp.c[1].v[3], &tmp.c[3].v[1]); swap(&tmp.c[2].v[3], &tmp.c[3].v[2]);
		return tmp;
	}
	// Inverse (For orthogonal matrices, the Transpose IS the Inverse!)
	RotMatrix operator!(){ return ~(*this); }
	// Element operation (M +,-,*,/ double)
	RotMatrix operator+(const double& b){ return RotMatrix(c[1]+b, c[2]+b, c[3]+b); }
	RotMatrix operator-(const double& b){ return RotMatrix(c[1]-b, c[2]-b, c[3]-b); }
	RotMatrix operator*(const double& b){ return RotMatrix(c[1]*b, c[2]*b, c[3]*b); }
	RotMatrix operator/(const double& b){ return RotMatrix(c[1]/b, c[2]/b, c[3]/b); }
	// Element operation (M +,-,/ M)
	RotMatrix operator+(const RotMatrix& b){ return RotMatrix(c[1]+b.c[1], c[2]+b.c[2], c[3]+b.c[3]); }
	RotMatrix operator-(const RotMatrix& b){ return RotMatrix(c[1]-b.c[1], c[2]-b.c[2], c[3]-b.c[3]); }
	RotMatrix operator/(const RotMatrix& b){ return RotMatrix(c[1]/b.c[1], c[2]/b.c[2], c[3]/b.c[3]); }
	// Vector3 Multiplication
	Vector3 operator*(const Vector3& b){
		RotMatrix TR = ~(*this);
		return Vector3( TR.c[1]^b , TR.c[2]^b , TR.c[3]^b );
	}
	// Matrix Multiplication
	RotMatrix operator*(const RotMatrix& b){
		RotMatrix T = ~(*this);
		RotMatrix rot(	Vector3( T.c[1]^b.c[1], T.c[2]^b.c[1], T.c[3]^b.c[1] ),
							Vector3( T.c[1]^b.c[2], T.c[2]^b.c[2], T.c[3]^b.c[2] ),
							Vector3( T.c[1]^b.c[3], T.c[2]^b.c[3], T.c[3]^b.c[3] ) );
		return rot;
	}
	// Print out Rotation Matrix
	void printout(){
		printf("Rotation Matrix:\n[\t%.9lf\t%.9lf\t%.9lf\n\t%.9lf\t%.9lf\t%.9lf\n\t%.9lf\t%.9lf\t%.9lf\t]\n\n",
			c[1].v[1], c[2].v[1], c[3].v[1], c[1].v[2], c[2].v[2], c[3].v[2], c[1].v[3], c[2].v[3], c[3].v[3]
		);
	}
};
struct RPY{ // in radians (Also Called Euler Angles)
	double r, p, y; // roll, pitch, yaw
	// Constructor
	RPY(){}
	RPY(double ri, double pi, double yi){r = ri; p = pi; y = yi;}
	// Assign
	RPY& operator=(const RPY& b){ r=b.r;p=b.p;y=b.y; return *this; }
	// Equal
	bool operator==(const RPY& b){ return (r==b.r)&&(p==b.p)&&(y==b.y); }
	// Element operation
	RPY operator+(const double& b){ return RPY(r+b,p+b,y+b); }
	RPY operator-(const double& b){ return RPY(r-b,p-b,y-b); }
	RPY operator*(const double& b){ return RPY(r*b,p*b,y*b); }
	RPY operator/(const double& b){ return RPY(r/b,p/b,y/b); }
	// Print out Roll Pitch Yaw
	void printout(){
		printf("RPY(Euler Angles):\n\tRoll(X): %lf\t\tPitch(Y): %lf\t\tYaw(Z): %lf\n",r,p,y);
		printf("deg->\tRoll(X): %lf\t\tPitch(Y): %lf\t\tYaw(Z): %lf\n\n",r*180/PI,p*180/PI,y*180/PI);
	}
};
struct EqvAA{
	double x, y, z, ang; //angle in radians
	// Constructor
	EqvAA(){}
	EqvAA(double kxi, double kyi, double kzi, double angi){
		if(kzi<0.0){
			kxi=-kxi; kyi=-kyi; kzi=-kzi;
			angi=-angi;
		}
		if(abs(angi+PI)<1e-6)angi=PI;
		x=kxi;y=kyi;z=kzi;ang=angi;
	}
	// Print out X-Y-Z axiz and Angle in Radians and Degrees
	void printout(){
		printf("EqvAA:\n\t[\t\tX\t,\tY\t,\tZ\t,\tANG\t]\n");
		printf("\t[\t%.9lf\t,%.9lf\t,%.9lf\t,%.9lf(deg %.2lf)\t]\n\n",x,y,z,ang,ang todeg);
	}
};
struct Quaternion{
	double w,x,y,z;
	
	// Constructor
	Quaternion(){}
	Quaternion(double wi, double xi, double yi, double zi){w=wi;x=xi;y=yi;z=zi;}
	// Assign
	Quaternion& operator=(const Quaternion& b){ return *this = Quaternion(b.w,b.x,b.y,b.z); }
	// Equal 
	bool operator==(const Quaternion& b){return w==b.w&&x==b.x&&y==b.y&&z==b.z; }
	// Conjugate
	Quaternion operator~(){ return Quaternion(w,-x,-y,-z); }
	// Inverse
	Quaternion operator!(){ return (~(*this))/((*this)^(*this)); }
	// Shift Right
	Quaternion operator>>(const int& b){ return b?Quaternion(z,w,x,y)>>(b-1):*this; }
	// Shift Left
	Quaternion operator<<(const int& b){ return b?Quaternion(x,y,z,w)>>(b-1):*this; }
	// Element operation (Q +,-,*,/ double)
	Quaternion operator+(const double& b){ return Quaternion(w+b,x+b,y+b,z+b); }
	Quaternion operator-(const double& b){ return Quaternion(w-b,x-b,y-b,z-b); }
	Quaternion operator*(const double& b){ return Quaternion(w*b,x*b,y*b,z*b); }
	Quaternion operator/(const double& b){ return Quaternion(w/b,x/b,y/b,z/b); }
	// Element operation (Q +,-,/ Q)
	Quaternion operator+(const Quaternion& b){return Quaternion(w+b.w,x+b.x,y+b.y,z+b.z); }
	Quaternion operator-(const Quaternion& b){return Quaternion(w-b.w,x-b.x,y-b.y,z-b.z); }
	Quaternion operator/(const Quaternion& b){return Quaternion(w/b.w,x/b.x,y/b.y,z/b.z); }
	// Dot Product
	double operator^(const Quaternion& b){return w*b.w + x*b.x + y*b.y + z*b.z;}
	// Quaternion Product
	Quaternion operator*(const Quaternion& b){
		return Quaternion((~(*this))^b , Quaternion(x,w,-z,y)^b ,
			Quaternion(y,z,w,-x)^b , Quaternion(z,-y,x,w)^b );
	}
	// Quaternion Rotate
	Quaternion operator%(const Quaternion& b){
		Quaternion tmp = b;
		return tmp*(*this)*(~tmp);
	}
	// Print out Quaternion parameters
	void printout(){
		printf("Quaternion:\n\tw\t\t+\tix\t\t+\tjy\t\t+\tkz\n");
		printf("\t%lf\t+\t%lf\t+\t%lf\t+\t%lf\n\n",w,x,y,z);
	}
};
struct ZYZ{ // in radians (Also Called Euler Angles)
	double a, b, y; // alpha(turn z first), beta(turn y second), gamma(turn z third)
	// Constructor
	ZYZ(){}
	ZYZ(double ai, double bi, double yi){a = ai; b = bi; y = yi;}
	// Assign
	ZYZ& operator=(const ZYZ& bb){ a=bb.a;b=bb.b;y=bb.y; return *this; }
	// Equal
	bool operator==(const ZYZ& bb){ return (a==bb.a)&&(b==bb.b)&&(y==bb.y); }
	// Element operation
	ZYZ operator+(const double& bb){ return ZYZ(a+bb,b+bb,y+bb); }
	ZYZ operator-(const double& bb){ return ZYZ(a-bb,b-bb,y-bb); }
	ZYZ operator*(const double& bb){ return ZYZ(a*bb,b*bb,y*bb); }
	ZYZ operator/(const double& bb){ return ZYZ(a/bb,b/bb,y/bb); }
	// Print out Roll Pitch Yaw
	void printout(){
		printf("ZYZ(Euler Angles):\n\tAlpha(Z): %lf\t\tBeta(Y): %lf\t\tGamma(Z): %lf\n",a,b,y);
		printf("deg->\tAlpha(Z): %lf\t\tBeta(Y): %lf\t\tGamma(Z): %lf\n\n",a*180/PI,b*180/PI,y*180/PI);
	}
};
double Atan2(double s2, double c2){
	if(abs(c2)<1e-8){
		if(abs(s2)<1e-8){
			printf("#Warning finding Atan2! Both input values are 0.\n");
			return 0;
		}
		if(s2>0)return PI/2; 
		return -PI/2;
	}else if(abs(s2)<1e-8){
		if(c2>0)return 0;
		return PI;
	}else if(c2<0){
		if(s2<0)return atan(s2/c2)-PI;
		return atan(s2/c2)+PI; 
	}else return atan(s2/c2);
}
double Asin(double s2, bool sol){
	if(!sol)return asin(s2);
	if(s2>=0.0)return PI-asin(s2);
	return -PI-asin(s2); 
}

// Unit Matrix
RotMatrix Unit_Matrix(){
	return RotMatrix( Vector3(1,0,0), Vector3(0,1,0), Vector3(0,0,1) );
}
// From Rotation Matrix
RPY RPYfromRotMatrix(RotMatrix b, bool sol){
	double beta = Atan2(-b.c[1].v[3],(sol?-1:1) * sqrt(b.c[1].v[1]*b.c[1].v[1] + b.c[1].v[2]*b.c[1].v[2]));
	double cb = cos(beta);
	return RPY( Atan2(b.c[2].v[3]/cb,b.c[3].v[3]/cb) , 
				beta , 
				Atan2(b.c[1].v[2]/cb,b.c[1].v[1]/cb) );
}
ZYZ ZYZfromRotMatrix(RotMatrix b, bool sol){
	double beta = Atan2((sol?-1:1) * sqrt(b.c[1].v[3]*b.c[1].v[3] + b.c[2].v[3]*b.c[2].v[3]),b.c[3].v[3]);
	double sb = sin(beta);
	if(abs(sb)<1e-8)sb=1;
	return ZYZ(
		Atan2(b.c[3].v[2]/sb , b.c[3].v[1]/sb),
		beta,
		Atan2(b.c[2].v[3]/sb , -b.c[1].v[3]/sb)
	);
}
Quaternion QuafromRPY(RPY b){
	b = b*0.5;
	double cy = cos(b.y), sy = sin(b.y), cr = cos(b.r), sr = sin(b.r), cp = cos(b.p), sp = sin(b.p);
	return Quaternion(cy*cr*cp + sy*sr*sp, cy*sr*cp - sy*cr*sp, cy*cr*sp + sy*sr*cp, sy*cr*cp - cy*sr*sp);
}
Quaternion QuafromEqvAA(EqvAA b){
	return Quaternion( b.x*sin(b.ang/2.0) , b.y*sin(b.ang/2.0) , b.z*sin(b.ang/2.0) , cos(b.ang/2.0) );
}
RotMatrix make_RotMatrix(char axis, double rad){ // in radians
	if(axis!='x'&&axis!='y'&&axis!='z')return RotMatrix(Vector3(1,0,0),Vector3(0,1,0),Vector3(0,0,1));
	int roti = axis-'x';
	return RotMatrix(	Vector3(1,0,0)>>roti,
						Vector3(0,cos(rad),sin(rad))>>roti,
						Vector3(0,-sin(rad),cos(rad))>>roti ) >>roti;
}
RotMatrix RotfromRPY(RPY b){
	return make_RotMatrix('z',b.y)*make_RotMatrix('y',b.p)*make_RotMatrix('x',b.r);
}
RotMatrix RotfromZYZ(ZYZ b){
	return make_RotMatrix('z',b.a)*make_RotMatrix('y',b.b)*make_RotMatrix('z',b.y);
/*
	Vector3 v1( cos(z.a)*cos(z.b)*cos(z.y)-sin(z.a)*sin(z.y), sin(z.a)*cos(z.b)*cos(z.y)+cos(z.a)*sin(z.y), -sin(z.b)*cos(z.y) ) , 
			v2( -cos(z.a)*cos(z.b)*sin(z.y)-sin(z.a)*cos(z.y), -sin(z.a)*cos(z.b)*sin(z.y)+cos(z.a)*cos(z.y), sin(z.b)*sin(z.y) ) , 
			v3( cos(z.a)*sin(z.b), sin(z.a)*sin(z.b), cos(z.b) ) ; 
*/
}
RotMatrix Px(Vector3 p){
	return RotMatrix( Vector3(0,p.v[3],-p.v[2]), Vector3(-p.v[3],0,p.v[1]), Vector3(p.v[2],-p.v[1],0) );
}
RotMatrix RotfromEqvAA(EqvAA r){
	double c = cos(r.ang) , s = sin(r.ang), v = 1-c;
	return RotMatrix(	Vector3(r.x*r.x*v+c, r.x*r.y*v+r.z*s, r.x*r.z*v-r.y*s),
						Vector3(r.x*r.y*v-r.z*s, r.y*r.y*v+c, r.y*r.z*v+r.x*s),
						Vector3(r.x*r.z*v+r.y*s, r.y*r.z*v-r.x*s, r.z*r.z*v+c)	);
}
EqvAA EqvAAfromQua(Quaternion q){
	double rad = 2.0*acos(q.w);
	return EqvAA(q.x/sin(rad/2),q.y/sin(rad/2),q.z/sin(rad/2),rad);
}
EqvAA EqvAAfromRotMatrix(RotMatrix b){
	double theta = acos((b.c[1].v[1]+b.c[2].v[2]+b.c[3].v[3]-1)/2);
	if(abs(abs(theta)-PI)<1e-10)
		return EqvAA( sqrt((b.c[1].v[1]+1)*0.5), sqrt((b.c[2].v[2]+1)*0.5), sqrt((b.c[3].v[3]+1)*0.5), theta  );
	if(abs(theta)<1e-10)
		return EqvAA( 0,0,0,0 );
	double ss = 2*sin(theta);
	return EqvAA( (b.c[2].v[3]-b.c[3].v[2])/ss, (b.c[3].v[1]-b.c[1].v[3])/ss, (b.c[1].v[2]-b.c[2].v[1])/ss, theta );
}
RPY RPYfromEqvAA(EqvAA b){ return RPYfromRotMatrix( RotfromEqvAA( b ) , 0 ); }
RotMatrix RotfromQua(Quaternion b){ return RotfromEqvAA( EqvAAfromQua( b ) ); }
RPY RPYfromQua(Quaternion b){ return RPYfromRotMatrix( RotfromQua( b ) , 0 ); }
EqvAA EqvAAfromRPY(RPY b){ return EqvAAfromQua( QuafromRPY( b ) ) ; }
Quaternion QuafromRotMatrix(RotMatrix b){ return QuafromEqvAA( EqvAAfromRotMatrix(b) ); }

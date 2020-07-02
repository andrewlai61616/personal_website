#define COMPLEX_FUNCTIONS_H

#define GAMMA 0.5772156649015328606
#define PI 3.14159265358979
#define F_const 96500.0
#define R_const 8.3144598
#define Temp 298.0
#define _i complex(0,1.0)
#define phi(z) atan2(z.y, z.x)

complex cpowi( double n ){ return complex( cos(n*PI*0.5) , sin(n*PI*0.5) ); }
complex csqrt( complex z ){
	double sqrt_r = sqrt( abscomplex(z) ), phase = phi(z);
	return complex( sqrt_r * cos( phase/2 ), sqrt_r * sin( phase/2 ) );
}
complex cln( complex z ){ return complex( log( abscomplex(z) ) , phi(z)); }
complex casin( complex z ){ return -_i * cln( _i*z + csqrt( 1 - z*z ) ); }
complex cacos( complex z ){ return -_i * cln( z + _i*csqrt( 1 - z*z ) ); }
complex cexp( complex z ){ return complex( exp(z.x)*cos(z.y), exp(z.x)*sin(z.y) ); }
complex ccos( complex z ){ return ( cexp(complex(0,1)*z) + cexp(complex(0,-1)*z) ) * 0.5 ; }
complex csin( complex z ){ return -_i * 0.5 * ( cexp(complex(0,1)*z) - cexp(complex(0,-1)*z) ); }
complex ccoth( complex z ){
	complex e_z = cexp(z), e_neg_z = cexp(-z);
	return (e_z+e_neg_z) / (e_z-e_neg_z);
}

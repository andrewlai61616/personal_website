#ifndef COMPLEX_FUNCTIONS_H
#include "complex_functions.h"
#endif

#define JACOBI

double int_for_L( double b ){
	// compute int(0->Kk) |cn(t+bi,m)|dt
	double t = 0.0, sum = 0.0, Kk_INT_N, sn_sqr, ph;
	
	double s_star, c_star, d_star;
	jacobianellipticfunctions(b, 1-_m, s_star, c_star, d_star, ph);
	s_star = s_star*s_star, c_star = c_star*c_star, d_star = d_star*d_star;
	
	Kk_INT_N = _Kk/L_INT_N;
	for( int i=0 ; i<=L_INT_N ; i++ ){
		sn_sqr = _sn_list[i]*_sn_list[i];
		sum += sqrt( (1-sn_sqr)*c_star + (1-sn_sqr*_m)*sn_sqr*d_star*s_star ) / ( c_star + _m*sn_sqr*s_star );
	}
	
	return sum * Kk_INT_N;
}

double int_for_b( double x ){
	// compute int(0->x) 1/sqrt(1-mcosh(t)^2)dt
	double t, sum = 0.0, _cosht, x_INT_N;
	
	x_INT_N = x/L_INT_N;
	for( t = x_INT_N ; t <= x ; t += x_INT_N ){
		_cosht = cosh(t);
		sum += 1 / sqrt( 1-_m*_cosht*_cosht );
	}
	return sum*x_INT_N;
}

void make_sn_table(){
	double t = 0.0, cn, dn, ph;
	for( int i=0 ; i<=L_INT_N ; i++ , t += _Kk/L_INT_N ){
		jacobianellipticfunctions(t, _m, _sn_list[i], cn, dn, ph);
	}
}

void make_ratio_table( ){
	printf("Making ratio table ...\n");
	double x, b, y, prev_y;
	double sn, cn, dn, ph;
	int Li;
	complex z, phi;
	_R[N] = prev_y = 0.0;
	for( x = _we*0.5 - _we*0.5/N , Li = N-1 ; x >= 0.0 ; x -= _we*0.5/N , Li-- ){
		if( !((Li*10) % N) ) printf("%d%%\t", 100-Li*100/N);
		phi = casin( -cos( x*PI/_w ) / _ve );
		b = int_for_b( phi.y );
		jacobianellipticfunctions(b, 1-_m, sn, cn, dn, ph);
		z = _w * cacos( -_ve*sn/cn*_i ) / PI;
		_R[Li] = (z.y - prev_y) * 2.0 * N / _we;
		prev_y = z.y;
	}
	printf("DONE\n");
}

void make_len_table(){
	printf("Making length table ...\n");
	double x, b;
	int Li;
	complex phi;
	for( x = 0 , Li = 0 ; x < _we*0.5 ; x += _we*0.5/N , Li++ ){
		if( !((Li*10) % N) ) printf("%d%%\t", Li*100/N);
		phi = casin( -cos( x*PI/_w ) / _ve );
		b = int_for_b( phi.y );
		#ifdef DEBUG
		if( !((Li*100) % N) ){
			printf("%lf\t%lf\n", x, b);
		}
		#endif
		_L[ Li ] = _w*_ve* int_for_L(b) /PI;
	}
	printf("DONE\n");
}

#ifndef Z_CALCULATE_H
#define Z_CALCULATE_H

#ifndef COMPLEX_FUNCTIONS_H
#include "complex_functions.h"
#endif

#define LOW_z -4.0
#define HIGH_z 2.0
#define STEPN_z 10000
#define p10(x) pow(10.0,x)
complex l_I0[STEPN_z+10], l_I1[STEPN_z+10], l_K0[STEPN_z+10], l_K1[STEPN_z+10];

double _P, _E[STEPN_z+10];

void int_I0_Re(double t, double xminusa, double bminusx, double &y, void *ptr) {
	double _Pct = _P*cos(t);
    y = exp( _Pct ) * cos( _Pct );
}
void int_I0_Im(double t, double xminusa, double bminusx, double &y, void *ptr) {
	double _Pct = _P*cos(t);
    y = exp( _Pct ) * sin( _Pct );
}
void int_I1_Re(double t, double xminusa, double bminusx, double &y, void *ptr) {
	double ct = cos(t), _Pct = _P*ct;
    y = exp( _Pct ) * cos( _Pct ) * ct;
}
void int_I1_Im(double t, double xminusa, double bminusx, double &y, void *ptr) {
	double ct = cos(t), _Pct = _P*ct;
    y = exp( _Pct ) * sin( _Pct ) * ct;
}
void int_K0_Re(double t, double xminusa, double bminusx, double &y, void *ptr) {
	double _Pct = _P*cosh(t);
    y = exp( -_Pct ) * cos( _Pct );
}
void int_K0_Im(double t, double xminusa, double bminusx, double &y, void *ptr) {
	double _Pct = _P*cosh(t);
    y = exp( -_Pct ) * sin( _Pct );
}
void int_K1_Re(double t, double xminusa, double bminusx, double &y, void *ptr) {
	double cht = cosh(t), _Pct = _P*cht;
    y = exp( -_Pct ) * cos( _Pct ) * cht;
}
void int_K1_Im(double t, double xminusa, double bminusx, double &y, void *ptr) {
	double cht = cosh(t), _Pct = _P*cht;
    y = exp( -_Pct ) * sin( _Pct ) * cht;
}

complex large_I(int a, complex z, bool e_term){
	// note that z = r * sqrt(omega / D) before multiplying sqrt(i)
	int i,j,k;
	complex add(1.0, 0.0), sum(1.0, 0.0);
	for(k=1;k<100;k++){
		add = add * (4.0*a*a-(2*k-1)*(2*k-1)) * 0.125 / k / z;
		if(k&1) sum -= add;
		else sum += add;
		//printf("k=%d, add=%lf,%lf, sum=%lf,%lf\n", k, add.x, add.y, sum.x, sum.y);
	}
	if(e_term) return sum * cexp(z) / csqrt(2*PI*z);
	return sum / csqrt(2*PI*z);
}

complex large_K(int a, complex z, bool e_term){
	// note that z = r * sqrt(omega / D) before multiplying sqrt(i)
	int i,j,k;
	complex add(1.0, 0.0), sum(1.0, 0.0);
	for(k=1;k<100;k++){
		add = add * (4.0*a*a-(2*k-1)*(2*k-1)) * 0.125 / k / z;
		sum += add;
		//printf("k=%d, add=%lf,%lf, sum=%lf,%lf\n", k, add.x, add.y, sum.x, sum.y);
	}
	if(e_term) return sum * cexp(-z) * csqrt(PI*0.5/z);
	return sum * csqrt(PI*0.5/z);
}

complex I0(){
	if( _P >= 100.0 ) return large_I( 0, complex( _P, _P ), 1 );
	
    autogkstate s;
    autogkreport rep;
    double Re, Im;
    autogksmooth(0.0, PI, s);
    alglib::autogkintegrate(s, int_I0_Re);
    autogkresults(s, Re, rep);
    
    autogksmooth(0.0, PI, s);
    alglib::autogkintegrate(s, int_I0_Im);
    autogkresults(s, Im, rep);
    
    return complex(Re,Im) / PI;
}
complex I1(){
	if( _P >= 100.0 ) return large_I( 1, complex( _P, _P ), 1 );
	
    autogkstate s;
    autogkreport rep;
    double Re, Im;
    autogksmooth(0.0, PI, s);
    alglib::autogkintegrate(s, int_I1_Re);
    autogkresults(s, Re, rep);
    
    autogksmooth(0.0, PI, s);
    alglib::autogkintegrate(s, int_I1_Im);
    autogkresults(s, Im, rep);
    
    return complex(Re,Im) / PI;
}
complex K0(){
	if( _P >= 100.0 ) return large_K( 0, complex( _P, _P ), 1 );
	
    autogkstate s;
    autogkreport rep;
    double Re, Im;
    autogksmooth(0.0, 1e2, s);
    alglib::autogkintegrate(s, int_K0_Re);
    autogkresults(s, Re, rep);
    
    autogksmooth(0.0, 1e2, s);
    alglib::autogkintegrate(s, int_K0_Im);
    autogkresults(s, Im, rep);
    
    return complex(Re,-Im);
}
complex K1(){
	if( _P >= 100.0 ) return large_K( 1, complex( _P, _P ), 1 );
	
    autogkstate s;
    autogkreport rep;
    double Re, Im;
    autogksmooth(0.0, 1e2, s);
    alglib::autogkintegrate(s, int_K1_Re);
    autogkresults(s, Re, rep);
    
    autogksmooth(0.0, 1e2, s);
    alglib::autogkintegrate(s, int_K1_Im);
    autogkresults(s, Im, rep);
    
    return complex(Re,-Im);
}

void initialize_bessel_functions(){
	printf("Initializing Bessel Functions ...\n");
	int i;
	for( i=0 ; i < STEPN_z ; i++ ){
		if(i%1000 == 0)printf("%d%%\t",i*100/STEPN_z+10);
		_P = pow( 10.0, LOW_z + i * (HIGH_z-(LOW_z)) / STEPN_z );
		l_I0[i] = I0(), l_I1[i] = I1(), l_K0[i] = K0(), l_K1[i] = K1();
	}
	printf(" DONE\n");
}

void initialize_elliptic_integral(){
	printf("Initializing Elliptic Integral Functions ... \n");
	int i;
	for( i=0 ; i < STEPN_z ; i++ ){
		if(i%1000 == 0)printf("%d%%\t",i*100/STEPN_z+10);
		_E[i] = ellipticintegrale( (double)i/STEPN_z );
	}
	printf(" DONE\n");
}

complex specific_Z( double omg, double k, double L_w, double w2D ){
	double r0 = L_w / (k-1), rd = r0 * k;
	double log10_z0 = log10( r0 * sqrt( omg * 0.5 * w2D ) ), log10_zd = log10( rd * sqrt( omg * 0.5 * w2D ) );
	
	complex I0d, K00, K0d, I00, K10, I10, Z, Z0, Zd;
	
	double tmpd1, tmpd2;
	int tmpi1, tmpi2;
	if( log10_z0 <= LOW_z ){
		if( log10_zd <= LOW_z ){
			//printf(" (1) ");
			// z0 * ln(zd / z0)
			Z = p10( log10_z0 ) * complex(1.0, 1.0) * ( log10_zd - log10_z0 ) / log10( exp(1.0) );
		}else if( log10_zd < HIGH_z ){
			//printf(" (2) ");
			// -z0 * ( ln(z0/2) + £^ + K0(zd)/I0(zd) )
			tmpd2 = STEPN_z * (log10_zd - LOW_z) / (HIGH_z-(LOW_z));
			tmpi2 = (int)tmpd2;
			I0d = l_I0[tmpi2] + ( l_I0[1+tmpi2] - l_I0[tmpi2] ) * ( tmpd2 - tmpi2 );
			K0d = l_K0[tmpi2] + ( l_K0[1+tmpi2] - l_K0[tmpi2] ) * ( tmpd2 - tmpi2 );
			Z = p10( log10_z0 ) * complex(1.0, 1.0);
			Z = -Z * ( cln(Z) - log(2.0) + GAMMA + K0d/I0d );
		}else{
			//printf(" (3) ");
			// -z0 * ( ln(z0/2) + £^)
			Z = p10( log10_z0 ) * complex(1.0, 1.0);
			Z = -Z * ( cln(Z) - log(2.0) + GAMMA );
		}
	}else if( log10_z0 <= HIGH_z ){
		if( log10_zd < HIGH_z ){
			// original
			//printf(" (4) ");
			tmpd1 = STEPN_z * (log10_z0 - LOW_z) / (HIGH_z-(LOW_z));
			tmpd2 = STEPN_z * (log10_zd - LOW_z) / (HIGH_z-(LOW_z));
			tmpi1 = (int)tmpd1;
			tmpi2 = (int)tmpd2;
			
			I00 = l_I0[tmpi1] + ( l_I0[1+tmpi1] - l_I0[tmpi1] ) * ( tmpd1 - tmpi1 );
			I10 = l_I1[tmpi1] + ( l_I1[1+tmpi1] - l_I1[tmpi1] ) * ( tmpd1 - tmpi1 );
			K00 = l_K0[tmpi1] + ( l_K0[1+tmpi1] - l_K0[tmpi1] ) * ( tmpd1 - tmpi1 );
			K10 = l_K1[tmpi1] + ( l_K1[1+tmpi1] - l_K1[tmpi1] ) * ( tmpd1 - tmpi1 );
			I0d = l_I0[tmpi2] + ( l_I0[1+tmpi2] - l_I0[tmpi2] ) * ( tmpd2 - tmpi2 );
			K0d = l_K0[tmpi2] + ( l_K0[1+tmpi2] - l_K0[tmpi2] ) * ( tmpd2 - tmpi2 );
			
			Z = (I0d*K00 - K0d*I00) / (I0d*K10 + I10*K0d);
		}else{
			// [ (I0d/K0d)*K00 - I00 ] / [ (I0d/K0d)*K10 + I10 ]
			// if( I0d/K0d > 10^120 ) { calculate K00/K10 only }
			// I0d/K0d (no exp term) * e^2zd > 10^120
			// {log10(I0d) - log10(K0d)} + 2zd*log10(e) > 120.0
			//printf(" (5) ");
			if( log10(exp(1.0)) * p10(log10_zd) > 60.0 ){
				//printf("I0d/K0d too big ");
				tmpd1 = STEPN_z * (log10_z0 - LOW_z) / (HIGH_z-(LOW_z));
				tmpi1 = (int)tmpd1;
				K00 = l_K0[tmpi1] + ( l_K0[1+tmpi1] - l_K0[tmpi1] ) * ( tmpd1 - tmpi1 );
				K10 = l_K1[tmpi1] + ( l_K1[1+tmpi1] - l_K1[tmpi1] ) * ( tmpd1 - tmpi1 );
				Z = K00 / K10;
			}else{
				//printf("I0d/K0d small engh ");
				tmpd1 = STEPN_z * (log10_z0 - LOW_z) / (HIGH_z-(LOW_z));
				tmpi1 = (int)tmpd1;
				I00 = l_I0[tmpi1] + ( l_I0[1+tmpi1] - l_I0[tmpi1] ) * ( tmpd1 - tmpi1 );
				I10 = l_I1[tmpi1] + ( l_I1[1+tmpi1] - l_I1[tmpi1] ) * ( tmpd1 - tmpi1 );
				K00 = l_K0[tmpi1] + ( l_K0[1+tmpi1] - l_K0[tmpi1] ) * ( tmpd1 - tmpi1 );
				K10 = l_K1[tmpi1] + ( l_K1[1+tmpi1] - l_K1[tmpi1] ) * ( tmpd1 - tmpi1 );
				I0d = large_I( 0, p10( log10_zd ), 1 );
				K0d = large_K( 0, p10( log10_zd ), 1 );
				Z = (I0d/K0d*K00 - I00) / (I0d/K0d*K10 + I10);
			}
		}
	}else{
		//printf(" (6) ");
		Z0 = p10( log10_z0 ) * complex( 1.0, 1.0 );
		Zd = p10( log10_zd ) * complex( 1.0, 1.0 );
		Z = cexp( 2.0*(Zd-Z0) );
		
		K00 = large_K( 0, Z0, 0 );
		K10 = large_K( 1, Z0, 0 );
		if( isinf(Z.x) || isinf(Z.y) ) Z = K00 / K10;
		else{
			I00 = large_I( 0, Z0, 0 );
			I10 = large_I( 1, Z0, 0 );
			I0d = large_I( 0, Zd, 0 );
			K0d = large_K( 0, Zd, 0 );
			Z = (I0d*K00*Z - K0d*I00) / (I0d*K10*Z + I10*K0d);
		}
		
		//printf("%lf,%lf,%lf,%lf,%lf,%lf ,%lf,%lf,%lf,%lf,%lf,%lf - %lf,%lf ", I00.x, I00.y, I10.x, I10.y, K00.x, K00.y, K10.x, K10.y
		//, I0d.x, I0d.y, K0d.x, K0d.y, Z.x, Z.y);
	}
	
	//printf("(%lf , %lf)\n", Z.x, Z.y);
	return Z;
}

double get_ratio(double xe_w, double er){
	double s = sin(er*PI*0.5) / sin( xe_w*PI );
	return s / sqrt(s*s-1) / tan( xe_w*PI );
}

double get_L_w(double xe_w, double er){
	double s = sin(er*PI*0.5) / sin( xe_w*PI ), ye_w, Max, Min;
	ye_w = log( sqrt(s*s-1) + s ) / PI;
	Max = ( ye_w+xe_w > 0.5 ? ye_w : (0.5-xe_w) );
	Min = ye_w + (0.5-xe_w) - Max;
	double tmpm = 1 - Min / Max * Min / Max;
	int mi = int(tmpm*STEPN_z);
	return ( _E[mi] * (mi + 1 - tmpm*STEPN_z) + _E[mi+1] * (tmpm*STEPN_z - mi) ) * Max;
}

#define L_N 1000
#define FINITE_SE 15
#define SKIP 10
/*FILE * z_out;
void finite_cylinder( double f, bool use_fix_n_band, int out_format, bool to_file, bool print_freq ){
	int Li;
	double omg = 2 * f * PI;
	complex z_all, sum(0,0), tmp;
	
	for( Li = FINITE_SE ; Li < N-FINITE_SE ; Li+=SKIP ){
		sum += 1.0 / specific_Z( omg, get_ratio( _we/_w*0.5 * Li/N ), get_L_w( _we/_w*0.5 * Li/N ) );
	}
	
	z_all = (N-2*FINITE_SE) / SKIP * 2 / (sum * csqrt( complex(0,omg) ));
	if(use_fix_n_band) z_all = z_all * 2 * R_const*Temp / ( _we*N_band*C_Ox*L_channel*F_const*F_const*sqrt(D) );
	else z_all = z_all * 2 * R_const*Temp / ( Total_W*C_Ox*L_channel*F_const*F_const*sqrt(D) );
	// This calculated z_all is the overall impedance Z_diff_IDA
	
	double p_f, z_abs, phi;
	complex p_z;
	switch(out_format){
		case 0: // Freq + Overall Z
			p_f = f;
			p_z = z_all;
			break;
		case 1: // D'less freq + Z/Area
			p_f = f*_w*_w/D;
			if(use_fix_n_band){
				p_z = z_all / N_band / _we / L_channel;
			}else{
				p_z = z_all / Total_W / L_channel;
			}
			break;
		case 2: // Freq + Z*D*1e10
			p_f = f;
			if(use_fix_n_band) p_z = z_all * D * L_channel * N_band * F_const * F_const * C_Ox / (2.0*R_const*Temp);
			else p_z = z_all * D * L_channel * Total_W * F_const * F_const * C_Ox / (2.0*R_const*Temp*_we);
			break;
		case 3: // Freq + Bessel()
			p_f = f;
			p_z = 2 * (N-2*FINITE_SE) / (sum * _we);
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
	}
	if( print_freq ){
		if( to_file ) fprintf( z_out, "%lf\t", p_f );
		else printf("%lf ", p_f);
	}
	if( to_file ){
		fprintf( z_out, "%lf\t%lf\t", p_z.x, p_z.y );
		z_abs = sqrt( p_z.x*p_z.x + p_z.y*p_z.y );
		phi = atan2( p_z.y, p_z.x ) * 180 / PI;
		fprintf( z_out, "%lf\t%lf\n", z_abs, phi );
	}else printf("%lf %lf\n", p_z.x, p_z.y);
}*/

#endif

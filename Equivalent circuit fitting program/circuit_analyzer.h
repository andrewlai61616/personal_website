#ifndef CIRCUIT_ANALYZER_H
#define CIRCUIT_ANALYZER_H

#include <math.h>
#include "Z_calculate.h"
#include "src/interpolation.h"
#ifndef PI
#define PI 3.14159265359
#endif
#define todeg(x) (x*180.0/PI)
#define torad(x) (x*PI/180.0)
using namespace alglib;

/*
In order to add an element into analyzing:
	1. add new element symbol and id to comment
	2. choose if element needs power (large range, not related to phase angle) in need_pow
	3. add new element symbol to ele_str
	4. #define element fitting start(initial) value
	5. in make_param_content, add case '[SYMBOL]', id and initial value settings
	6. in how_many_parameters, add case '[SYMBOL]' and define parameter number add value
	7. in CIRCUIT_Z, add case '[SYMBOL]' and set element behaviour
*/

// R: 1, C: 2, W: 3, Q: 4, nQ: 5, Y: 6, B: 7, YO: 8, nO: 9, BO: 10, YG: 11, kG: 12
// YI: 13, w2D: 14, er: 15
const bool need_pow[] = {0, 1, 1, 1, 1, 0 /*5*/, 1, 1, 1, 0, 1 /*10*/,
							1, 1, 1, 1, 0 /*15*/};
const char* ele_str[] = { "" , "R" , "C" , "W" , "Q" , "nQ" /*5*/, "Y" , "B" , "YO" , "nO" , "BO" /*10*/,
						"YG" , "kG" , "YI" , "w2D" , "er" /*15*/};

#define R_S 3.0 // resistor
#define C_S -7.0 // capacitor
#define Q_S -7.0 // constant phase element
#define n_S 0.8
#define W_S -5.0 // infinite warburg
#define Y_S -4.0 // finite warburg
#define B_S 0.3
#define YO_S -4.0 // constant phase finite warburg
#define nO_S 0.5
#define BO_S 0.3
#define YG_S -4.0 // Gerischer
#define kG_S 1.0
#define YI_S -4.0 // IDA diffusion
#define w2D_S 0.0
#define er_S 0.5

#define Rnd_Range_Pow 3.0
#define Rnd_Range 0.2

struct Z{
	double f;
	complex z;
};

#define p10(x) pow(10.0,x)

double dabs( double x ){ return x>0.0?x:-x; }

complex exp( complex z ){
	double w = exp( z.x );
	return complex( w*cos(z.y) , w*sin(z.y) );
}

complex pow_j( double n ){ return complex( cos(n*PI*0.5), sin(n*PI*0.5) ); }

void make_param_content( const char* s, double* cin, int* e_list ){
	int i = 0, s_n = strlen(s), s_i;
	for( s_i = 0 ; s_i < s_n ; s_i++ ){
		switch(s[s_i]){
			case 'R':
				e_list[i] = 1;
				cin[i++] = R_S;
				break;
			case 'C':
				e_list[i] = 2;
				cin[i++] = C_S;
				break;
			case 'W':
				e_list[i] = 3;
				cin[i++] = W_S;
				break;
			case 'Q':
				e_list[i] = 4;
				cin[i++] = Q_S;
				e_list[i] = 5;
				cin[i++] = n_S;
				break;
			case 'O':
				e_list[i] = 6;
				cin[i++] = Y_S;
				e_list[i] = 7;
				cin[i++] = B_S;
				break;
			case 'X':
				e_list[i] = 8;
				cin[i++] = YO_S;
				e_list[i] = 9;
				cin[i++] = nO_S;
				e_list[i] = 10;
				cin[i++] = BO_S;
				break;
			case 'G':
				e_list[i] = 11;
				cin[i++] = YG_S;
				e_list[i] = 12;
				cin[i++] = kG_S;
			case 'I':
				e_list[i] = 13;
				cin[i++] = YI_S;
				e_list[i] = 14;
				cin[i++] = w2D_S;
				e_list[i] = 15;
				cin[i++] = er_S;
		}
	}
}

int how_many_parameters( const char* s ){
	int s_n = strlen(s), i, n=0;
	for( i=0;i<s_n;i++ ){
		switch(s[i]){
			case 'R':
				n++;
				break;
			case 'C':
				n++;
				break;
			case 'W':
				n++;
				break;
			case 'Q':
				n+=2;
				break;
			case 'O':
				n+=2;
				break;
			case 'X':
				n+=3;
				break;
			case 'G':
				n+=2;
				break;
			case 'I':
				n+=3;
				break;
		}
	}
	return n;
}

bool abort_fitting;
complex CIRCUIT_Z( const char* s, const real_1d_array &c, double OMEGA, bool series, int* s_i, int* c_i){
	//printf("In s = %s . c[*c_i] = %lf .\n", s, c[*c_i]);
	
	int tmpi1, tmpi2;
	double x, tmpd1, tmpd2, tmpomg;
	complex z(0,0), tmpz, tmpz2, tmpz3, tmpz4, tmpz5, tmpz6, tmpz7;
	// R(resistance) , C(capacitance) , W(Warburg)
	// Q(constant phase element) , O(finite Warburg tanh(Bsqrt(jw)))
	while( s[*s_i] ){
		if(abort_fitting){
			(*s_i)++;
			continue;
		}
		//printf("s[*s_i] = %c\n", s[*s_i]);
		switch( s[*s_i] ){
			case 'R':
				//printf("In R = %lf\n", c[*c_i]);
				if(series) z += complex(p10(c[*c_i]), 0);
				else z += complex(1.0/p10(c[*c_i]),0);
				(*c_i)++;
				//printf("Done R\n");
				break;
			case 'C':
				if(series) z += complex(0, -1.0/(OMEGA*p10(c[*c_i])) );
				else z += complex(0, OMEGA*p10(c[*c_i]));
				(*c_i)++;
				break;
			case 'W':
				tmpz = p10(c[*c_i]) * sqrt(OMEGA) * pow_j(0.5); // reciprocal of Warburg
				if(series) z += 1.0/tmpz;
				else z += tmpz;
				(*c_i)++;
				break;
			case 'Q':
				tmpz = p10(c[*c_i]) * pow(OMEGA, c[(*c_i)+1]) * pow_j( c[(*c_i)+1] ); // reciprocal of CPE
				if(series) z += 1.0/tmpz;
				else z += tmpz;
				(*c_i)+=2;
				break;
			case 'O':
				// http://www.consultrsr.net/resources/eis/diff-o.htm
				// Z(w) = 1/Y0sqrt(jw) * tanh(Bsqrt(jw))
				tmpz = p10(c[(*c_i)+1]) * sqrt(OMEGA) * pow_j(0.5); // = x
				//printf("Bsqrt(jw) = %10lf , %10lf\t", tmpz.x, tmpz.y);
				tmpz = exp( -2.0*tmpz );
				//printf("exp(x) = %10lf , %10lf\t", tmpz.x, tmpz.y);
				tmpz = (1-tmpz) / (1+tmpz);
				//printf("tanh(x) = %10lf , %10lf\t", tmpz.x, tmpz.y);
				tmpz /= ( p10(c[*c_i]) * sqrt(OMEGA) * pow_j(0.5) );
				//printf("tmpz = %10lf , %10lf\n", tmpz.x, tmpz.y);
				
				if(series) z += tmpz;
				else z += 1.0/tmpz;
				(*c_i)+=2;
				break;
			case 'X':
				// http://www.consultrsr.net/resources/eis/diff-o.htm
				// Z(w) = 1/Y0sqrt(jw) * tanh(Bsqrt(jw))
				
				//tmpz2 = pow(OMEGA, c[(*c_i)+1]) * pow_j( c[(*c_i)+1] );
				
				tmpz = p10(c[(*c_i)+2]) * pow(OMEGA, c[(*c_i)+1]) * pow_j( c[(*c_i)+1] ); // = x
				tmpz = exp( -2.0*tmpz );
				tmpz = (1-tmpz) / (1+tmpz);
				tmpz /= ( p10(c[*c_i]) * pow(OMEGA, c[(*c_i)+1]) * pow_j( c[(*c_i)+1] ) );
				
				if(series) z += tmpz;
				else z += 1.0/tmpz;
				(*c_i)+=3;
				break;
			case 'G':
				// http://www.consultrsr.net/resources/eis/gerischer.htm
				// Z(w) = 1/Y0sqrt(k+jw)
				
				tmpd1 = p10(c[(*c_i)+1]);
				tmpd2 = sqrt(OMEGA*OMEGA + tmpd1*tmpd1) ;
				tmpd1 = 0.5*tmpd1/tmpd2;
				
				tmpz = sqrt( 0.5 + tmpd1 ) + sqrt( 0.5 - tmpd1 ) * complex(0.0, 1.0);
				tmpz = p10(c[*c_i]) * sqrt(tmpd2) * tmpz;
				
				if(series) z += 1.0/tmpz;
				else z += tmpz;
				(*c_i)+=2;
				break;
			case 'I':
				//log10(Y0) , log10(w2D)  , er  
				//c[*c_i]   , c[(*c_i)+1] , c[(*c_i)+2]
				
				//int tmpi1, tmpi2;
				//double tmpd1, tmpd2, tmpomg;
				//complex tmpz, tmpz2, tmpz3, tmpz4, tmpz5, tmpz6, tmpz7;
				
				/*
				1. 要有 get_r get_L_w function <- elliptic integral table needed
				2. 要有 summation <- bessel function table needed
				3. 
				*/
				
				if( c[(*c_i)+2] <= 0.001 || c[(*c_i)+2] >= 0.999 ){
					printf("er out of bounds!! (%lf) Abort fitting.\n", c[(*c_i)+2]);
					abort_fitting = true;
					(*c_i)+=3;
					continue;
				}
				tmpz = complex(0,0);
				for( int Li = FINITE_SE ; Li < L_N-FINITE_SE ; Li+=SKIP ){
					tmpd1 = c[(*c_i)+2]*0.5 * Li/L_N;
					tmpz2 = specific_Z	( OMEGA,
										  get_ratio( tmpd1, c[(*c_i)+2] ),
										  get_L_w( tmpd1, c[(*c_i)+2] ) ,
										  p10( c[(*c_i)+1] )
										);
										
					if(isnan(tmpz2.x) || isnan(tmpz2.y) || isinf(tmpz2.x) || isinf(tmpz2.y)){
						printf("Wait!");
						printf("(OMG=%lf, 0,1,2 = %lf , %lf , %lf)\n", OMEGA, c[*c_i], c[(*c_i)+1], c[(*c_i)+2]);
						printf("d1 = %lf , tmpz2 = %lf , %lf\n", tmpd1, tmpz2.x, tmpz2.y);
						while(1);
					}
					
					#ifdef DEBUG
					//printf("d1 = %lf , tmpz2 = %lf , %lf\n", tmpd1, tmpz2.x, tmpz2.y);
					#endif
					tmpz += 1.0 / tmpz2;
				}

				tmpz = (L_N-2*FINITE_SE) / SKIP * 2.0 / ( tmpz * p10(c[*c_i]) * sqrt(OMEGA) * pow_j(0.5) );
				#ifdef DEBUG
				//printf("%lf,%lf,%lf,%lf,%lf\n", c[*c_i], c[(*c_i)+1], c[(*c_i)+2], tmpz.x, tmpz.y);
				#endif
				
				if(series) z += tmpz;
				else z += 1.0/tmpz;
				(*c_i)+=3;
				break;
			case '(':
				(*s_i)++;
				tmpz = CIRCUIT_Z(s, c, OMEGA, !series, s_i, c_i);
				if(series) z += tmpz;
				else z += 1.0 / tmpz;
				break;
			case ')':
				if(series) return z;
				else return 1.0 / z;
				break;
		}
		#ifdef DEBUG
		//printf("Add *s_i now z = (%lf, %lf)\n", z.x, z.y);
		#endif
		(*s_i)++; // note: need to manually add *s_i in same layer
	}
	//printf("Out of while loop returning...\n.");
	#ifdef DEBUG
	printf(" -> final z = %lf,%lf\n", z.x, z.y);
	#endif
	if(isnan(z.x) || isnan(z.y) || isinf(z.x) || isinf(z.y)){
		printf("Detected NaN or Inf! -> final z = %lf,%lf\n", z.x, z.y);
		abort_fitting = true;
		//while(1);
	}
	if(series) return z;
	else return 1.0/z;
}

#endif

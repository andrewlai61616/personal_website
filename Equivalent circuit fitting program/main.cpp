//#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

//#define DEBUG

#include "src/interpolation.h"
#include "src/integration.h"
using namespace alglib;

#include "complex_functions.h"
#include "Z_calculate.h" 
#include "circuit_analyzer.h"
#include "CHI.h"

#define abs_to_ph_weight_const 1.0
#define trial_times 100
#define succeed_times 10

#define MAX_CIRCUIT_STR 20
#define MAX_DATA_POINTS 1010
Z Z_l[MAX_DATA_POINTS];
int nZ_l, nCIR;

real_1d_array best_c_fit, c_fit, c_init, zero_l;
real_2d_array omega_l;
double nothing_double[MAX_DATA_POINTS];
char circuit_str[MAX_CIRCUIT_STR];
int elements[MAX_CIRCUIT_STR];
void init_all(){
	srand(time(NULL));
	get_txt_list();
	
	nCIR = how_many_parameters( circuit_str );
	printf("Parameter # = %d\n", nCIR);
	c_fit.setcontent( nCIR, nothing_double );
	c_init.setcontent( nCIR, nothing_double );
	make_param_content( circuit_str, nothing_double, elements );
	for( int i=0;i<nCIR;i++ ) c_init[i] = nothing_double[i];
}

double err_sqr_re_im( double re_x, double re_y, double im_x, double im_y ){
	//double err_re = sqrt(dabs(re_x)) - sqrt(dabs(re_y));
	//double err_im = sqrt(dabs(im_x)) - sqrt(dabs(im_y));
	
	//err_re = log10(dabs(re_x)) - log10(dabs(re_y));
	//err_im = log10(dabs(im_x)) - log10(dabs(im_y));
	
	double wRe, wIm;
	wRe = 1 / (re_y>0.0?re_y:-re_y);
	wIm = 1 / (im_y>0.0?im_y:-im_y);
	
	return wRe * pow(re_x-re_y, 2.0) + wIm * pow(im_x-im_y, 2.0);
}

int call_count;
void func_err(const real_1d_array &c, const real_1d_array &x, double &err_sqr, void *ptr){
	call_count++; 
	int s_i=0, c_i=0;
	//printf("Start %s\n", c.tostring(8).c_str());
	complex z = CIRCUIT_Z( circuit_str, c, x[0], 1, &s_i, &c_i);
	
	// use fit |Z| and phase method
	/*double err_z = log10(abscomplex(z)) - log10(x[1]);
	double err_p = atan2( z.y , z.x ) - x[2];
	err_sqr = abs_to_ph_weight_const*err_z*err_z + err_p*err_p;*/
	
	// use fit Re(Z) and Im(Z) method
	err_sqr = err_sqr_re_im( z.x, x[1], z.y, x[2] );
	
	/*if( !(call_count % 1000) )
		printf("# = %5d\terr_eqr = %lf\n", call_count, err_sqr);*/
	//printf("(%lf) %lf,\t%lf err = %lf,\t%lf,\t%lf\n", x[0]/(2.0*PI), z.x, z.y, err_z, err_p, err_sqr);
}

int main(int argc, char **argv){
	srand(time(NULL));
	srand(rand()*rand());
	initialize_bessel_functions();
	initialize_elliptic_integral();
						
	int i,j,k,try_i,succeed;
	double epsx = 0.000001, diffstep = 0.00001;
    ae_int_t maxits = 0, info;
    lsfitstate state;
    lsfitreport rep;
	
	//printf("Hi! Welcome to ALL~YOU~CAN~FIT equivalent circuit fitting program.\n\n");
	printf("You can use symbols \"R\", \"C\", \"W\", \"Q\", \"O\", \"X\" and \"I\".\n");
	printf("I: IDA diffusion element (cylindrical finite diffusion)\n");
	printf("Q: constant phase element. O: finite warburg. X: constant phase finite warburg.\n\n");
	printf("This program finds all the .txt files in this folder that are \"of the format of CHI outputs\" and\n");
	printf("tries to find the equivalent circuit using Levenberg-Marquardt non-linear fitting method.\n\n");
	printf("¡°Note: series R,C -> RC , combo R,C -> (RC) , series in combo -> (R(RC)) , combo in series -> R(RC)\n");
	printf("¡°Note: And please, no spaces.\n\n");
	printf("Insert the circuit: ");
	scanf("%s", circuit_str);
	for( i=0;i<strlen(circuit_str);i++ ) circuit_str[i] = toupper( circuit_str[i] );
	putchar('\n');
	
	int s_i_last, c_i_last;
	complex z_last;
	double best_MSE, mean_sqr_err, err_z_last, err_p_last, err_re_last, err_im_last;
	double rnd_x;
	init_all();
	char tmpstr[100];
	FILE * summary = fopen("summary.csv","w");
	for( j = 0 ; j < nCIR ; j++ ) fprintf( summary, ",%s", ele_str[ elements[j] ] );
	fprintf(summary, ",MSE\n");
	for( i=0 ; i<filen ; i++ ){
		puts("");
		nZ_l = read_txt( i, Z_l );
		
		best_MSE = 1e9;
		succeed = 0;
		for( try_i=1; try_i<=trial_times; try_i++ ){
			printf("Trial #%d / %d:\n", try_i, trial_times);
			
			// set initial contents
			for( j=0;j<nCIR;j++ ){
				// trial several times each with a random bias value
				rnd_x = (double)(((rand()*rand())%1000)-500) * 0.0001;
				if( need_pow[ elements[j] ] ) rnd_x *= Rnd_Range_Pow;
				else rnd_x *= Rnd_Range;
				c_fit[j] = c_init[j] + rnd_x;
			}
			omega_l.setcontent( nZ_l, 3, nothing_double );
			for( j=0;j<nZ_l;j++ ){
				omega_l[j][0] = 2.0*PI*Z_l[j].f;
				
				// use fit |Z| and phase method
				/*omega_l[j][1] = abscomplex(Z_l[j].z);
				omega_l[j][2] = atan2( Z_l[j].z.y , Z_l[j].z.x );*/
				
				// use fit Re(Z) and Im(Z) method
				omega_l[j][1] = Z_l[j].z.x;
				omega_l[j][2] = Z_l[j].z.y;
			}
			zero_l.setcontent( nZ_l, nothing_double );
			for( j=0;j<nZ_l;j++ ) zero_l[j] = 0.0;
			
			//fitting
			abort_fitting = false;
			call_count = 0;
		    printf("Start %s\n", c_fit.tostring(8).c_str());
		    lsfitcreatef(omega_l, zero_l, c_fit, diffstep, state);
		    lsfitsetcond(state, epsx, maxits);
		    alglib::lsfitfit(state, func_err);
		    if( abort_fitting ) continue;
		    succeed ++;
		    lsfitresults(state, info, c_fit, rep);
		    printf("Final %s , Info %d\n", c_fit.tostring(8).c_str(), info);
		    
		    // error calculation
		    mean_sqr_err = 0.0;
		    for( j=0;j<nZ_l;j++ ){
		    	s_i_last = c_i_last = 0;
				z_last = CIRCUIT_Z( circuit_str, c_fit, omega_l[j][0], 1, &s_i_last, &c_i_last);
				
				mean_sqr_err += err_sqr_re_im( z_last.x, omega_l[j][1], z_last.y, omega_l[j][2] );
			}
			mean_sqr_err /= nZ_l;
			if(best_MSE > mean_sqr_err){
				best_MSE = mean_sqr_err;
				best_c_fit = c_fit;
			}
			printf("Mean square error = %lf , Best = %lf\n", mean_sqr_err, best_MSE);
			
			if( succeed == succeed_times ) break;
		}
		
		// output results
		abort_fitting = false;
		strcpy( tmpstr, filename[i] );
		tmpstr[ strlen(tmpstr)-4 ] = '\0';
		fprintf(summary, "'%s", tmpstr);
		for( j = 0 ; j < nCIR ; j++ ){
			if( need_pow[ elements[j] ] ) fprintf( summary, ",%e", p10(best_c_fit[j]) );
			else fprintf( summary, ",%e", best_c_fit[j] );
		}
		fprintf(summary, ",%e\n", best_MSE);
		output_csv( i , Z_l , best_c_fit , nZ_l, best_MSE , circuit_str , call_count, nCIR, elements );
	}
	fclose(summary);
	printf("DONE\n");
    while(1);
    return 0;
}

/* info
    * -8    optimizer   detected  NAN/INF  in  the  target
            function and/or gradient
    * -7    gradient verification failed.
            See LSFitSetGradientCheck() for more information.
    * -3    inconsistent constraints
    *  2    relative step is no more than EpsX.
    *  5    MaxIts steps was taken
    *  7    stopping conditions are too stringent,
            further improvement is impossible
*/

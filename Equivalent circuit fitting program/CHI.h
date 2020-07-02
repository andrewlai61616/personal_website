#include <dirent.h>
#include <math.h> 
#include <stdio.h>
#include "src/interpolation.h"
#include "circuit_analyzer.h"

#define MAX_FILES 500
char filename[MAX_FILES][FILENAME_MAX], currdir[FILENAME_MAX], tmpname[FILENAME_MAX];
int filen;

bool got_txt = false;
int get_txt_list(){
	got_txt = true;
	DIR *dir;
	struct dirent *ent;
	int filenamen;
	
	_getcwd( currdir, FILENAME_MAX );
	filen = 0;
	if ((dir = opendir (currdir)) != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir (dir)) != NULL) {
			filenamen = strlen(ent->d_name);
			if(filenamen>=5&&strcmp(((ent->d_name)+filenamen-3), "txt")==0){
				strcpy(filename[filen], ent->d_name);
				//printf ("%s\n", ent->d_name);
				filen++;
			}
		}
		closedir (dir);
		printf("There are %d txt files in current directory.\n",filen);
		return 0;
	} else {
	  	/* could not open directory */
	  	perror ("");
	  	return 1;
	}
}
	
int read_txt( int filei, Z* Z_l ){
	if( !got_txt ){
		printf("You haven't read the txt file list in the directory!\n");
		return 0;
	}
	char wholefname[FILENAME_MAX], tmpstr[FILENAME_MAX];
	strcpy(wholefname, currdir);
	strcat(wholefname, "\\");
	strcat(wholefname, filename[filei]);
	
	FILE *csvFile;
	csvFile = fopen (wholefname,"r");
	printf("Processing file : %s (%d of %d)\n",filename[filei],filei+1,filen);
	
	// P????/??g
	while(~fscanf(csvFile,"%s",tmpstr)){
		if(strlen(tmpstr)==9 || strlen(tmpstr)==10)
			if(tmpstr[0]=='P'&&tmpstr[5]=='/'&&tmpstr[8]=='g')
				break;
	}
	
	int nf;
	double freq, inz, inp;
	for(nf=0;;nf++){
		if( fscanf(csvFile,"%lf%*lf%*lf%lf%lf",&Z_l[nf].f,&inz,&inp) == EOF ) break;
		if(inp < -90.0)inp = -90.0;
		else if(inp > 0.0) inp = -(1e-3);
		Z_l[nf].z = complex( inz*cos( torad(inp) ) , inz*sin( torad(inp) ) );
	}
	printf("Total %d points.\n", nf);
	
	return nf;
}

void output_csv( int filei, Z* Z_list, const real_1d_array &c_list, int nZ_list, double MSE, char* C_STR, int ITER, int nCIR, int* e_list ){
	
	FILE *csvFile;
	
	strcpy(tmpname, filename[filei]);
	strcpy(tmpname + strlen(tmpname) - 3, "csv");
	printf("Writing to \"%s\"\n", tmpname);
	csvFile = fopen( tmpname, "w" );
	
	tmpname[ strlen(tmpname)-4 ] = '\0';
	fprintf( csvFile, "Name,%s\nCircuit,%s\nMSE,%10lf\nIterations,%d\nParams:\n", tmpname, C_STR,MSE, ITER );
	complex z;
	int j, s_i, c_i;
	for( j = 0 ; j < nCIR ; j++ ) fprintf( csvFile, ",%s", ele_str[ e_list[j] ] );
	fprintf( csvFile, "\n");
	for( j = 0 ; j < nCIR ; j++ ){
		if( need_pow[ e_list[j] ] ) fprintf( csvFile, ",%e", p10(c_list[j]) );
		else fprintf( csvFile, ",%e", c_list[j] );
	}
	fprintf( csvFile, "\n\nFit\nFreq (Hz),Re(Z) (£[),Im(Z) (£[),|Z| (£[),Phase (deg),Org |Z| (£[),Org Phase (deg)\n" );
    for(  j = 0 ; j < nZ_list ; j++ ){
    	s_i = c_i = 0;
		z = CIRCUIT_Z( C_STR, c_list, 2.0*PI*Z_list[j].f, 1, &s_i, &c_i);
		
		fprintf( csvFile, "%lf,%lf,%lf,%lf,%lf,%lf,%lf\n", Z_list[j].f , z.x , z.y, abscomplex(z), atan2(z.y,z.x)*180.0/PI,
			abscomplex( Z_list[j].z ), atan2( Z_list[j].z.y, Z_list[j].z.x )*180.0/PI );
	}
	
	fclose( csvFile );
}

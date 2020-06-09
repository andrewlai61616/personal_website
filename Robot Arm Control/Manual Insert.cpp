#include<cstdio>
#include<cstring>
#include "include/RPY_RotMatrix_EqvAxisAngle_Quaternion.h"
#include "include/Transformation_Matrix.h"
#include "include/Forward_Kinematics.h"
#include "include/IRB140_ABB.h"

#define MAX_TURN_DEG 180.0
#define MAX_POINTS 100000
#define FACTOR 352.0/138.0

int count_points = 0;

char output[100000];
void fprintf_output(FILE **outFile, THETA_ABB b, double sec){
	fprintf(*outFile, "l[%d][0]=%lf,l[%d][1]=%lf,l[%d][2]=%lf,l[%d][3]=%lf,l[%d][4]=%lf,l[%d][5]=%lf;\n",
	count_points,b.t1 todeg,count_points,b.t2 todeg,count_points, b.t3 todeg,count_points,b.t4 todeg,count_points,b.t5 todeg,count_points,b.t6 todeg);
	count_points++;
	//fprintf(*outFile,"self.move_arm_smooth(%lf,%lf,%lf,%lf,%lf,%lf,%lf)\n",b.t1,b.t2,b.t3,b.t4,b.t5,b.t6,sec);
}

int inverse_kinematics(THETA_ABB** *ans, double input_x, double input_y, double input_z, double input_roll, double input_pitch, double input_yaw){	
	// Disregards factor from simulation program. Uncomment if necessary.
	// input_x*=FACTOR; input_y*=FACTOR; input_z*=FACTOR;
	RPY rpy1(input_roll torad,input_pitch torad,input_yaw torad);
	RotMatrix r1 = RotfromRPY(rpy1);
	Vector3 v1(input_x, input_y, input_z);
	TransMatrix t1(r1, v1, Vector3(0,0,0), 1);
	
	return ABB_PIEPER_06(t1, *ans);
}

bool is0(double x){return abs(x) < 1e-5;}

void find_valid_output(FILE **outFile, THETA_ABB** ans, double sec){
	int i,j;
	for(i=1;i<=4;i++){
		if(!ans[i][1].valid)continue;
		for(j=1;j<=2;j++){
			if(i==1&&j==1)continue;
			if(angle_max(ans[i][j], MAX_TURN_DEG torad)){
				printf("\tFound valid answer : theta = \n");
				ans[i][j].printout();
				break;
			}
		}
		if(j<=2)break;
	}
	if(i==5)
		printf("Cannot find valid solution for this point!\nSkipping to next point\n");
	else
		fprintf_output(outFile, ans[i][j], sec);
}

double ix[MAX_POINTS], iy[MAX_POINTS], iz[MAX_POINTS];
double iro[MAX_POINTS], ipi[MAX_POINTS], iya[MAX_POINTS], isec[MAX_POINTS];
int main(){
	
	//make_output(0.1,0.2,2,4,54,1,5);
	int i,j,n;
	
	printf("\nYou can manually enter number of points (n).\nAfterwards enter \'x y z roll pitch yaw seconds\' for each point.\n");
	printf("If you do not want to rotate the tip then: Roll = 180, Pitch = -90, Yaw = 0.\n\n");
	printf("Please enter number of points n: ");
	scanf("%d",&n);
	for(i=1;i<=n;i++){
		printf("\n[Point %d]\nx y z = ",i);
		scanf("%lf%lf%lf",&ix[i],&iy[i],&iz[i]);
		printf("roll pitch yaw (degrees) = ");
		scanf("%lf%lf%lf",&iro[i],&ipi[i],&iya[i]);
		printf("Walk seconds = ");
		scanf("%lf",&isec[i]);
	}
	
	THETA_ABB** ans = new THETA_ABB*[5];
	for(int i = 1; i <= 4; ++i)
	    ans[i] = new THETA_ABB[3];
	
	printf("Opening Manual_Insert.txt ... ");
	FILE * outFile;
	outFile = fopen("Manual_Insert.txt","w");
	
	double x,y,z;
	double roll, pitch, yaw;
	int soln;
	
	for(i=1;i<=n;i++){
		printf("Calculating inverse kinematics for point %d:\n",i);
		x = ix[i], y = iy[i], z = iz[i];
		roll = iro[i], pitch = ipi[i], yaw = iya[i];
		soln = inverse_kinematics(&ans, x, y, z, roll, pitch, yaw);
		find_valid_output(&outFile, ans, isec[i]);
	}
	
	fclose(outFile);
	
	printf("Done. Manual_Insert.txt has been outputted.\nPress Enter to Exit program.");
	getchar();
	
	return 0;
}


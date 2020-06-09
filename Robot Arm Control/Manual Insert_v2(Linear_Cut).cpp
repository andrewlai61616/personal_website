#include<cstdio>
#include<cstring>
#include "include/RPY_RotMatrix_EqvAxisAngle_Quaternion.h"
#include "include/Transformation_Matrix.h"
#include "include/Forward_Kinematics.h"
#include "include/IRB140_ABB.h"

#define DIFF_DIS 1
#define MAX_DIFF_DEG 5.0
#define MAX_TURN_DEG 170.0
#define MAX_POINTS 100000
#define FACTOR 352.0/138.0

char output[100000];
int count_points = 0;

void fprintf_output(FILE **outFile, THETA_ABB b, double sec){
	fprintf(*outFile, "l[%d][0]=%lf,l[%d][1]=%lf,l[%d][2]=%lf,l[%d][3]=%lf,l[%d][4]=%lf,l[%d][5]=%lf;\n",
	count_points,b.t1 todeg,count_points,b.t2 todeg,count_points, b.t3 todeg,count_points,b.t4 todeg,count_points,b.t5 todeg,count_points,b.t6 todeg);
	count_points++;
	//fprintf(*outFile,"        self.move_arm_smooth(%lf,%lf,%lf,%lf,%lf,%lf,%lf)\n",b.t1,b.t2,b.t3,b.t4,b.t5,b.t6,sec);
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

THETA_ABB last_t;
bool find_valid_output(int ansi, FILE **outFile, THETA_ABB** ans, double sec){
	int i,j;
	bool valid[5][3];
	for(i=1;i<=4;i++){
		valid[i][1] = valid[i][2] = 0;
		if(!ans[i][1].valid)continue;
		for(j=1;j<=2;j++){
			if(angle_max(ans[i][j], MAX_TURN_DEG torad)){
				
				/*if(ans[i][j].t2<0.517){
					printf("AHHHHHHHHHHHHHHHHHH ");
					ans[i][j].printout();
				}*/
				
				if(ansi==1)valid[i][j] = 1;
				else if(angle_max((ans[i][j]-last_t), MAX_DIFF_DEG torad)){ // 這邊看有沒有辦法轉很小的角度 
					printf("last(%.3lf, %.3lf, %.3lf, %.3lf, %.3lf, %.3lf) ",last_t.t1,last_t.t2,last_t.t3,last_t.t4,last_t.t5,last_t.t6);
					/*if(ansi==3)*/printf("now(%.3lf, %.3lf, %.3lf, %.3lf, %.3lf, %.3lf)\n\n",ans[i][j].t1,ans[i][j].t2,ans[i][j].t3,ans[i][j].t4,ans[i][j].t5,ans[i][j].t6);
					last_t = ans[i][j];
					fprintf_output(outFile, ans[i][j], sec);
					return 0;
				}
			}
		}
	}
	if(ansi==1){
		printf("Wait!! Please pick a valid start position for the start point:\n");
		for(i=1;i<=4;i++){
			for(j=1;j<=2;j++){
				if(valid[i][j]){
					printf("For i = %d , j = %d. ",i,j);
					ans[i][j].printout();
				}
			}
		}
		printf("Enter i j: ");
		int ini, inj;
		scanf("%d%d",&ini,&inj);
		if(ini<=4&&ini>=1&&inj<=2&&inj>=1 && valid[ini][inj]){
			printf("Great! The robot will start at the chosen position.\n\n");
			last_t = ans[ini][inj];
			fprintf_output(outFile, ans[ini][inj], sec);
			return 0;
		}else{
			printf("Not a valid input. Byebye...");
			getchar();
			getchar();
			return 1;
		}
	}
	printf("Cannot find valid solution for this point!\nProgram will close. Byebye...\n");
	getchar();
	getchar();
	return 1;
	
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
	
	printf("Opening Manual_Insert_v2.txt ... ");
	FILE * outFile;
	outFile = fopen("Manual Insert_v2(Linear_Cut).txt","w");
	
	double x,y,z;
	double roll, pitch, yaw;
	int soln;
	
	double dis, diff_sec;
	int cut_points;
	
	for(i=1;i<=n;i++){
		printf("Calculating inverse kinematics for point %d:\n",i);
		if(i==1){
			x = ix[i], y = iy[i], z = iz[i];
			roll = iro[i], pitch = ipi[i], yaw = iya[i];
			soln = inverse_kinematics(&ans, x, y, z, roll, pitch, yaw);
			if(find_valid_output(i, &outFile, ans, isec[i]))return 0;
		}else{
			printf("From\n(%.3lf, %.3lf, %.3lf, %.3lf, %.3lf, %.3lf) to (%.3lf, %.3lf, %.3lf, %.3lf, %.3lf, %.3lf)\n\n", ix[i-1],iy[i-1],iz[i-1],iro[i-1],ipi[i-1],iya[i-1],ix[i],iy[i],iz[i],iro[i],ipi[i],iya[i]);
			dis = sqrt( (ix[i]-ix[i-1])*(ix[i]-ix[i-1]) + (iy[i]-iy[i-1])*(iy[i]-iy[i-1]) + (iz[i]-iz[i-1])*(iz[i]-iz[i-1]) );
			cut_points = int(dis / DIFF_DIS);
			diff_sec = isec[i] / cut_points;
			for(j=1;j<=cut_points;j++){
				x = ix[i-1] + (ix[i]-ix[i-1])*j/cut_points, y = iy[i-1] + (iy[i]-iy[i-1])*j/cut_points, z = iz[i-1] + (iz[i]-iz[i-1])*j/cut_points;
				roll = iro[i-1] + (iro[i]-iro[i-1])*j/cut_points;
				pitch = ipi[i-1] + (ipi[i]-ipi[i-1])*j/cut_points;
				yaw = iya[i-1] + (iya[i]-iya[i-1])*j/cut_points;
				soln = inverse_kinematics(&ans, x, y, z, roll, pitch, yaw);
				printf("xyzrpy(%.3lf, %.3lf, %.3lf, %.3lf, %.3lf, %.3lf) ", x,y,z,roll,pitch,yaw);
				if(find_valid_output(i, &outFile, ans, diff_sec)){
					fclose(outFile);
					return 0;
				} 
			}
			/*//Only for point 3
			if(i==3){
				x = ix[i], y = iy[i], z = iz[i];
				roll = iro[i], pitch = ipi[i], yaw = iya[i];
				soln = inverse_kinematics(&ans, x, y, z, roll, pitch, yaw);
				for(int k=1;k<=4;k++){
					for(j=1;j<=2;j++){
						fprintf_output(&outFile, ans[k][j], 0.5);
					}
				}
			}
			//*/
		}
	}
	
	fclose(outFile);
	
	printf("Done. Manual_Insert.txt has been outputted.\nPress Enter to Exit program.");
	getchar();
	
	return 0;
}


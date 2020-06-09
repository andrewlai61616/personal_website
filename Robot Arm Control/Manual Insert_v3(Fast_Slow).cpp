#include<cstdio>
#include<cstring>
#include "include/RPY_RotMatrix_EqvAxisAngle_Quaternion.h"
#include "include/Transformation_Matrix.h"
#include "include/Forward_Kinematics.h"
#include "include/IRB140_ABB.h"

#define DIFF_DIS 2
#define MAX_DIFF_DEG 5.0
#define MAX_TURN_DEG 150.0
#define MAX_POINTS 100000
#define FACTOR 352.0/138.0

char output[100000];
void fprintf_output(FILE **outFile, THETA_ABB b, double sec){
	fprintf(*outFile,"self.move_arm_smooth(%lf,%lf,%lf,%lf,%lf,%lf,%lf)\n",b.t1,b.t2,b.t3,b.t4,b.t5,b.t6,sec);
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
	int opi, opj;
	double op_diff_ans = 1e9, diff_tmp;
	bool valid[5][3];
	for(i=1;i<=4;i++){
		valid[i][1] = valid[i][2] = 0;
		if(!ans[i][1].valid)continue;
		for(j=1;j<=2;j++){
			if(angle_max(ans[i][j], MAX_TURN_DEG torad)){
				
				if(ansi==1)valid[i][j] = 1;
				else{ // 這邊找差異最小的角度 
					diff_tmp = (ans[i][j].t1-last_t.t1)*(ans[i][j].t1-last_t.t1) + (ans[i][j].t2-last_t.t2)*(ans[i][j].t2-last_t.t2) + 
							   (ans[i][j].t3-last_t.t3)*(ans[i][j].t3-last_t.t3) + (ans[i][j].t4-last_t.t4)*(ans[i][j].t4-last_t.t4) + 
							   (ans[i][j].t5-last_t.t5)*(ans[i][j].t5-last_t.t5) + (ans[i][j].t6-last_t.t6)*(ans[i][j].t6-last_t.t6);
					if(diff_tmp < op_diff_ans){
						opi = i;
						opj = j;
						op_diff_ans = diff_tmp;
					}
				}
			}
		}
	}
	
	if(ansi!=1){
		last_t = ans[opi][opj];
		fprintf_output(outFile, ans[opi][opj], sec);
		printf("last(%.3lf, %.3lf, %.3lf, %.3lf, %.3lf, %.3lf) ",last_t.t1,last_t.t2,last_t.t3,last_t.t4,last_t.t5,last_t.t6);
		printf("now(%.3lf, %.3lf, %.3lf, %.3lf, %.3lf, %.3lf)\n\n",ans[opi][opj].t1,ans[opi][opj].t2,ans[opi][opj].t3,ans[opi][opj].t4,ans[opi][opj].t5,ans[opi][opj].t6);
		return 0;
	}

	
	if(ansi==1){
		printf("Wait!! Please pick a valid start position for the start point:\n\n");
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

int savedn;
double saved_x[MAX_POINTS], saved_y[MAX_POINTS], saved_z[MAX_POINTS];
double saved_r[MAX_POINTS], saved_p[MAX_POINTS], saved_ya[MAX_POINTS];
int CUT(double dis, double x1, double y1, double z1, double r1, double p1, double ya1, double x2, double y2, double z2, double r2, double p2, double ya2, int dir){
	if(dis<=DIFF_DIS){
		//saved_x[savedn] = x2, saved_y[savedn] = y2, saved_z[savedn] = z2;
		//saved_r[savedn] = r2, saved_p[savedn] = p2, saved_ya[savedn] = ya2;
		//savedn++;
		return 0; // 已經在外層插入這個點了 所以不用再新增點的數量 
	}
	int point_count = 0;
	double xm = (x1+x2)/2, ym = (y1+y2)/2 , zm = (z1+z2)/2, rm = (r1+r2)/2, pm = (p1+p2)/2, yam = (ya1+ya2)/2, dis2 = dis/2;
	if(dir==2){
		// Cal Front
		point_count += CUT(dis2, x1, y1, z1, r1, p1, ya1, xm, ym, zm, rm, pm, yam, 0);
		
		// Add mid
		saved_x[savedn] = xm, saved_y[savedn] = ym, saved_z[savedn] = zm;
		saved_r[savedn] = rm, saved_p[savedn] = pm, saved_ya[savedn] = yam;
		savedn++;
		point_count++;
		
		// Cal Back
		point_count += CUT(dis2, xm, ym, zm, rm, pm, yam, x2, y2, z2, r2, p2, ya2, 1);
		
		// Add Back
		saved_x[savedn] = x2, saved_y[savedn] = y2, saved_z[savedn] = z2;
		saved_r[savedn] = r2, saved_p[savedn] = p2, saved_ya[savedn] = ya2;
		savedn++;
		point_count++;
	}else if(!dir){
		// Cal Front
		point_count += CUT(dis2, x1, y1, z1, r1, p1, ya1, xm, ym, zm, rm, pm, yam, 0);
		
		// Add mid
		saved_x[savedn] = xm, saved_y[savedn] = ym, saved_z[savedn] = zm;
		saved_r[savedn] = rm, saved_p[savedn] = pm, saved_ya[savedn] = yam;
		savedn++;
		point_count++;
	}else{
		// Add mid
		saved_x[savedn] = xm, saved_y[savedn] = ym, saved_z[savedn] = zm;
		saved_r[savedn] = rm, saved_p[savedn] = pm, saved_ya[savedn] = yam;
		savedn++;
		point_count++;
		
		// Cal Back
		point_count += CUT(dis2, xm, ym, zm, rm, pm, yam, x2, y2, z2, r2, p2, ya2, 1);
	}
	return point_count;
}

int main(){
	
	//make_output(0.1,0.2,2,4,54,1,5);
	int i,j,n;
	
	printf("\nYou can manually enter number of points (n).\nAfterwards enter \'x y z roll pitch yaw seconds\' for each point.\n");
	printf("If you do not want to rotate the tip then: Roll = 180, Pitch = -90, Yaw = 0.\n");
	printf("This is the 3rd Version of Manual_Insert! It will cut n points between point i and i+1.\n");
	printf("What will be n? n will be the minimum number of points that makes the closest 2 cut point's distance < DIFF_DIS.\n");
	printf(" ---> ... .   .       .       .   . ... <---\n\n");
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
	
	printf("Opening Manual Insert_v3(Fast_Slow).txt ... \n\n");
	FILE * outFile;
	outFile = fopen("Manual Insert_v3(Fast_Slow).txt","w");
	
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
			
			savedn = 0;
			cut_points = CUT(dis, ix[i-1], iy[i-1], iz[i-1], iro[i-1], ipi[i-1], iya[i-1], ix[i],iy[i],iz[i], iro[i], ipi[i], iya[i], 2);
			printf("Got cut points = %d, savedn = %d\n",cut_points,savedn);
			diff_sec = isec[i] / cut_points;
			for(j=0;j<savedn;j++){
				x = saved_x[j], y = saved_y[j], z = saved_z[j], roll = saved_r[j], pitch = saved_p[j], yaw = saved_ya[j];
				soln = inverse_kinematics(&ans, x, y, z, roll, pitch, yaw);
				printf("xyzrpy(%.3lf, %.3lf, %.3lf, %.3lf, %.3lf, %.3lf) ", x,y,z,roll,pitch,yaw);
				if(find_valid_output(i, &outFile, ans, diff_sec)){
					fclose(outFile);
					return 0;
				}
				
				//printf("%.2lf\t%.2lf\t%.2lf\t%.2lf\t%.2lf\t%.2lf\n", saved_x[j], saved_y[j], saved_z[j], saved_r[j], saved_p[j], saved_ya[j]);
			}
			
			/*
			//cut_points = int(dis / DIFF_DIS);
			diff_sec = isec[i] / cut_points;
			for(j=1;j<=cut_points;j++){
				
				soln = inverse_kinematics(&ans, x, y, z, roll, pitch, yaw);
				printf("xyzrpy(%.3lf, %.3lf, %.3lf, %.3lf, %.3lf, %.3lf) ", x,y,z,roll,pitch,yaw);
				if(find_valid_output(i, &outFile, ans, diff_sec)){
					fclose(outFile);
					return 0;
				}
			}*/
		}
	}
	
	fclose(outFile);
	
	printf("Done. Manual_Insert.txt has been outputted.\nPress Enter to Exit program.");
	getchar();
	
	return 0;
}


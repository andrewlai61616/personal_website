#define IRB140_ABB_H

#include<cstdio>
#include<cstring>
#include<cmath>

#ifndef FORWARD_KINEMATICS_H
#include "Forward_Kinematics.h"
#endif

#ifndef PIEPER_GENERAL_H
#include "Pieper_General.h"
#endif

struct THETA_ABB{
	bool valid;
	double t1, t2, t3, t4, t5, t6;
	// constructor
	THETA_ABB(){}
	THETA_ABB(double t1i, double t2i, double t3i, double t4i, double t5i, double t6i){
		t1=t1i; t2=t2i; t3=t3i; t4=t4i; t5=t5i; t6=t6i;
	}
	// assign
	THETA_ABB& operator=(const THETA_ABB& b){
		t1=b.t1; t2=b.t2; t3=b.t3; t4=b.t4; t5=b.t5; t6=b.t6;
	}
	// equal
	bool operator==(const THETA_ABB& b){
		return (t1==b.t1)&&(t2==b.t2)&&(t3==b.t3)&&(t4==b.t4)&&(t5==b.t5)&&(t6==b.t6);
	}
	// subtract
	THETA_ABB operator-(const THETA_ABB& b){
		return THETA_ABB(t1 - b.t1, t2 - b.t2, t3 - b.t3, t4 - b.t4, t5 - b.t5, t6 - b.t6);
	}
	// printout
	void printout(){
		printf("THETA_ABB:\n\tt1 : %lf\tt2 : %lf\tt3 : %lf\tt4 : %lf\tt5 : %lf\tt6 : %lf\n",t1,t2,t3,t4,t5,t6);
		printf("(deg)\tt1 : %.3lf\tt2 : %.3lf\tt3 : %.3lf\tt4 : %.3lf\tt5 : %.3lf\tt6 : %.3lf\n\n",t1 todeg,t2 todeg,t3 todeg,t4 todeg,t5 todeg,t6 todeg);
	}
};

struct IRB140_ABB{
	Forward j[7];
	// initialize j
	void init(){
		j[1] = Forward(0,0,0,0);
		j[2] = Forward(-90 torad, 70, 0, 0);
		j[3] = Forward(0, 360, 0, 0);
		j[4] = Forward(-90 torad, 0, 0, 380);
		j[5] = Forward(90 torad, 0, 0, 0);
		j[6] = Forward(-90 torad, 0, 0, 0);
	}
	// constructor
	IRB140_ABB(){}
	IRB140_ABB(THETA_ABB t){
		init();
		j[1].theta=t.t1; j[2].theta=t.t2 - (90 torad); j[3].theta=t.t3; j[4].theta=t.t4; j[5].theta=t.t5; j[6].theta=t.t6;
	}
	IRB140_ABB(double t1i, double t2i, double t3i, double t4i, double t5i, double t6i){
		init();
		j[1].theta=t1i; j[2].theta=t2i - (90 torad); j[3].theta=t3i; j[4].theta=t4i; j[5].theta=t5i; j[6].theta=t6i;
	}
	// assign
	IRB140_ABB& operator=(const IRB140_ABB& b){for(int i=1;i<=6;i++)j[i]=b.j[i]; return *this;}
	// equal
	bool operator==(const IRB140_ABB& b){return (j[1]==b.j[1])&&(j[2]==b.j[2])&&(j[3]==b.j[3])&&(j[4]==b.j[4])&&(j[5]==b.j[5])&&(j[6]==b.j[6]);}
	// printout
	void printout(){
		printf("IRB140_ABB:\n\tt1=%.4lf\tt2=%.4lf(after -90)\tt3=%.4lf\tt4=%.4lf\tt5=%.4lf\tt6=%.4lf\n\n",
		j[1].theta,j[2].theta,j[3].theta,j[4].theta,j[5].theta,j[6].theta);
	}
};

TransMatrix TransfromIRB140_ABB(IRB140_ABB b, int until){
	TransMatrix t(Unit_Matrix(), Vector3(0,0,352), Vector3(0,0,0), 1);
	for(int i=1;i<=until;i++){
		t = t*TransFromForward(b.j[i]);
	}
	return t;
}
TransMatrix TransfromIRB140_ABB_Fast(IRB140_ABB b, int until){
	TransMatrix t(Unit_Matrix(), Vector3(0,0,352), Vector3(0,0,0), 1);
	TransMatrix tmp;
	for(int i=1;i<=until;i++){
		tmp = TransFromForward_Fast(b.j[i]);
		t = t*tmp;
	}
	return t;
}

bool angle_max(THETA_ABB b, double max_rad){
	return abs(b.t1)<=max_rad && abs(b.t2)<=max_rad && abs(b.t3)<=max_rad && abs(b.t4)<=max_rad && abs(b.t5)<=max_rad && abs(b.t6)<=max_rad ;
}

double ABB_f1(double t3){
	return -380*sin(t3) + 360;
}
double ABB_f2(double t3){
	return 380*cos(t3);
}
double ABB_f3(double t3){
	return 0;
}

THETA_ABB FIND_t1_t2_from_t3(double t3, double x, double y, double z){
	double r = x*x + y*y + z*z;
	THETA_ABB thetas(0,0,t3,0,0,0);
	double f1 = ABB_f1(t3), f2 = ABB_f2(t3), f3 = ABB_f3(t3);
	double k1 = f1, k2 = -f2, k3 = 380*380 + 70*70 + 360*360 - 2*380*360*sin(t3), k4 = 0;
	
	double s2 = (k1*(r-k3)/140 + k2*z) / (k2*k2 + k1*k1), c2 = (z-k2*s2)/k1;
	
	thetas.t2 = Atan2(s2,c2);
	double g1 = s2*f1 + c2*f2 + 70, c1 = x/g1, s1 = y/g1;
	thetas.t1 = Atan2(s1,c1);
	return thetas;
}

void ABB_PIEPER_04(double x, double y, double z, THETA_ABB **sol){
	THETA_ABB me(0,0,0,0,0,0);
	for(int i=1;i<=4;i++)
		for(int j=1;j<=2;j++)
			sol[i][j] = me;
	// z =  -380sin(t2+t3) + 360cos(t2) (seen from 1)
	// r = [ 380cos(t2+t3) + 360sin(t2) ] * 140 + k3 (seen from 1)
	// k3 = 380^2 + 70^2 + 360^2 - 2*380*360sin(t3)
	double r = x*x+y*y+z*z, r2 = r - 380*380 - 70*70 - 360*360, k = 2*380*360, z2 = z*z - 380*380 - 360*360;
	double b = 140*140+2*r2, c = r2*r2 + 140*140*z2, d = b*b - 4*c;
	double s3_1, s3_2;
	double t3_11, t3_12, t3_21, t3_22;
	bool singular;
	for(int i=1;i<=4;i++)
		for(int j=1;j<=2;j++)
			sol[i][j].valid=false;
	if (d < 0){
		printf("No real value for sin3 was calculated! Please Check if something's wrong\n");
		return;
	}
	if (d == 0){
		singular = true;
		s3_1 = -b/2.0 /2/380/360;
		t3_11 = Asin(s3_1,0);
		t3_12 = Asin(s3_1,1);
		//printf("Only one value was solved for sin3 (%lf) (Singularity!)\n", s3_1);
		if(s3_1<-1.0||s3_1>1.0){
			printf("Somethings wrong. sin value out of bounds.\n");
			return;
		}
	}else{
		singular = false;
		s3_1 = (-b+sqrt(d))/2.0 /2/380/360;
		s3_2 = (-b-sqrt(d))/2.0 /2/380/360;
		//printf("Two real values solved for sin3 (%lf, %lf)\n", s3_1, s3_2);
		if(s3_1<-1.0||s3_1>1.0){
			if(s3_2<-1.0||s3_2>1.0){
				//printf("Found two sin values which are all out of bounds.\n");
				return;
			}else{
				//printf("Found one sin value which is out of bounds.\n");
				s3_2 = s3_1;
				d = 0;
			}
		}else if(s3_2<-1.0||s3_2>1.0){
			//printf("Found one sin value which is out of bounds.\n");
			d = 0;
		}else{
			t3_21 = Asin(s3_2,0);
			t3_22 = Asin(s3_2,1);
		}
		t3_11 = Asin(s3_1,0);
		t3_12 = Asin(s3_1,1);
	}
	sol[1][1] = FIND_t1_t2_from_t3(t3_11,x,y,z);
	sol[1][1].valid = sol[1][2].valid = true;
	sol[2][1] = FIND_t1_t2_from_t3(t3_12,x,y,z);
	sol[2][1].valid = sol[2][2].valid = true;
	if(d>0){
		sol[3][1] = FIND_t1_t2_from_t3(t3_21,x,y,z);
		sol[3][1].valid = sol[3][2].valid = true;
		sol[4][1] = FIND_t1_t2_from_t3(t3_22,x,y,z);
		sol[4][1].valid = sol[4][2].valid = true;
	}
}

void ABB_PIEPER_46(RotMatrix R06, RotMatrix R04, THETA_ABB *sol){
	RotMatrix R46 = (!R04)*R06;
	//printf("Inn 46\n");
	//R46.printout();
	ZYZ zyz46 = ZYZfromRotMatrix(R46,0);
	sol[1].t4 = zyz46.a;
	sol[1].t5 = -zyz46.b;
	sol[1].t6 = zyz46.y;
	zyz46 = ZYZfromRotMatrix(R46,1);
	sol[2].t4 = zyz46.a;
	sol[2].t5 = -zyz46.b;
	sol[2].t6 = zyz46.y;
}

int ABB_PIEPER_06(TransMatrix T06, THETA_ABB **sol){
	double x = T06.P.v[1], y = T06.P.v[2], z = T06.P.v[3] - 352;
	IRB140_ABB abb;
	TransMatrix trans_abb;
	RotMatrix R06 = T06.R, R04;
	ABB_PIEPER_04(x,y,z, sol);
	
	int soln=0;
	for(int i=1;i<=4;i++){
		if(sol[i][1].valid){
			
			soln+=2;
			sol[i][2] = sol[i][1];
			abb = IRB140_ABB(sol[i][1]);
			
			trans_abb = TransfromIRB140_ABB(abb, 4);
			R04 = trans_abb.R;
			//
			/*printf("Wait!! this is i=%d trans_abb ",i);
			sol[i][1].printout();
			trans_abb.printout();
			R06.printout();*/
			//
			
			ABB_PIEPER_46(R06, R04, sol[i]);
		}
	}
	return soln;
}

ROBOT ABB_TO_ROBOT(IRB140_ABB b){
	ROBOT r;
	for(int i=1;i<=6;i++){
		r.j[i-1].alpha = b.j[i].alpha;
		r.j[i-1].a = b.j[i].a;
		r.j[i].theta = b.j[i].theta;
		r.j[i].d = b.j[i].d;
	}
	r.j[1].d = 352;
	return r;
}

#define PIEPER_GENERAL_H

#include<cstdio>
#include<cstring>

#ifndef FORWARD_KINEMATICS_H
#include "Forward_Kinematics.h"
#endif

#define BRUTE_FORCE_N 10000000.0
#define first_stage_error 1e-4

struct THETA{
	bool valid;
	double t1, t2, t3, t4, t5, t6;
	// constructor
	THETA(){}
	THETA(double t1i, double t2i, double t3i, double t4i, double t5i, double t6i){
		t1=t1i; t2=t2i; t3=t3i; t4=t4i; t5=t5i; t6=t6i;
	}
	// assign
	THETA& operator=(const THETA& b){
		t1=b.t1; t2=b.t2; t3=b.t3; t4=b.t4; t5=b.t5; t6=b.t6;
	}
	// equal
	bool operator==(const THETA& b){
		return (t1==b.t1)&&(t2==b.t2)&&(t3==b.t3)&&(t4==b.t4)&&(t5==b.t5)&&(t6==b.t6);
	}
	// printout
	void printout(){
		printf("THETA_ABB:\n\tt1 : %lf\tt2 : %lf\tt3 : %lf\tt4 : %lf\tt5 : %lf\tt6 : %lf\n",t1,t2,t3,t4,t5,t6);
		printf("(deg)\tt1 : %.3lf\tt2 : %.3lf\tt3 : %.3lf\tt4 : %.3lf\tt5 : %.3lf\tt6 : %.3lf\n\n",t1 todeg,t2 todeg,t3 todeg,t4 todeg,t5 todeg,t6 todeg);
	}
};

struct ROBOT{
	
	Forward j[7];
	// initialize j
	void init(){
		for(int i=0;i<=6;i++)
			j[i] = Forward(0.0,0.0,0.0,0.0);
	}
	// constructor
	ROBOT(){ init(); }
	// insert angles (radian)
	void add_theta(THETA t){
		j[1].theta=t.t1; j[2].theta=t.t2; j[3].theta=t.t3; j[4].theta=t.t4; j[5].theta=t.t5; j[6].theta=t.t6;
	}
	void add_theta(double t1i, double t2i, double t3i, double t4i, double t5i, double t6i){
		j[1].theta=t1i; j[2].theta=t2i; j[3].theta=t3i; j[4].theta=t4i; j[5].theta=t5i; j[6].theta=t6i;
	}
	// assign
	ROBOT& operator=(const ROBOT& b){for(int i=0;i<=6;i++)j[i]=b.j[i]; return *this;}
	// equal
	bool operator==(const ROBOT& b){return (j[0]==b.j[0])&&(j[1]==b.j[1])&&(j[2]==b.j[2])&&(j[3]==b.j[3])&&(j[4]==b.j[4])&&(j[5]==b.j[5])&&(j[6]==b.j[6]);}
	// printout
	void printout(){
		printf("ROBOT:\n");
		for(int i=0;i<=6;i++){
			printf("(%d) ", i);
			j[i].printout();
		}
		puts("");
	}
};

double f1__c, f1__s, f1__k, f2__c, f2__s, f2__k, f3__c, f3__s, f3__k, k4__c, k4__s, k4__k;
double ff__c, ff__s, ff__k, k3__c, k3__s, k3__k;

TransMatrix TransfromROBOT(ROBOT b, int until){
	TransMatrix t(Unit_Matrix(), Vector3(0,0,0), Vector3(0,0,0), 1);
	TransMatrix tmp;
	Forward f;
	for(int i=1;i<=until;i++){
		f = Forward(b.j[i-1].alpha, b.j[i-1].a, b.j[i].theta, b.j[i].d);
		t = t*TransFromForward_Fast(f);
	}
	return t;
}

double R_ff(ROBOT b){
	ff__c = 2.0 * b.j[2].a * b.j[3].a;
	ff__s = 2.0 * b.j[2].a * b.j[4].d * sin(b.j[3].alpha);
	ff__k = b.j[3].a*b.j[3].a + b.j[2].a*b.j[2].a + b.j[3].d*b.j[3].d + b.j[4].d*b.j[4].d + 2.0 * b.j[3].d*b.j[4].d*cos(b.j[3].alpha) ;
	return cos(b.j[3].theta) * ff__c + sin(b.j[3].theta) * ff__s + ff__k ;
}
double R_f1(ROBOT b){
	f1__c = b.j[3].a;
	f1__s = b.j[4].d * sin(b.j[3].alpha);
	f1__k = b.j[2].a;
	return f1__c * cos(b.j[3].theta) + f1__s * sin(b.j[3].theta) + f1__k;
}
double R_f2(ROBOT b){
	f2__c = -b.j[4].d * sin(b.j[3].alpha) * cos(b.j[2].alpha);
	f2__s = b.j[3].a * cos(b.j[2].alpha);
	f2__k = -b.j[4].d * sin(b.j[2].alpha) * cos(b.j[3].alpha) - b.j[3].d * sin(b.j[2].alpha);
	return f2__s * sin(b.j[3].theta) + f2__c * cos(b.j[3].theta) + f2__k;
}
double R_f3(ROBOT b){
	f3__c = -b.j[4].d * sin(b.j[3].alpha) * sin(b.j[2].alpha);
	f3__s = b.j[3].a * sin(b.j[2].alpha);
	f3__k = b.j[4].d * cos(b.j[2].alpha) * cos(b.j[3].alpha) + b.j[3].d * cos(b.j[2].alpha);
	return f3__s * sin(b.j[3].theta) + f3__c * cos(b.j[3].theta) + f3__k;
}

double R_g1(ROBOT b){
	return cos(b.j[2].theta) * R_f1(b) - sin(b.j[2].theta) * R_f2(b) + b.j[1].a;
}
double R_g2(ROBOT b){
	return sin(b.j[2].theta) * cos(b.j[1].alpha) * R_f1(b) + cos(b.j[2].theta) * cos(b.j[1].alpha) * R_f2(b)
	- sin(b.j[1].alpha) * R_f3(b);
}
double R_g3(ROBOT b){
	return sin(b.j[2].theta) * sin(b.j[1].alpha) * R_f1(b) + cos(b.j[2].theta) * sin(b.j[1].alpha) * R_f2(b)
	+ cos(b.j[1].alpha) * R_f3(b) + b.j[2].d * cos(b.j[1].alpha);
}

double R_r_check(ROBOT b){
	return sqrt(pow(R_g1(b),2.0)+pow(R_g2(b),2.0)+pow(R_g3(b),2.0));
}
double R_r(double x, double y, double z){
	return sqrt(x*x + y*y + z*z);
}

double R_k1(ROBOT b){
	return R_f1(b);
}
double R_k2(ROBOT b){
	double k2 = -R_f2(b);
	f2__c = -f2__c;
	f2__s = -f2__s;
	f2__k = -f2__k;
	return k2;
}
double R_k3(ROBOT b){
	double ff = R_ff(b);
	R_f1(b),R_f2(b),R_f3(b);
	
	k3__c = ff__c + 2.0*b.j[2].d*f3__c;
	k3__s = ff__s + 2.0*b.j[2].d*f3__s;
	k3__k = ff__k + b.j[1].a*b.j[1].a + b.j[2].d*b.j[2].d + 2.0*b.j[2].d*f3__k;
	
	return k3__c * cos(b.j[3].theta) + k3__s * sin(b.j[3].theta) + k3__k;
}
double R_k4(ROBOT b){
	double k4 = R_f3(b) * cos(b.j[1].alpha) + b.j[2].d * cos(b.j[1].alpha);
	k4__c = f3__c*cos(b.j[1].alpha);
	k4__s = f3__s*cos(b.j[1].alpha);
	k4__k = f3__k*cos(b.j[1].alpha) + b.j[2].d * cos(b.j[1].alpha);
	return k4;
}

int R_Pieper(TransMatrix T, ROBOT b){
	double x = T.P.v[1] , y = T.P.v[2] , z = T.P.v[3] , r = R_r(x,y,z);
	double k1,k2,k3,k4;
	
	double aa,bb,cc,det;
	
	double t3[16];
	int t3n=0;
	
	if(b.j[1].a==0.0){
		// r = k3 = k3__c * c3 + k3__s * s3 + k3__k
		R_k3(b);
		aa = r - k3__k + k3__c;
		bb = -2*k3__s;
		cc = r - k3__k - k3__c;
		det = bb*bb - 4*aa*cc;
		if(det>0){
			printf("Found two real values for u = tan(theta3/2).\n");
			
		}else if(abs(det)<1e-5){
			printf("Found one real value for u = tan(theta3/2).\n");
			
		}else{
			printf("Found no real value for u = tan(theta3/2).\n");
			
		}
	}else if(abs(sin(b.j[1].alpha))<1e-5){
		// z = k4 = k4__c * c3 + k4__s * s3 + k4__k
		R_k4(b);
		aa = z - k4__k + k4__c;
		bb = -2*k4__s;
		cc = z - k4__k - k4__c;
		det = bb*bb - 4*aa*cc;
		if(det>0){
			printf("Found two real values for u = tan(theta3/2).\n");
			
		}else if(abs(det)<1e-5){
			printf("Found one real value for u = tan(theta3/2).\n");
			
		}else{
			printf("Found no real value for u = tan(theta3/2).\n");
			
		}
	}else{
		/*
		(r-k3)^2	 (z-k4)^2
		--------- +	----------- = k1^2 + k2^2
		  4a1^2		s(alpha1)^2
		*/
		
	}

	
	return t3n;
}

















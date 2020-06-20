#include<cstdio>
#include<cstring>
#include<ctime>
#include<cstdlib>
using namespace std;
const int MAX = 5100;
const int MAXIMUM = 500; //the bigger this number is, the slower but more accurate
int l[MAX], ln;

char a[4], fa[4];
int guess(int x,int fakea){
	a[3] = x%10, a[2] = (x/10)%10, a[1] = (x/100)%10, a[0] = x/1000;
	fa[3] = fakea%10, fa[2] = (fakea/10)%10, fa[1] = (fakea/100)%10, fa[0] = fakea/1000;
	int ret = 0;
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			if(a[i] == fa[j]){
				ret += i == j ? 10 : 1;
				break;
			}
		}
	}
	return ret;
}

int sum = 0, maxm = 0, minm = 10000;
int mini, minival, maxjval, add, inpa, inpb, gs;
void guess_input(int k){
	printf("Guess : ");
	if(k<1000)putchar('0');
	printf("%d.\n", k);
	printf("Enter the results ( ?A ?B ) : ");
	scanf("%d%d", &inpa, &inpb);
	gs = inpa * 10 + inpb;
}
void AI(){
	
	int k = l[(rand()*rand())%ln], newln, cal = 0;
	guess_input(k);
	
	newln=0;
	for(int i=0; i<ln; i++)
		if(guess(l[i], k) == gs)
			l[newln++] = l[i];
	
	ln = newln;
	cal = 1;
	mini = minival = maxjval = -1;
	while(ln>1){
		minival=(int)1e8;
		add = ln / MAXIMUM + 1;
		
		
		for(int i=0; i<ln; i+=add){ // 假設我選了這個為答案
			maxjval=0;
			for(int j=0; j<ln; j+=add){ // 假設這個是正確答案 
				gs = guess(l[i], l[j]);
				for(int k = 0; k<ln; k+=add){ // 剩下的這個答案有可能嗎? 
					if(gs == guess(l[j], l[k])){
						maxjval++;
						if(maxjval == minival)break;
					}
				}
				if(maxjval == minival)break;
			}
			if(maxjval < minival){
				minival = maxjval;
				mini = i;
			}
		}
		
		k = l[mini];
		guess_input(k);
		
		newln = 0;
		for(int i=0;i<ln;i++)
			if(guess(l[i], k) == gs)
				l[newln++] = l[i];
		ln = newln;
		cal++;
	}
	printf("The Answer : ");
	if(l[0] < 1000) putchar('0');
	printf("%d. Guessed Times : %d.\n", l[0], cal);
}
void reset(){
	// save all possible answers
	ln = 0;
    for(int i=123; i<=9876; i++){
		int a1 = i%10, a2 = (i/10)%10, a3 = (i/100)%10, a4 = i/1000;
		if( (a1-a2)*(a1-a3)*(a1-a4)*(a2-a3)*(a2-a4)*(a3-a4) ) l[ln++]=i;
	}
}

int main(){
	srand(time(NULL));
	reset();
	srand(time(NULL));
	AI();
	while(1);
    return 0;
}

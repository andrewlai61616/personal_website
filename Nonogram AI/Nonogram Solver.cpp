#include<cstdio>
#include<cstring>
#include<algorithm>
#include<time.h>
using namespace std;
const int MAX=105;
const long long MAXCOMBO=10000000;

int n,m;
int ln[MAX][MAX],lm[MAX][MAX],lnc[MAX],lmc[MAX];
int lnsum[MAX][MAX],lmsum[MAX][MAX];

long long c[MAX][MAX];
bool never[MAX][MAX],always[MAX][MAX];
bool nevertmp[MAX],alwaystmp[MAX],wedontstop;
int keys[MAX];


long long calcount=0;
bool grid[MAX][MAX];
bool DP(int row,int k,int cur){
	int i,j,cnt,nm,final,orgcur=cur;
	for(j=1;j<=m;j++){
		cnt=0;
		i=nm=1;
		final=j<cur?row:row-1;
		while(i<=final){
			if(grid[i][j]){
				cnt++;
				if(cnt>lm[j][nm]){
					return 0;
				}
			}else if(cnt){
				if(cnt<lm[j][nm])return 0;
				cnt=0;
				nm++;
			}
			i++;
		}
	}
	if(row==n+1)return 1;
	int usage=lnsum[row][lnc[row]]-lnsum[row][k-1]+lnc[row]-k;
	for(i=0;i<ln[row][k];i++){
		if(never[row][cur+i]){
			cur+=i+1;
			if(cur+usage>m+1)return 0;
			i=-1;
		}
	}
	for(i=orgcur;i<cur;i++){
		if(always[row][i])return 0;
	}
	for(i=0;i<ln[row][k];i++)grid[row][cur+i]=1;
	int nxtrow,nxtk;
	if(k==lnc[row]){
		nxtrow=row+1;
		nxtk=1;
	}else{
		nxtrow=row;
		nxtk=k+1;
	}
	do{
		calcount++;
		//check
		if(k==lnc[row]){
			for(i=cur+ln[row][k];i<=m;i++)if(always[row][i])break;
			if(i==m+1)if(DP(nxtrow,nxtk,1))return 1;
		}else{
			if(DP(nxtrow,nxtk,cur+ln[row][k]+1))return 1;
		}
		//add and forward black dot
		if(always[row][cur])break;
		grid[row][cur+ln[row][k]]=1;
		grid[row][cur++]=0;
		if(never[row][cur+ln[row][k]-1]){
			for(i=0;i<ln[row][k];i++)grid[row][cur+i]=0;
			orgcur=cur;
			cur+=ln[row][k];
			for(i=0;i<ln[row][k];i++){
				if(never[row][cur+i]){
					cur+=i+1;
					if(cur+usage>m+1)return 0;
					i=-1;
				}
			}
			for(i=orgcur;i<cur;i++){
				if(always[row][i])return 0;
			}
			for(i=0;i<ln[row][k];i++)grid[row][cur+i]=1;
		}
	}while(cur+usage<=m+1);
	//give back black dot
	for(i=0;i<ln[row][k];i++)grid[row][cur+i]=0;
	return 0;
}

void row(int y){
	keys[lnc[y]+1]=m+2;
	int i,cur;
	memset(nevertmp,1,sizeof(nevertmp));
	memset(alwaystmp,1,sizeof(alwaystmp));
	for(i=cur=1;i<=lnc[y];i++){
		keys[i]=cur;
		cur+=ln[y][i]+1;
	}
	do{
		for(cur=1;cur<keys[1];cur++){
			if(always[y][cur])goto next_permutation_y;
		}
		for(i=1;i<=lnc[y];i++){
			for(cur=keys[i];cur<keys[i]+ln[y][i];cur++){
				if(never[y][cur])goto next_permutation_y;
			}
			for(cur=keys[i]+ln[y][i];cur<keys[i+1];cur++){
				if(always[y][cur])goto next_permutation_y;
			}
		}
		for(cur=keys[lnc[y]]+ln[y][lnc[y]];cur<=m;cur++){
			if(always[y][cur])goto next_permutation_y;
		}
		for(cur=1;cur<keys[1];cur++){
			alwaystmp[cur]=0;
		}
		for(i=1;i<=lnc[y];i++){
			for(cur=keys[i];cur<keys[i]+ln[y][i];cur++){
				nevertmp[cur]=0;
			}
			for(cur=keys[i]+ln[y][i];cur<keys[i+1];cur++){
				alwaystmp[cur]=0;
			}
		}
		for(cur=keys[lnc[y]]+ln[y][lnc[y]];cur<=m;cur++){
			alwaystmp[cur]=0;
		}
		next_permutation_y:
		i=lnc[y];
		while(keys[i]+ln[y][i]+1==keys[i+1]){
			i--;
			if(!i)break;
		}
		if(!i)break;;
		keys[i]++;
		cur=keys[i]+ln[y][i]+1;
		i++;
		for(;i<=lnc[y];i++){
			keys[i]=cur;
			cur+=ln[y][i]+1;
		}
	}while(1);
	for(cur=1;cur<=m;cur++){
		if(!never[y][cur]&&nevertmp[cur]){
			wedontstop=1;
			never[y][cur]=1;
		}
		if(!always[y][cur]&&alwaystmp[cur]){
			wedontstop=1;
			always[y][cur]=1;
		}
	}
}
void column(int x){
	keys[lmc[x]+1]=n+2;
	int j,cur;
	memset(nevertmp,1,sizeof(nevertmp));
	memset(alwaystmp,1,sizeof(alwaystmp));
	for(j=cur=1;j<=lmc[x];j++){
		keys[j]=cur;
		cur+=lm[x][j]+1;
	}
	do{
		for(cur=1;cur<keys[1];cur++){
			if(always[cur][x])goto next_permutation_x;
		}
		for(j=1;j<=lmc[x];j++){
			for(cur=keys[j];cur<keys[j]+lm[x][j];cur++){
				if(never[cur][x])goto next_permutation_x;
			}
			for(cur=keys[j]+lm[x][j];cur<keys[j+1];cur++){
				if(always[cur][x])goto next_permutation_x;
			}
		}
		for(cur=keys[lmc[x]]+lm[x][lmc[x]];cur<=n;cur++){
			if(always[cur][x])goto next_permutation_x;
		}
		
		for(cur=1;cur<keys[1];cur++){
			alwaystmp[cur]=0;
		}
		for(j=1;j<=lmc[x];j++){
			for(cur=keys[j];cur<keys[j]+lm[x][j];cur++){
				nevertmp[cur]=0;
			}
			for(cur=keys[j]+lm[x][j];cur<keys[j+1];cur++){
				alwaystmp[cur]=0;
			}
		}
		for(cur=keys[lmc[x]]+lm[x][lmc[x]];cur<=n;cur++){
			alwaystmp[cur]=0;
		}
		next_permutation_x:
		j=lmc[x];
		while(keys[j]+lm[x][j]+1==keys[j+1]){
			j--;
			if(!j)break;
		}
		if(!j)break;
		keys[j]++;
		cur=keys[j]+lm[x][j]+1;
		j++;
		for(;j<=lmc[x];j++){
			keys[j]=cur;
			cur+=lm[x][j]+1;
		}
	}while(1);
	for(cur=1;cur<=n;cur++){
		if(!never[cur][x]&&nevertmp[cur]){
			wedontstop=1;
			never[cur][x]=1;
		}
		if(!always[cur][x]&&alwaystmp[cur]){
			wedontstop=1;
			always[cur][x]=1;
		}
	}
}

long long combo(int nn,int mm){
	return c[mm+1][nn-mm+1];
}

int main(){
    int i,j,t;
    
    memset(c,0,sizeof(c));
    c[0][1]=1;
    for(i=1;i<MAX;i++){
		for(j=1;j<MAX;j++){
			c[i][j]=c[i-1][j]+c[i][j-1];
		}
	}
    // input valuess
    scanf("%d%d",&n,&m);
    for(i=1;i<=n;i++){
		scanf("%d",&lnc[i]);
		lnsum[i][0]=0;
		for(j=1;j<=lnc[i];j++){
			scanf("%d",&ln[i][j]);
			lnsum[i][j]=ln[i][j]+lnsum[i][j-1];
		}
	}
    for(i=1;i<=m;i++){
		scanf("%d",&lmc[i]);
		lmsum[i][0]=0;
		for(j=1;j<=lmc[i];j++){
			scanf("%d",&lm[i][j]);
			lmsum[i][j]=lm[i][j]+lmsum[i][j-1];
		}
	}
	printf("start\n");
	memset(never,0,sizeof(never));
	memset(always,0,sizeof(always));
	t=1;
	wedontstop=1;
	while(wedontstop){
		printf("%d\n",t++);
		wedontstop=0;
		for(i=1;i<=n;i++){
			if(combo(m-lnsum[i][lnc[i]]+lnsum[i][0]+1,lnc[i])<MAXCOMBO)row(i);
		}
		for(j=1;j<=m;j++){
			if(combo(n-lmsum[j][lmc[j]]+lmsum[j][0]+1,lmc[j])<MAXCOMBO)column(j);
		}
	}
	
	/*DP(1,1,1);
	for(i=1;i<=n;i++){
		for(j=1;j<=m;j++){
			printf("%c ",grid[i][j]?'@':' ');
		}
		puts("");
	}
	puts("");
	printf("%I64d\n\n",calcount);*/
	
	printf("answer sent to Ans.txt\n");
	freopen("Ans.txt","w",stdout);
	for(i=1;i<=n;i++){
		printf("%d:\t",i);
		for(j=1;j<=m;j++){
			if(always[i][j])printf("X ");
			else printf("_ ");
			//printf("%c ",always[i][j]?'':' ');
			//printf("%c ",always[i][j]?never[i][j]?'!':'@':never[i][j]?'X':' ');
		}
		puts("");
	}
	puts("");
	for(i=1;i<=n;i++){
		printf("%d:\t",i);
		for(j=1;j<=m;j++){
			if(never[i][j])printf("  ");
			else printf("¢p");
			//printf("%c ",always[i][j]?'':' ');
			//printf("%c ",always[i][j]?never[i][j]?'!':'@':never[i][j]?'X':' ');
		}
		puts("");
	}
	puts("");
	while(1);
    return 0;
}

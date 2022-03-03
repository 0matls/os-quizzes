#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

// to save a list of integers into a binary file 
void writeBinary(char *name, int *minmax){
    FILE *write_ptr = fopen(name, "wb");
    fwrite(minmax, sizeof(int), 2, write_ptr);
    fclose(write_ptr);
}


void minmaxAllListsFork( int **numbers, int nList, int *nElem ){

int minmax[2];
int pID = 0;
int status;
int min_val = INT_MAX;
int max_val = INT_MIN;
int p;
for(p = 0; p < nList; p++){

	
	pID = fork();
	
	
	if (pID < 0){
	    printf("Cannot fork!!\n");
	    exit(1);
	}
	else if (pID == 0){
	    //child process
	    for(int i = 0; i < (nElem[p] ); i++){
    		if(min_val > numbers[p][i])
    		    min_val = numbers[p][i];   
    		
    		if(max_val < numbers[p][i])
    		    max_val = numbers[p][i];    
	    }


	    minmax[0] = min_val;
	    minmax[1] = max_val;
	   
	    char *name = (char *)malloc(8*sizeof(char));
	    sprintf(name, "minmax-%d", p);
	    writeBinary(name, minmax);
	    exit(0);
	}
	 if (pID > 0) {	    
	   if (waitpid(pID, &status, 0) != pID){
	   	status = -1;
	   }
			
	}

}
     
    return;
    
}


void printboard(int **p,int n,int m){
	int i,j,k;
	for(i=0;i<n;i++){
		for(j=0;j<m;j++){
			printf("%d\t",p[i][j]);
		}
		putchar('\n');
	}
	printf("\n\n\n");
}



int main(void){
    int abc[2][2] = {{1, 2},{ 3 ,4 }};
    int el[2] = {2,2};

	int n =2;
	int m=2;
	int **p,i,j;
	srand(time(0));
	p=(int **)malloc(n*sizeof(int*));
	for(i=0;i<n;i++){
		p[i]=(int *)malloc(m*sizeof(int));
	}
	for (i=0;i<n;i++){
		for(j=0;j<m;j++){
			p[i][j]=rand()%101;
		}
	}
	printboard(p,n,m);
	minmaxAllListsFork(p, 2, el);
    return 0;
}


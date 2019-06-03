// A greedy aproximation based solution for 0-1 Knapsack problem with LARGE data sizes
#include<stdio.h> 
#include<stdlib.h>
#include <time.h>
#include <string.h>
struct Item { 
    int value; 
	int weight;
	int index;
};
typedef struct Item item;
 
// A utility function returns maximum density(value/weight) of two items which is necessary to calculate qsort()
int itemCmp(const void *a, const void *b) 
{ 
	double d1 = (double)(*((item*)a)).value/(*((item*)a)).weight;
    double d2 = (double)(*((item*)b)).value/(*((item*)b)).weight;
    if (d1 > d2) 
    	return  -1;
    if (d1 < d2)
    	return  1;
    return 0;
} 
 
//A function that orders array of items by their density
void  orderByDensity(item* densities,int wt[], int val[], int n){
	int i;
	for (i = 0; i < n; i++) {
		densities[i].index=i;
		densities[i].value=val[i];
		densities[i].weight=wt[i];
	}
	qsort(densities, n, sizeof(item), itemCmp);
}

//add items in to sack
int addItems(int* solSet, item* arr,int W,int n){
	int curWeight = 0;  		// Current weight in knapsack 
	int res = 0;			 // Result (value in Knapsack) 
	
	// Looping through all Items 
    int i;
    for (i = 0; i < n; i++) 
    { 
        // If adding Item won't overflow, add it completely 
        if (curWeight + (arr+i)->weight <= W) 
        { 
            curWeight += (arr+i)->weight; 
            res += (arr+i)->value; 
            solSet[(arr+i)->index]=1;	// makes array indexes[index of chosen item]=1. this array will be necessary to print output
        } 
    } 
    return res;
}


//greedy algorithm
int greedy(int* solSet, item* arr,int W, int wt[], int val[], int n,double timeLimit){
	
  	int finalResult,res,i=0, j=0;
  	item tmp;
  	clock_t start, end;
  	double time_passed=0,accuracyRatio1,accuracyRatio2;
		
  	//order input array by density
  	orderByDensity(arr, wt, val, n);
    
    //calculate finalResult
	finalResult=addItems(solSet, arr,W,n);
  	
  	//some modification on standart greedy algorithm
    while(time_passed<timeLimit){
    	start=clock();//start timer
    	int x=0;
		
    	//find biggest element which is not in solution set and swap it with first element
		int c;
		for (c=0+i;c<n;c++){
			if(solSet[arr[c].index]==0) break;
		}
	
		if (i != c) {
			//swap arr[c-1] and arr[0]
			tmp = arr[0+i];
			arr[0+i] = arr[c-1];
			arr[c-1] = tmp;
			//calculate res by shifting array by j=1,..,c
			if( j!=c ){
				if(i+j==n) break;		//safety insurence for overflow
				int d;
				for(d=0;d<n;d++)
					solSet[d]=0;	
				res=addItems(solSet, arr+i+j,W,n-j-i);
				for(d=i+j;d<n;d++)
					solSet[d-i-j]=solSet[d];
				j++;
			}
		}

		if(res > finalResult)
			finalResult=res;
				
    	i++;
		
		
		end = clock();
    	time_passed += ((double) (end - start)) / CLOCKS_PER_SEC;
	}
    
    // return final value 
    return finalResult;
}
  
int main() { 
	int numberOfItems, capacity, value, weight;
	clock_t start, end,start2,end2;
    double cpu_time_used=0.0,cpu_time_used2=0.0;
	FILE *fp, *outG; 
	
	 
	char filename[100]; 
    printf("Enter the filename(with extention): "); 
    scanf("%s", filename); 
    
    start2 = clock();
    //Check out for the file
   if ((fp = fopen(filename, "r")) == NULL){
        printf("Error! opening file");
        // Program exits if file pointer returns NULL.
        exit(1);         
    }
    // if exist it continoues
	//Read first line
    fscanf(fp, "%d %d", &numberOfItems, &capacity);
    
    
    //initialize values[] and weights[]
    int *values, *weights, i=0,j;
    values = malloc(numberOfItems*sizeof(int));
    if (values==NULL)printf("values[] allocation error");
    weights = malloc(numberOfItems*sizeof(int));
    if (weights==NULL)printf("weights[] allocation error");

	// Read the content and write in to arrays
    while( fscanf(fp, "%d %d", &value, &weight) != EOF )
    {
        values[i]=value;
        weights[i]=weight;
        i++;
    }  
	fclose(fp);
   
   
   ///////////////////////////////////////////////////////
   /////////			GREEDY APROXIMATION		//////////
   ///////////////////////////////////////////////////////
	//ordered list of items
	item *densities;
	densities = malloc(numberOfItems*sizeof(item));
    if (values==NULL)printf("densities[] allocation error");
    //chosen elements(solution set)
    int solSet[numberOfItems], a,b;
    for(a=0; a<numberOfItems;a++){
    	solSet[a]= 0;
	}
	
	//initialize a new buffer
	char *buffer2;
	buffer2 = malloc((numberOfItems+1)*20*sizeof (char));
	
	start = clock();	//timer
	
	int res= greedy(solSet,densities, capacity, weights, values, numberOfItems,cpu_time_used/10);	//call greedy function
	
	end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\n %f is the time taken by the greedy approximation algorithm", cpu_time_used);
    
    //print results to buffer2
	sprintf(buffer2,"%d\n", res); 
	for(b=0; b<numberOfItems;b++){
		if(solSet[b]==1)
    		sprintf(buffer2 + strlen(buffer2), "1\n"); 
    	else
    		sprintf(buffer2 + strlen(buffer2), "0\n"); 
	}
	
	//print output file for greedy algorithm
	char outputName2[100];
	snprintf(outputName2, 100, "output-%s", filename);
    if ((outG = fopen(outputName2, "w+")) == NULL){
        printf("Error! writing file");
        // Program exits if file pointer returns NULL.
        exit(1);         
    }
    else fprintf(outG,"%s",buffer2);
    fclose(outG);
	
	end2= clock();
    cpu_time_used2 = (double) (end2-start2) / CLOCKS_PER_SEC;
    printf("\n %f is the time taken by the whole greedy approximation part of the code", cpu_time_used2);
    
    
	free(values);
	free(weights);
	free(buffer2);
    free(densities);
	return 0;
} 

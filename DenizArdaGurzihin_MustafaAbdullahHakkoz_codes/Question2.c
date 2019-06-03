#include<stdio.h> 
#include<stdlib.h>
#include <time.h>
#include <string.h>

 

// File read opearation is mading with the typing your own input as file name so if you want to type directly the dat file and this .c file has to be in same directory. 

int multiple_Knapsack(int numberOfItems, int num_Knapsack, int cap_Knapsack[], int values[], int weights[], double Fractional[], int solSet[numberOfItems][num_Knapsack]){
	int Num_Knapsack = num_Knapsack;
	int numbers = numberOfItems;
	int x,a,b;
	int Val_Knapsack[Num_Knapsack];
	
	//Initialize all elements to zero
	for(x=0; x<Num_Knapsack; x++){
		Val_Knapsack[x] = 0;
	}
	
	//assign some value to x for while loop
	x=findMax(Fractional, numbers);
	
	//while loop to check all the elements in the Fractional
	while(Fractional[x]!=0){
		int j=0;
		//assign y for the first time
		int y = findMax(Fractional, numbers);
		
		for(j=0; j<num_Knapsack; j++){
			//we want to put the biggest value so every time we need to check y for max.
			y = findMax(Fractional, numbers);
			//if there is enough capacity for y value
			if(cap_Knapsack[j]>weights[y]){
				//decrease total capacity by the weight of the node and tadd the value of the node as value of knapsack
				cap_Knapsack[j] = cap_Knapsack[j] - weights[y];
				Val_Knapsack[j] = Val_Knapsack[j] + values[y];
				//arrange an output array an initialize current element we have used as 1 and make fractional part 0 to show we have used this element
				solSet[y][j] = 1;	
				Fractional[y] = 0.0;	
			}
		}
		//if we couldnt make any place for this index y; we made zero in here to continoue next element
		Fractional[y] = 0.0;
		//find the new max index to continoue loop until every element become 0 
		x=findMax(Fractional, numbers);
	}
	
	//finding the total value of the knapsack
	int Total_Val=0;
	
	for(x=0; x<Num_Knapsack; x++){
		Total_Val = Total_Val + Val_Knapsack[x];
	}
	
	printf("%d \n", Total_Val );
	
	//Printing the Output Matrix to see 1 and 0 
	/*for(a=0; a<numbers; a++){
		for(b=0; b<Num_Knapsack; b++){
			printf("%d ", solSet[a][b]);
		}
		printf("\n");
	}*/
	return Total_Val;
}

//finding max fractional value in the input; this functions finds the index of the maximum value
int findMax(double Fractional[], int numberOfItems){
	int LastIndex=numberOfItems;
	int index=0;
	double Max = Fractional[index];
	int returnIndex=0;
	
	//First we initialize the Max with the first element than we have started to trace all array to see if we could find bogger one
	//once we have found, we have made it Max and we have coninou like this until end of array
	for(index=0; index<LastIndex; index++){
		if(Max<Fractional[index]){
			Max = Fractional[index];
			returnIndex = index;
		}
	}
	return returnIndex;
}



int main(){
	double time_spent = 0.0, time_spent2=0.0;
	
	int numberOfItems, capacity, value, weight, num_Knapsack;
	FILE *fp;
	FILE *fpout;
	char filename[100]; 
    printf("File read opearation is mading with the typing your own input as file name so if you want to type directly the dat file and this .c file has to be in same directory \n");
	printf("Enter the filename(with extention): "); 
    scanf("%s", filename); 
    
    clock_t start2 = clock();
    
    if ((fp = fopen(filename, "r")) == NULL){
        printf("Error! opening file");
        // Program exits if file pointer returns NULL.
        exit(1);         
    }
    // if exist it continoues
	//Read first line
    fscanf(fp, "%d %d", &numberOfItems, &num_Knapsack);
    
    int cap_Knapsack[num_Knapsack];
    int cT=0;
    
	for(cT=0; cT<num_Knapsack; cT++){
		fscanf(fp, "%d ", &cap_Knapsack[cT]);
	}
    
    
    double Fractional[numberOfItems];
    // Read the content and write in to arrays
    int values[numberOfItems], weights[numberOfItems], count=0;
    while( fscanf(fp, "%d %d", &value, &weight) != EOF ){
        values[count]=value;
        weights[count]=weight;
        Fractional[count] = (double) values[count]/weights[count];
        count++;
    }	
	fclose(fp);
	
	//chosen elements(solution set)
    int solSet[numberOfItems][num_Knapsack], a,b;
    for(a=0; a<numberOfItems;a++){
    	for(b=0; b<num_Knapsack;b++){
    	solSet[a][b]= 0;
		}
	}
	
	clock_t start = clock();

	int total_Value;
	total_Value=multiple_Knapsack(numberOfItems, num_Knapsack, cap_Knapsack,  values,  weights,  Fractional,solSet);
	
	clock_t end = clock(); 
    time_spent = (double)(end-start)/CLOCKS_PER_SEC; // in seconds 
	
	
	
	//initialize a new buffer
	char *buffer;
	buffer = malloc((numberOfItems+1)*20*sizeof (char));
	//print results to buffer
	sprintf(buffer,"%d\n", total_Value); 
	for(a=0; a<numberOfItems;a++){
    	for(b=0; b<num_Knapsack;b++){
			if(solSet[a][b]==1){
				if(b==num_Knapsack-1)
	    			sprintf(buffer + strlen(buffer), "1\n"); 
	    		else 
	    			sprintf(buffer + strlen(buffer), "1 "); 
			}
	    		
	    	else{   		
	    		if(b==num_Knapsack-1)
	    			sprintf(buffer + strlen(buffer), "0\n"); 
	    		else 
	    			sprintf(buffer + strlen(buffer), "0 "); 
			}
	    		
	    }
	}
	sprintf(buffer + strlen(buffer), "\0");  
	
	//PRINT OUTPUT FILE
	char outputName[100];
	snprintf(outputName, 100, "output-%s", filename);
	fpout = fopen(outputName, "w");
	if (fpout == NULL){
		printf("Error! writing file");
        // Program exits if file pointer returns NULL.
        exit(1);         
    }
    else fprintf(fpout,"%s",buffer);
    fclose(fpout);
    
	clock_t end2 = clock();
	time_spent2 = (double)(end2-start2)/CLOCKS_PER_SEC; // in seconds 
	
    printf("\n %f is total time spent by multiple knapsack algorithm.", time_spent);
    printf("\n %f is total time spent by whole program.", time_spent2);
    
    free(buffer);
	
	return 0;
}

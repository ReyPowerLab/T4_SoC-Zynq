//#include <iomanip>
#include <math.h>
#include "specs.h"
#include "EucHW.h"
#include "EucSW.h"
#include <iostream>
using namespace std;

void genRandArray(T min, T max, int size, T *array);
int compare(T gold, T result, T th);




int main (){
	int errors = 0;
	int tests = 10;
	
	T AB[2*M];
	T C_HW, C_SW;

	T diff;
	T th = 0.000001;
	T min = 0;
	T max = 100;
	cout << "Data Number: ["<< M <<"]"  << endl;
	for (int i=0; i<tests; i++){
		genRandArray(min, max, 2*M, AB);
	//	genRandArray(min, max, M, B);
	//	T A[8]={1,2,3,4,5,6,7,8};
	//	T B[8]={8,7,6,5,4,3,2,1};
 	//T AB[2*8]={1,2,3,4,5,6,7,8,8,7,6,5,4,3,2,1};


		//C_HW = EucHW (A, B,true);
		       EucHW (AB, &C_HW);
		       EucSW (AB, &C_SW);

		errors += compare(C_SW, C_HW,  th);
	//	cout <<"Results of A0: "<< dec << A[0] << endl;
	//	cout <<"Results of B0: "<< dec << B[0] << endl;
		cout <<"Results of C_SW: "<< dec << C_SW << endl;
		cout <<"Results of C_HW: "<< dec << C_HW << endl;
	}
	cout <<"Number of errors: " << errors << endl;
	if (errors){
		return 1;
	}


	return 0;
}


void genRandArray(T min, T max, int size, T *array){
    for(int i=0; i<size; i++){
        array[i] = min + static_cast <T> (rand()) / ( static_cast <T> (RAND_MAX/(max-min)));
    }
}

int compare(T gold, T result,  T th){
        int errors = 0;
        double dif = 0;
        dif = fabs((double)gold - (double)result);
                // a comparison with NaN will always be false
                if (!(dif <= (double)th)){
                        errors=1;
                }
        return errors;
}

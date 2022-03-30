#include "EucSW.h"
#include <math.h>
#include <cmath>
#include <iostream>
#include <stdbool.h>
using namespace std;


void EucSW (T2 AB[2*M], T *C){

	T delta, sumatoria = 0;


	/*for (int dates = 0; dates < M; dates++) {
		cout <<".A_"<<dates<<"(VectorA["<<dates<<"]),"<< endl;
		}
		for (int dates = 0; dates < M; dates++) {
			cout <<".B_"<<dates<<"(VectorB["<<dates<<"]),"<< endl;
			}*/

	          loop: for (int dates = 0; dates < M; dates++) {

	              delta=(T)(AB[dates]-AB[dates+M]);
	     //        cout <<  "delta_SW = " << delta << endl;
	              sumatoria+= (T)(delta*delta);
	         //     cout <<"Results of sumatoria_SW: "<< dec << sumatoria << endl;


	         }
			 *C = sqrt((T)sumatoria);
		//	 cout <<"Results of C_SW: "<< dec << C << endl;

}



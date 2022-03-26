#include "EucHW.h"
#include <math.h>
#include <cmath>
#include <iostream>
#include <stdbool.h>
#include  <stdio.h>
using namespace std;




void EucHW (T2 AB[2*M],T *C){
#pragma HLS INTERFACE mode=s_axilite port=AB storage_impl=bram
#pragma HLS INTERFACE mode=s_axilite port=C
#pragma HLS INTERFACE mode=s_axilite port=return
#pragma HLS ARRAY_PARTITION variable=AB type=cyclic factor=8
	T delta, sumatoria = 0;

	          loop: for (int dates = 0; dates < M; dates++) {
					 #pragma HLS UNROLL
 	 	 	// 	 	 #pragma HLS pipeline
	              delta=(T)(AB[dates]-AB[dates+M]);
	     //        cout <<  "delta_SW = " << delta << endl;
	              sumatoria+= (T)(delta*delta);
	    //          cout <<"Results of sumatoria_SW: "<< dec << sumatoria << endl;


	         }
			 *C = sqrt((T)sumatoria);
		//	 cout <<"Results of C_SW: "<< dec << C << endl;

}


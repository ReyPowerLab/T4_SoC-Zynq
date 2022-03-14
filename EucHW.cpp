#include "EucHW.h"
#include <math.h>
#include <cmath>
#include <iostream>
#include <stdbool.h>
#include  <stdio.h>
using namespace std;




void EucHW (T2 A[M], T2 B[M],T *C){
// Si cambio T2 por chart o short incrementa mucho el uso de recursos
//#pragma HLS latency min=80 max=100 No da resultados adecuados viola restricicones de tiempo
//#pragma HLS INTERFACE   mode=ap_none  port=A
//#pragma HLS ARRAY_PARTITION variable=A type=cyclic factor=8
//#pragma HLS array_partition variable=A block dim=2
#pragma HLS ARRAY_PARTITION variable=A complete  // Con este pragma no alcanza recurso
//#pragma HLS INTERFACE mode=ap_none port=A
//#pragma HLS INTERFACE  mode=ap_none port=B no reconoce ap_none
//#pragma HLS ARRAY_PARTITION variable=B type=cyclic factor=8
//#pragma HLS array_partition variable=B block dim=2
#pragma HLS ARRAY_PARTITION variable=B complete  // Con este pragma no alcanza recurso
//#pragma HLS INTERFACE mode=ap_none port=B no reconoce
// con variables int estima menos recursos
//#pragma HLS INTERFACE mode=ap_none port=return

	T delta, sumatoria = 0;

	          loop: for (int dates = 0; dates < M; dates++) {
				//	 #pragma HLS UNROLL
 	 	 	 	 	 #pragma HLS pipeline
	              delta=(T)(A[dates]-B[dates]);
	     //        cout <<  "delta_SW = " << delta << endl;
	              sumatoria+= (T)(delta*delta);
	    //          cout <<"Results of sumatoria_SW: "<< dec << sumatoria << endl;


	         }
			 *C = sqrt((T)sumatoria);
		//	 cout <<"Results of C_SW: "<< dec << C << endl;

}


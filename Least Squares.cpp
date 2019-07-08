/*
* Least Squares Programming Assignment
* Coded in C++
* Victor Guo
* Mr. van Bemmel
* September 4th, 2018
*/

#include<bits/stdc++.h>

using namespace std;
vector<vector<double> > matrix;//stores vandermonde matrix
vector<vector<double> > matrixTranspose;//stores matrix transpose
vector<vector<double> > result;//stores a matrix during intermediate steps
vector<vector<double> > inverse;//stores the inverse matrix
vector<double> dataPointX;//store x values
vector<double> dataPointY;//store y values
vector<double> coeff;//stores the fitted coefficients
int num;//number of data points
int K; //degree of polynomial required to fit

//method to receive input
void input(){
	printf("Enter the number of Datapoints: ");
	scanf("%d",&num);
	printf("Enter degree of Polynomial: ");
	scanf("%d",&K);
	printf("Enter the data points: \n");
	//set up matrix
	for(int i = 0; i < num; i++){
		matrix.push_back(vector<double>());
		double tmpX,tmpY;
		scanf("%lf %lf",&tmpX,&tmpY);
		dataPointX.push_back(tmpX);//append value to list of x values
		dataPointY.push_back(tmpY);//append value to list of y values
		
		for(int j = K; j >= 0; j--){//iterate in descending order
			//add the powers of tmpX as required in vandermonde matrix
			matrix[i].push_back(pow(tmpX,j));
		}
	}
	
	//set up matrix transpose
	for(int i = 0; i <= K; i++){
		matrixTranspose.push_back(vector<double>());
		for(int j = 0; j < num; j++){
			//insert matrix values with indices switched 
			matrixTranspose[i].push_back(matrix[j][i]);
		}
	}
}

//method to print matrix
void printMatrix(vector<vector<double> >& vec){
	for(int i = 0; i < vec.size(); i++){
		for(int j = 0; j < vec[0].size(); j++){
			printf("%f ",vec[i][j]);
		}
		printf("\n");
	}
}

//method to print vector
void printVector(vector<double>& vec){
	for(int i = 0; i < vec.size(); i++){
		printf("%f\n",vec[i]);
	}
}

//method to print data
void printData(){
	for(int i = 0; i < dataPointY.size(); i++){
		printf("%f %f\n",dataPointX[i],dataPointY[i]);
	}
}

//method to multiply two matrices
void multiply(vector<vector<double> >& vec1,vector<vector<double> >& vec2){
	//iterate over each row in the first matrix
	for(int i = 0;  i < vec1.size(); i++){
		result.push_back(vector<double>());
		//iterate over each column in the second matrix
		for(int j = 0; j < vec2[0].size(); j++){
			double val = 0;
			for(int k = 0; k < vec2.size(); k++){
				//multiply corresponding values then add to sum
				val+=vec1[i][k]*vec2[k][j];
			}
			result[i].push_back(val);//append value to resulting matrix
		}
	}
}

//method to multiply matrix with vector
void multiplyVector(vector<vector<double> >& vec1,vector<double>& vec2){
	//for every row in the matrix
	for(int i = 0; i < vec1.size(); i++){
		double tmp = 0;
		//for every row in the vector
		for(int j = 0; j < vec2.size(); j++){
			//multiply corresponding values then add to sum
			tmp+=vec1[i][j]*vec2[j];
		}
		coeff.push_back(tmp);//append value to resulting vector
	}
}

//method to invert matrix
void invertMatrix(vector<vector<double> >& vec1){
	//append the identity matrix to the input matrix
	for(int i = 0; i < vec1.size(); i++){
		inverse.push_back(vector<double>());
		for(int j = 0; j < vec1[0].size(); j++){
			//append the input matrix values
			inverse[i].push_back(vec1[i][j]);
		}
		for(int j = 0; j < vec1[0].size(); j++){
			//append the identity matrix values
			inverse[i].push_back(i==j ? 1 : 0);
		}
	}
	
	//iterate over the columns
	for(int j = 0; j < vec1[0].size(); j++){
		//iterate over the rows
		for(int i = 0; i < vec1.size(); i++){
			int idx = 0;
			if(i==j){//this value in the matrix needs to become 1
				if(inverse[i][j] == 0){//if it is already 0
					//Find another value along this col. that is not 0
					for(int k = 0; k < vec1.size();k++){
						if(inverse[k][j] != 0){
							idx = k;//store this row number into idx
							break;
						}
					}
					//Row operation that adds row idx into current row
					for(int k = 0; k < inverse[0].size(); k++){
						inverse[i][k]+=inverse[idx][k];
					}
				}
				double tmp = inverse[i][j];
				//Do a row operation to scale the matrix value to 1
				for(int k = 0; k < inverse[0].size(); k++){
					inverse[i][k]*=(1.0/tmp);
				}
			}
			else{//this value in the matrix needs to become 0
				if(inverse[i][j] == 0){//if it is already 0
					//Find another value along this col. that is not 0
					for(int k = 0; k < vec1.size();k++){
						if(inverse[k][j] != 0){
							idx = k;//store this row number into idx
							break;
						}
					}
					//Row operation that adds row idx into current row
					for(int k = 0; k < inverse[0].size(); k++){
						inverse[i][k]+=inverse[idx][k];
					}
				}
				//subtract from current row a scaled multiple of row j
				double tmp = inverse[i][j];
				for(int k = 0; k < inverse[0].size(); k++){
					inverse[i][k]-=(inverse[j][k]*tmp/inverse[j][j]);
				}
			}
		}
	}
	
	//Erase the first part of the matrix (identity matrix)
	for(int i = 0; i < vec1.size(); i++){
		for(int j = 0; j < vec1[0].size();j++){
			inverse[i].erase(inverse[i].begin());
		}
	}
}

int main(){
	
	input(); //receive input

	printf("\n\n Your input: \n");
	printData();
	
	printf("\n\n Vandermonde Matrix: \n");
	printMatrix(matrix);
	
	printf("\n\n Transpose of Vandermonde Matrix: \n");
	printMatrix(matrixTranspose);
	
	printf("\n\n The product matrix: \n");
	multiply(matrixTranspose,matrix);
	printMatrix(result);
	
	printf("\n\n Inverse of product matrix: \n");
	invertMatrix(result);
	printMatrix(inverse);
	result.clear();
	multiply(inverse,matrixTranspose);
	multiplyVector(result,dataPointY);
	
	printf("\n\n Coefficients: \n");
	printVector(coeff);
	
	return 0;
}

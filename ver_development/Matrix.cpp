#include "Stdafx.h"

Matrix3X3 createFixed3X3Matrix(float init){		//3by3 행렬 초기화
	Matrix3X3 temp;

	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			temp.index[i][j] = init;
		}
	}

	return temp;
}

Matrix3X1 createFixed3X1Matrix(float init){		// 3by1 행렬 초기화
	Matrix3X1 temp;

	for(int i = 0; i < 1; i++){
		for(int j = 0; j < 3; j++){
			temp.index[i][j] = init;
		}
	}

	return temp;
}

Matrix3X1 multiplyMatrixNxM(Matrix3X3 temp, Matrix3X1 start){	// 3by3 * 3by1 행렬 곱셈
	Matrix3X1 result = createFixed3X1Matrix(0);

	for(int i = 0; i < 1; i++){
		for(int j = 0; j < 3; j++){
				//result.index[i][j] += (temp.index[j][k]*start.index[k][i]);
				result.index[i][j] += (temp.index[i][j]*start.index[j][i]);
		}
	}

	return result;
}

Matrix3X3 multiplyMatrices(Matrix3X3 a, Matrix3X3 b){
	Matrix3X3 temp = createFixed3X3Matrix(0);

	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			for(int k = 0; k < 3; k++){
				temp.index[i][j] += (a.index[i][j] * b.index[k][j]);
			}
		}
	}
	return temp;
}

void printMatrix(Matrix3X1 a){			// 3by1 행렬 원소 출력
	for(int i = 0; i < 1; i++){
		for(int j = 0; j < 3; j++){
			float temp = a.index[i][j];
			cout << temp << " ";
		}
		cout << endl;
	}
}

Matrix3X1 translate2DByMultiplication(Matrix3X1 start, float dx, float dy){	// 3by3 * 3by1 행렬을 이용한 평행이동
	Matrix3X3 temp;
	Matrix3X1 result;

	//행렬을 0으로 초기화
	temp = createFixed3X3Matrix(0);

	//곱셈을 할 3X3행렬을 설정
	temp.index[0][0] = 1;
	temp.index[1][1] = 1;
	temp.index[2][2] = 1;

	//평행이동할 양을 대입
	temp.index[0][2]=dx;
	temp.index[1][2]=dy;

	result = multiplyMatrixNxM(temp, start);

	return result;
}

Matrix3X1 translate2DByAddition(Matrix3X1 start, float dx, float dy){	// 덧셈을 이용한 평행이동	
	start.index[0][0] += dx;
	start.index[0][1] += dy;

	return start;
}

Matrix3X1 rotate2D(Matrix3X1 start, float theta){
	Matrix3X3 temp;
	Matrix3X1 result;

	// 행렬을 0으로 초기화
	temp = createFixed3X3Matrix(0);

	// 행렬에 필요한 값을 설정
	temp.index[0][0] = cos(DegreesToRads(theta));
	temp.index[1][1] = cos(DegreesToRads(theta));
	temp.index[2][2] = 1;

	temp.index[0][1] = -1*(sin(DegreesToRads(theta)));
	temp.index[1][0] = sin(DegreesToRads(theta));
	temp.index[2][2] = 1;

	result = multiplyMatrixNxM(temp,start);
	return result;
}
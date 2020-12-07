#include "Stdafx.h"

Matrix3X3 createFixed3X3Matrix(float init){		//3by3 ��� �ʱ�ȭ
	Matrix3X3 temp;

	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			temp.index[i][j] = init;
		}
	}

	return temp;
}

Matrix3X1 createFixed3X1Matrix(float init){		// 3by1 ��� �ʱ�ȭ
	Matrix3X1 temp;

	for(int i = 0; i < 1; i++){
		for(int j = 0; j < 3; j++){
			temp.index[i][j] = init;
		}
	}

	return temp;
}

Matrix3X1 multiplyMatrixNxM(Matrix3X3 temp, Matrix3X1 start){	// 3by3 * 3by1 ��� ����
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

void printMatrix(Matrix3X1 a){			// 3by1 ��� ���� ���
	for(int i = 0; i < 1; i++){
		for(int j = 0; j < 3; j++){
			float temp = a.index[i][j];
			cout << temp << " ";
		}
		cout << endl;
	}
}

Matrix3X1 translate2DByMultiplication(Matrix3X1 start, float dx, float dy){	// 3by3 * 3by1 ����� �̿��� �����̵�
	Matrix3X3 temp;
	Matrix3X1 result;

	//����� 0���� �ʱ�ȭ
	temp = createFixed3X3Matrix(0);

	//������ �� 3X3����� ����
	temp.index[0][0] = 1;
	temp.index[1][1] = 1;
	temp.index[2][2] = 1;

	//�����̵��� ���� ����
	temp.index[0][2]=dx;
	temp.index[1][2]=dy;

	result = multiplyMatrixNxM(temp, start);

	return result;
}

Matrix3X1 translate2DByAddition(Matrix3X1 start, float dx, float dy){	// ������ �̿��� �����̵�	
	start.index[0][0] += dx;
	start.index[0][1] += dy;

	return start;
}

Matrix3X1 rotate2D(Matrix3X1 start, float theta){
	Matrix3X3 temp;
	Matrix3X1 result;

	// ����� 0���� �ʱ�ȭ
	temp = createFixed3X3Matrix(0);

	// ��Ŀ� �ʿ��� ���� ����
	temp.index[0][0] = cos(DegreesToRads(theta));
	temp.index[1][1] = cos(DegreesToRads(theta));
	temp.index[2][2] = 1;

	temp.index[0][1] = -1*(sin(DegreesToRads(theta)));
	temp.index[1][0] = sin(DegreesToRads(theta));
	temp.index[2][2] = 1;

	result = multiplyMatrixNxM(temp,start);
	return result;
}
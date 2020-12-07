#pragma once

#define RadsToDegrees(radian) (float)((radian) * (180.0f / M_PI))	// ���̾��� ���ڷ� �޾Ƶ��̰� �� ������ ��ȯ�� ���� ����
#define DegreesToRads(degrees) (float)((degrees) * (M_PI / 180.0f))	// �� ������ ���� �������� ��ȯ

typedef struct tag_3X3{		// 3by3 ���
	float index[3][3];
} Matrix3X3;
typedef struct tag_3X1{		// 3by1 ���
	float index[3][1];
} Matrix3X1;

Matrix3X3 createFixed3X3Matrix(float init);
Matrix3X1 createFixed3X1Matrix(float init);
Matrix3X1 multiplyMatrixNxM(Matrix3X3 temp, Matrix3X1 start);
void printMatrix(Matrix3X1 a);
Matrix3X1 translate2DByMultiplication(Matrix3X1 start, float dx, float dy);
Matrix3X1 translate2DByAddition(Matrix3X1 start, float dx, float dy);
Matrix3X1 rotate2D(Matrix3X1 start, float theta);
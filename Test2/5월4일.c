#include<stdio.h>


int main() {
	//2차원 배열 선언
	int secarr[10][8];
	//초기값 지정(1)
	/*secarr[0][0] = 0;
	secarr[0][1] = 0;*/
	/*for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 8; j++) {
			secarr[i][j] = i+j;
		}
	}
	int s2[2][3] = { 1,2,3,4,5,6 };
	int s3[2][3] = { 10,20,30,
					 40,50,60 };
	int s4[2][3] = { {100,200,300},
					 {400,500,600} };
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 8; j++) {
			printf("%2d", secarr[i][j]);
		}
		printf("\n");
	}*/

	//초기값 지정2
	/*int s[2][3];
	printf("s[2][3]의 수를 차례로 입력: ");
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			scanf_s("%d",&s[i][j]);
		}
	}*/
	//처리 프로세스
	/*for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			printf("%2d", s[i][j]);
		}
		printf("\n");
	}*/
	//오늘의 연습
	//2차원 배열 선언 그것에 대한 연산 수행후 결과도출
	int arr[5][5];
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			arr[i][j] = i * 5 + j;
		}
	}
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			printf("%3d", arr[i][j]);
		}
		printf("\n");
	}


}
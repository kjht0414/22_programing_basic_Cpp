#include<stdio.h> //standard input output header file

int main() {//main function , int : integer (정수)
	//printf("안녕하세요?");
	//return 0;
	/*
	* 이건 주석
	*/
	//상수
	//0, 1, 2
	// 1.5 333.24
	// 'a' "안녕하세요?"

	//변수 선언
	/*int a;
	float b = 3.14;
	char c = 'A';*/
	//char d = "안녕하세요?"; 잘못된표현
	/*a = 365;
	printf("a = %d\n",a);
	printf("b = %.15f\n", b);
	printf("c = %c\n", c);*/

	/*
	int i;
	float f;
	char c;
	i = -3000;
	f =3.141592;
	c = '@';

	printf("i=%6.d;\nf=%6.2f;\nc=%6.c;\n", i, f, c);
	*/

	//1. 자료 입력
	//2. 자료 처리
	//3. 자료 출력

	//자료의 사칙연산 + - * /

	/*float big_moon;
	big_moon = 30;
	big_moon = big_moon + 1.2;
	
	printf("큰달 = %.1f;\n", big_moon);*/

	//1.c 시작

	//
	//int a, b;
	//a = 365;
	//b = 366;
	//int c;
	//if (a > b) { // >= < <= != ==
	//	c = 1;
	//}
	//else if (a == b) {
	//	c = 2;
	//}
	//else{
	//	c = 3;
	//}


	//// for 문 
	//int result=0;
	//int n;
	//// 1부터 n까지의 누적합 구하기
	//printf("n값을 입력하세요 : ");
	//scanf_s("%d",&n);
	//for (int i = 0; i <= n; i++) {   //++i
	//	result += i;
	//}
	//
	//printf("1부터 %d까지의 덧셈 : %d\n",n, result);


	//연습 : 단입력을 받고 구구단 계산을 해보시오.

	/*int multi ,result=0;
	printf("값을 입력 : ");
	scanf_s("%d", &multi);
	for (int i = 1; i < 10; i++) {
		printf("%d x %d = %d \n",multi , i ,i*multi);
	}*/

	// 1부터 n까지의 누적합 구하기 (while version)
	int n ,result=0 ,count=1;
	printf("값을 입력 : ");
	scanf_s("%d", &n);

	while (count <= n) {
		result += count;
		count++;
	}

	printf("1부터 %d까지의 덧셈 : %d\n", n, result);

}


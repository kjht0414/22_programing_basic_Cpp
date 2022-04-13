#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//함수 작성 방법
// (1) 리턴타입 명시
// (2) 함수명 작성
// (3) 함수명 뒤에 () 안에 매개변수(들) 지정
// (4) () 뒤에 {} 부여
// (4) 함수 내용을 {} 안에 작성
void myfunc() {
	printf("안녕하세요?\n");
	int a = 365;
	int b;
	printf("어떤 정수를 입력하시오:");
	scanf_s("%d", &b);
	printf("a + b = %d", a + b);
}

void triangle() {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j <= i; j++) {
			printf("*");
		}
		printf("\n");
	}
}


int main() {
	//int* ptr; //주소부여X, 초기값X
	//ptr = malloc(sizeof(int)); //heap에서 동적할당.
	//
	//printf("%p\n", ptr);
	////int a;
	////printf("변수 a의 주소: %p\n", &a);
	////*ptr = 100;
	////포인터의 초기화 방법
	////(1) 대상 포인터명 (2) 초기값 (3) 공간의 크기
	//memset(ptr, 0x00, sizeof(int)); //byte단위로 초기화 int 는 4byte 이기에 문제
	//printf("%d\n", *ptr);

	//free(ptr);

	/*printf("안녕하세요?\n");
	int a = 365;
	int b;
	printf("어떤 정수를 입력하시오:");
	scanf_s("%d",&b);
	printf("a + b = %d",a+b);*/

	/*myfunc();
	return 0;*/

	//오늘의 연습
	//리턴하지 않고 매개변수가 없는 함수를 하나 작성하고 실행
	triangle();
}
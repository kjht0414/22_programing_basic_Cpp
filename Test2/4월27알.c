#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void swap(int* arga, int* argb);

void e_swap(int sa, int sb);

int find(char* src, char dst);

int main() {
	//두 변수의 값을 맞교환
	int a = 10, b = 20;
	/*int temp;
	temp = a;
	a = b;
	b = temp;
	swap(&a, &b);
	printf("a = %d, b = %d",a,b);*/

	//printf("교환전 a = %d, b = %d\n", a, b);

	////e_swap(a,b);
	//swap(&a, &b);

	//printf("교환후 a = %d, b = %d\n", a, b);
	//printf("&a = %p, &b = %p\n",&a,&b);

	//오늘의 연습
	//간단한 처리를 하는 함수 만들기
	//주소를 넘겨받아야함
	
	char* s = "hello world";
	char dst = 'o';
	int pos = find(s, dst);
	printf("%s\n", s);
	printf("목표:%c\n", dst);
	printf("위치:%d\n", pos);
}

void swap(int* arga, int* argb) {
	int temp;
	printf("교환전(함수내) arga = %d, argb = %d\n", arga, argb);
	printf("&arga = %p, &argb = %p\n", &arga, &argb);
	temp = *arga;
	*arga = *argb;
	*argb = temp;
	printf("교환후(함수내) arga = %d, argb = %d\n", arga, argb);
	printf("&arga = %p, &argb = %p\n", &arga, &argb);
}

void e_swap(int sa, int sb) {
	int temp;
	printf("교환전(함수내) sa = %d, sb = %d\n", sa, sb);
	printf("&sa = %p, &sb = %p\n", &sa, &sb);
	temp = sa;
	sa = sb;
	sb = temp;
	printf("교환후(함수내) sa = %d, sb = %d\n", sa, sb);
	printf("&sa = %p, &sb = %p\n", &sa, &sb);
}

int find(char* src, char dst) {
	int count = 0;
	while (*(src + count)) {
		if (*(src + count) == dst) {
			return count;
		}
		count++;
	}
	return -1;
}
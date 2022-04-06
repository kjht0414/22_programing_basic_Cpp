#include<stdio.h>

int main() {
	////배열 -> 포인터  2022.04.06 array -> pointer
	//int a[5]={1,2,3,4,5};
	//int* ptr;
	//// 포인터에 특정 주소 배정
	//ptr = &a[0];

	//printf("%p: %d\n\n", ptr, *ptr);//주소출력용 포멧 -> %p
	//
	///*for (int i = 0; i < 5; i++) {
	//	printf("%d", a[i]);
	//}
	//printf("\n");*/
	////포인터로 배열 전체 값을 출력
	//for (int i = 0; i < 5; i++) {
	//	printf("%p: %d\n", ptr, *ptr);
	//	ptr++;
	//}
	//printf("\n");
	////포인터의 증감
	//printf("루프 종료후 ptr 주소:  %p\n", ptr);
	//ptr -= 3;
	//printf("포인터 감소후 상황 %p: %d\n", ptr, *ptr);

	//=================================================

	//오늘의 복습 2022.04.06
	//어떤 배열이 짝수를 가지도록 구성
	//배열의 개수는 5개로 제한
	//배열의 값은 for 또는 while로 초기화
	//이 배열을 포인터를 통해 인쇄할것
	//인쇄 종료후 포인터를 사용하여 내림차순으로 인쇄

	//1. 변수 선언
	int array[5]; 
	int* ptr=&array[0];
	//2. 배열 초기화 또는 값 변경 (짝수)
	for (int i = 0; i < 5; i++) {
		array[i] = i * 2;//*(ptr + i) = i*2;
	}
	//3. 오름차순 출력
	for (int i = 0; i < 5; i++) {
		printf("%d ", *ptr);
		ptr++;
	}
	printf("\n");
	//4. 내림차순 출력
	for (int i = 0; i < 5; i++) {
		ptr--;
		printf("%d ", *ptr);
	}
	printf("\n");

	//=================================================

	
}
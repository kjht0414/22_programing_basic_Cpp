#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void swap(int* arga, int* argb);

void e_swap(int sa, int sb);

int find(char* src, char dst);

int main() {
	//�� ������ ���� �±�ȯ
	int a = 10, b = 20;
	/*int temp;
	temp = a;
	a = b;
	b = temp;
	swap(&a, &b);
	printf("a = %d, b = %d",a,b);*/

	//printf("��ȯ�� a = %d, b = %d\n", a, b);

	////e_swap(a,b);
	//swap(&a, &b);

	//printf("��ȯ�� a = %d, b = %d\n", a, b);
	//printf("&a = %p, &b = %p\n",&a,&b);

	//������ ����
	//������ ó���� �ϴ� �Լ� �����
	//�ּҸ� �Ѱܹ޾ƾ���
	
	char* s = "hello world";
	char dst = 'o';
	int pos = find(s, dst);
	printf("%s\n", s);
	printf("��ǥ:%c\n", dst);
	printf("��ġ:%d\n", pos);
}

void swap(int* arga, int* argb) {
	int temp;
	printf("��ȯ��(�Լ���) arga = %d, argb = %d\n", arga, argb);
	printf("&arga = %p, &argb = %p\n", &arga, &argb);
	temp = *arga;
	*arga = *argb;
	*argb = temp;
	printf("��ȯ��(�Լ���) arga = %d, argb = %d\n", arga, argb);
	printf("&arga = %p, &argb = %p\n", &arga, &argb);
}

void e_swap(int sa, int sb) {
	int temp;
	printf("��ȯ��(�Լ���) sa = %d, sb = %d\n", sa, sb);
	printf("&sa = %p, &sb = %p\n", &sa, &sb);
	temp = sa;
	sa = sb;
	sb = temp;
	printf("��ȯ��(�Լ���) sa = %d, sb = %d\n", sa, sb);
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
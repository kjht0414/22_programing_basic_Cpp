#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//�Լ� �ۼ� ���
// (1) ����Ÿ�� ���
// (2) �Լ��� �ۼ�
// (3) �Լ��� �ڿ� () �ȿ� �Ű�����(��) ����
// (4) () �ڿ� {} �ο�
// (4) �Լ� ������ {} �ȿ� �ۼ�
void myfunc() {
	printf("�ȳ��ϼ���?\n");
	int a = 365;
	int b;
	printf("� ������ �Է��Ͻÿ�:");
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
	//int* ptr; //�ּҺο�X, �ʱⰪX
	//ptr = malloc(sizeof(int)); //heap���� �����Ҵ�.
	//
	//printf("%p\n", ptr);
	////int a;
	////printf("���� a�� �ּ�: %p\n", &a);
	////*ptr = 100;
	////�������� �ʱ�ȭ ���
	////(1) ��� �����͸� (2) �ʱⰪ (3) ������ ũ��
	//memset(ptr, 0x00, sizeof(int)); //byte������ �ʱ�ȭ int �� 4byte �̱⿡ ����
	//printf("%d\n", *ptr);

	//free(ptr);

	/*printf("�ȳ��ϼ���?\n");
	int a = 365;
	int b;
	printf("� ������ �Է��Ͻÿ�:");
	scanf_s("%d",&b);
	printf("a + b = %d",a+b);*/

	/*myfunc();
	return 0;*/

	//������ ����
	//�������� �ʰ� �Ű������� ���� �Լ��� �ϳ� �ۼ��ϰ� ����
	triangle();
}
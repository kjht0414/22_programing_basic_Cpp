#include<stdio.h> //standard input output header file

int main() {//main function , int : integer (����)
	//printf("�ȳ��ϼ���?");
	//return 0;
	/*
	* �̰� �ּ�
	*/
	//���
	//0, 1, 2
	// 1.5 333.24
	// 'a' "�ȳ��ϼ���?"

	//���� ����
	/*int a;
	float b = 3.14;
	char c = 'A';*/
	//char d = "�ȳ��ϼ���?"; �߸���ǥ��
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

	//1. �ڷ� �Է�
	//2. �ڷ� ó��
	//3. �ڷ� ���

	//�ڷ��� ��Ģ���� + - * /

	/*float big_moon;
	big_moon = 30;
	big_moon = big_moon + 1.2;
	
	printf("ū�� = %.1f;\n", big_moon);*/

	//1.c ����

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


	//// for �� 
	//int result=0;
	//int n;
	//// 1���� n������ ������ ���ϱ�
	//printf("n���� �Է��ϼ��� : ");
	//scanf_s("%d",&n);
	//for (int i = 0; i <= n; i++) {   //++i
	//	result += i;
	//}
	//
	//printf("1���� %d������ ���� : %d\n",n, result);


	//���� : ���Է��� �ް� ������ ����� �غ��ÿ�.

	/*int multi ,result=0;
	printf("���� �Է� : ");
	scanf_s("%d", &multi);
	for (int i = 1; i < 10; i++) {
		printf("%d x %d = %d \n",multi , i ,i*multi);
	}*/

	// 1���� n������ ������ ���ϱ� (while version)
	int n ,result=0 ,count=1;
	printf("���� �Է� : ");
	scanf_s("%d", &n);

	while (count <= n) {
		result += count;
		count++;
	}

	printf("1���� %d������ ���� : %d\n", n, result);

}


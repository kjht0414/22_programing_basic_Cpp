#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void printNcounts(char* str) {
	printf("%s\n", str);

	int count = 0;
	while (str[count]) { //while(str[count] != 0) 0�� false 0�̿ܿ��� ���� true '\0'==0==NULL����
		count++;
	}
	printf("���ڼ�: %d\n", count);
	for (int i = 0; i < count; i++) {
		printf("%p %c\n", &str[i], str[i]);
	}
}

int main() {
	////char* str;
	////str = malloc(sizeof(char) * 10);

	//char* str = "Hello world";
	////char s[20] = "�ȳ��ϼ���?";

	//printNcounts(str);

	//str = "Programming Basic 2nd Class";
	//printNcounts(str);


	/*
	������ ����
	���ڿ��� �����Ͱ� ����Ű���� �ϰ�,
	�ش� ���ڿ��� Ȧ����°�� �ش��ϴ� ���ڸ�
	���
	*/

	char* str;
	char c;
	int index;
	int char_counter = 0;
	str = "Hello World";

	printf("%s\n", str);

	while (c = *(str + (index = (char_counter * 2 + 1)))) {
		printf("%d: %c\n", index,c);
		char_counter++;
	}
}
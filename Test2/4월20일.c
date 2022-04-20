#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void printNcounts(char* str) {
	printf("%s\n", str);

	int count = 0;
	while (str[count]) { //while(str[count] != 0) 0은 false 0이외에는 전부 true '\0'==0==NULL문자
		count++;
	}
	printf("글자수: %d\n", count);
	for (int i = 0; i < count; i++) {
		printf("%p %c\n", &str[i], str[i]);
	}
}

int main() {
	////char* str;
	////str = malloc(sizeof(char) * 10);

	//char* str = "Hello world";
	////char s[20] = "안녕하세요?";

	//printNcounts(str);

	//str = "Programming Basic 2nd Class";
	//printNcounts(str);


	/*
	오늘의 연습
	문자열을 포인터가 가리키도록 하고,
	해당 문자열의 홀수번째에 해당하는 글자를
	출력
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
/*for, while, 정수 또는 실수를 가리키는 포인터, 함수(매개변수, 리턴 없음)
등으로 어떤 프로그램을 만들것
버전을 2가지로 만들것 ex)첮번째 버전이 배열 없이 정수 및 실수 자료를 직접 대입하여
처리하고 for 문, 함수 등으로 구현했다면
두번째 버전은 scanf로 입력하고 while 문으로 함수없이 구현
*/

/*
* 학생 성적 처리 프로그램을 제작(common)
* 학생수를 미리 지정하고 정보를 입력(version1)
* ========================나중에 할것===========================
* 과목을 커스텀 가능하도록(version2)
* 학생수를 미리 지정하지 않고 동적으로 추가, 삭제, 삽입이 가능할것(version2)
* 학생의 특정 속성(이름,번호,성적 등)으로 정렬할 수 있을것(version2)
*/

/*
* while 문이 아니라 for 문으로 구현
* 포인터를 배열로 변경
* 장점: 더 보기 좋은 코드
*/

#include<stdio.h>
#include<stdlib.h>

int get_str_size(char* str) {//for 문으로 구현
	int count;
	for (count = 0; str[count]; count++);
	return count;
}

void str_copy(char* dst, char* src) {//for 문으로 구현
	int count;
	char c;
	for (count = 0; c = src[count]; count++) {
		dst[count] = c;
	}
	dst[count] = c;
}

void student_inform_name(char** names, int index) {
	char name_temp[0xFF];
	int name_size;

	scanf_s("%s", name_temp, 0xFF);

	name_size = get_str_size(name_temp);
	names[index] = malloc(sizeof(char) * (name_size + 1));
	str_copy(names[index], name_temp);
}

void student_inform_grade(int* grade) {
	scanf_s("%d", grade);
}

void print_students(char** names, int* kor_grades, int* math_grades, int* eng_grades, float* avg_grades, int size) {
	char* name;
	for (int i = 0; i < size; i++) {
		name = names[i];
		printf("이름: %s\n", name);
		printf("국어: %d\n", kor_grades[i]);
		printf("수학: %d\n", math_grades[i]);
		printf("영어: %d\n", eng_grades[i]);
		printf("평균: %.2f\n", avg_grades[i]);
		printf("=========================\n");
	}
}

int main() {
	int student_size;
	char** student_names;
	int* student_grades_kor;
	int* student_grades_math;
	int* student_grades_eng;

	float* student_grades_avg;

	printf("학생들의 수:");
	scanf_s("%d", &student_size);

	student_names = malloc(sizeof(char*) * student_size);
	student_grades_kor = malloc(sizeof(int) * student_size);
	student_grades_math = malloc(sizeof(int) * student_size);
	student_grades_eng = malloc(sizeof(int) * student_size);

	student_grades_avg = malloc(sizeof(float) * student_size);

	for (int i = 0; i < student_size; i++) {
		printf("(%d/%d)번째 학생의 이름:", i + 1, student_size);
		student_inform_name(student_names, i);

		printf("(%d/%d)번째 학생의 국어:", i + 1, student_size);
		student_inform_grade(&student_grades_kor[i]);

		printf("(%d/%d)번째 학생의 수학:", i + 1, student_size);
		student_inform_grade(&student_grades_math[i]);

		printf("(%d/%d)번째 학생의 영어:", i + 1, student_size);
		student_inform_grade(&student_grades_eng[i]);

		for (int i = 0; i < student_size; i++) {
			student_grades_avg[i] = 0;
			student_grades_avg[i] += student_grades_kor[i];
			student_grades_avg[i] += student_grades_math[i];
			student_grades_avg[i] += student_grades_eng[i];
			student_grades_avg[i] /= 3;
		}
		printf("=========================\n");
	}

	print_students(student_names, student_grades_kor, student_grades_math, student_grades_eng, student_grades_avg, student_size);
}
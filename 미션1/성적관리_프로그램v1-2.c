/*for, while, ���� �Ǵ� �Ǽ��� ����Ű�� ������, �Լ�(�Ű�����, ���� ����)
������ � ���α׷��� �����
������ 2������ ����� ex)�R��° ������ �迭 ���� ���� �� �Ǽ� �ڷḦ ���� �����Ͽ�
ó���ϰ� for ��, �Լ� ������ �����ߴٸ�
�ι�° ������ scanf�� �Է��ϰ� while ������ �Լ����� ����
*/

/*
* �л� ���� ó�� ���α׷��� ����(common)
* �л����� �̸� �����ϰ� ������ �Է�(version1)
* ========================���߿� �Ұ�===========================
* ������ Ŀ���� �����ϵ���(version2)
* �л����� �̸� �������� �ʰ� �������� �߰�, ����, ������ �����Ұ�(version2)
* �л��� Ư�� �Ӽ�(�̸�,��ȣ,���� ��)���� ������ �� ������(version2)
*/

/*
* while ���� �ƴ϶� for ������ ����
* �����͸� �迭�� ����
* ����: �� ���� ���� �ڵ�
*/

#include<stdio.h>
#include<stdlib.h>

int get_str_size(char* str) {//for ������ ����
	int count;
	for (count = 0; str[count]; count++);
	return count;
}

void str_copy(char* dst, char* src) {//for ������ ����
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
		printf("�̸�: %s\n", name);
		printf("����: %d\n", kor_grades[i]);
		printf("����: %d\n", math_grades[i]);
		printf("����: %d\n", eng_grades[i]);
		printf("���: %.2f\n", avg_grades[i]);
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

	printf("�л����� ��:");
	scanf_s("%d", &student_size);

	student_names = malloc(sizeof(char*) * student_size);
	student_grades_kor = malloc(sizeof(int) * student_size);
	student_grades_math = malloc(sizeof(int) * student_size);
	student_grades_eng = malloc(sizeof(int) * student_size);

	student_grades_avg = malloc(sizeof(float) * student_size);

	for (int i = 0; i < student_size; i++) {
		printf("(%d/%d)��° �л��� �̸�:", i + 1, student_size);
		student_inform_name(student_names, i);

		printf("(%d/%d)��° �л��� ����:", i + 1, student_size);
		student_inform_grade(&student_grades_kor[i]);

		printf("(%d/%d)��° �л��� ����:", i + 1, student_size);
		student_inform_grade(&student_grades_math[i]);

		printf("(%d/%d)��° �л��� ����:", i + 1, student_size);
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
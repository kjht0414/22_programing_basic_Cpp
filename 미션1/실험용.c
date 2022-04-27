#include<stdio.h>
#include<stdlib.h>
#include<memory.h>

struct LinkedList{
	void* data;
	struct LinkedList* next;
};

struct Subject {
	int grade;
};

struct Student {
	char* name;
	int id;
	struct Subject* grades;
};



int get_str_size(char* str);
void str_copy(char* dst, char* src);
int same_string(char* a, char* b);
void byte_copy(char* dst, char* src, int size);

struct LinkedList* subjects = 0;
void print_subjects();
void set_subjects();
void modify_subject();
void insert_subject();
void append_subject();


struct LinkedList* students = 0;
void print_students();
void set_students();

int request_linked_index();

struct LinkedList* append_linked(struct LinkedList* dst);
struct LinkedList* points2linked(void** src, int unit, int size);
struct LinkedList* insert_linked(struct LinkedList* dst, struct LinkedList* src, int index);
int length_linked(struct LinkedList* node);


int main() {
	char commend[1024];
	
	
	while (1) {
		scanf_s(" %[^\n]", commend, 1024);
		if (same_string(commend, "/subjects")) {
			print_subjects();
		}
		else if (same_string(commend, "/append subject")) {
			append_subject();
		}
		else if (same_string(commend, "/insert subject")) {
			insert_subject();
		}
		else if (same_string(commend, "/modify subject")) {
			modify_subject();
		}
		else if (same_string(commend, "/set subjects")) {
			set_subjects();
		}
		else if (same_string(commend, "/students")) {
			printf("a");
			print_students();
		}
		else if (same_string(commend, "/set students")) {
			printf("b");
			set_students();
		}
		else if (same_string(commend, "/help")) {
			printf("help\n");
		}
		else {

		}
	}
 }

int get_str_size(char* str) {
	int count = 0;
	while (*(str + count)) {
		count++;
	}
	return count;
}

int same_string(char* a, char* b) {
	if (get_str_size(a) < get_str_size(b)) {
		char* temp = b;
		b = a;
		a = temp;
	}
	int count = 0;
	while (*(a + count)) {
		if (*(a + count) != *(b + count)) {
			return 0;
		}
		count++;
	}
	return 1;
}

void str_copy(char* dst, char* src) {
	int count = 0;
	char c;
	do {
		c = *(src + count);
		*(dst + count) = c;
		count++;
	} while (c);
}

void print_subjects() {
	if (!subjects) {
		printf("you need to set_subjects");
		return;
	}

	struct LinkedList* node= subjects;
	int subject_code = 0;
	do {
		printf("[%d]%s\n", subject_code, *(char**)(node->data));
		subject_code++;
	} while (node = node->next);
}

void set_subjects() {
	int subjects_size;
	char** subjects_pointer;
	printf("subjects size:");
	scanf_s("%d", &subjects_size);
	subjects_pointer = malloc(sizeof(char*) * (subjects_size ));
	for (int i = 0; i < subjects_size; i++) {
		char name[256];
		printf("(%d/%d)-th subject's name:", i+1, subjects_size);
		scanf_s("%s", name, 256);
		*(subjects_pointer + i) = malloc(sizeof(char) * (get_str_size(name) + 1));
		str_copy(*(subjects_pointer + i),name);
	}
	
	 subjects = points2linked(subjects_pointer, sizeof(char*), subjects_size);
	 free(subjects_pointer);
}

void modify_subject() {
	int subjects_index = request_linked_index();
	if (subjects_index == -1) {
		return 0;
	}
	char name[256];
	struct LinkedList* node = subjects;
	for (int i = 0; i < subjects_index; i++) {
		if (!node->next) {
			printf("index was over length of subjects\n");
			return;
		}
		node = node->next;
	}
	printf("subject name:");
	scanf_s("%s",name,256);
	free(*(char**)(node->data));
	*(char**)node->data = malloc(sizeof(char) * (get_str_size(name) + 1));
	str_copy(*(char**)node->data, name);
}

void insert_subject() {
	int subjects_index = request_linked_index();
	if (subjects_index == -1) {
		return 0;
	}
	struct LinkedList* node = subjects;
	for (int i = 0; i < subjects_index-1; i++) {
		if (!node->next) {
			printf("index was over length of subjects\n");
			return;
		}
		node = node->next;
	}
	char name[256];
	printf("subject name:");
	scanf_s("%s", name, 256);
	struct LinkedList* new_linked = malloc(sizeof(struct LinkedList));
	memset(new_linked, 0, sizeof(struct LinkedList));
	(char**)new_linked->data = malloc(sizeof(char*));
	*(char**)new_linked->data = malloc(sizeof(char) * (get_str_size(name) + 1));
	str_copy(*(char**)new_linked->data, name);
	subjects = insert_linked(subjects, new_linked, subjects_index);
}

void append_subject() {
	char name[256];
	struct LinkedList* node = subjects;
	printf("subject name:");
	scanf_s("%s", name, 256);
	struct LinkedList* new_linked = append_linked(node);
	(char**)new_linked->data = malloc(sizeof(char*));
	*(char**)new_linked->data = malloc(sizeof(char) * (get_str_size(name) + 1));
	str_copy(*(char**)new_linked->data, name);
}

int request_linked_index() {
	int subjects_index;
	if (!subjects) {
		printf("you need to set_subjects\n");
		return -1;
	}
	printf("subjects index:");
	scanf_s("%d", &subjects_index);
	return subjects_index;
}

struct LinkedList* append_linked(struct LinkedList* dst) {
	struct LinkedList* node;
	struct LinkedList* new_linked;
	node = dst;
	while (node->next) {
		node = node->next;
	}
	new_linked = malloc(sizeof(struct LinkedList));
	memset(new_linked, 0, sizeof(struct LinkedList));
	node->next = new_linked;
	return new_linked;
}

//struct LinkedList* points2linked(void** src, int size) {
//	struct LinkedList* result;
//	struct LinkedList* node;
//	result = malloc(sizeof(struct LinkedList));
//	result->data = *src;
//	result->next = 0;
//	node = result;
//	for (int i = 1; i < size; i++) {
//		append_linked(node);
//		node = node->next;
//		node->data = *(src + i);
//	}
//	return result;
//}

void byte_copy(char* dst, char* src, int size) {
	for (int i = 0; i < size; i++) {
		*(dst + i) = *(src + i);
	}
}

struct LinkedList* points2linked(void* src, int unit, int size) {
	struct LinkedList* result;
	struct LinkedList* node;
	result = malloc(sizeof(struct LinkedList));
	result->data = malloc(unit);
	byte_copy((char*)result->data, ((char*)src)+0, unit);
	result->next = 0;
	node = result;
	for (int i = 1; i < size; i++) {
		node=append_linked(node);
		node->data = malloc(unit);
		byte_copy((char*)node->data, ((char*)src)+i*unit, unit);
	}
	return result;
}

struct LinkedList* insert_linked(struct LinkedList* dst, struct LinkedList* src, int index) {
	struct LinkedList* node;
	struct LinkedList* temp;
	if (index <= 0) {
		node = src;
		temp = dst;
	}
	else {
		node = dst;
		for (int i = 0; i < index - 1; i++) {
			node = node->next;
		}
		temp = node->next;
		node->next = src;
	}
	
	while (node->next) {
		node = node->next;
	}
	node->next = temp;

	if (index <= 0) {
		return src;
	}
	else {
		return dst;
	}
}

int length_linked(struct LinkedList* node) {
	int count = 1;
	while (node->next)
	{
		node = node->next;
		count++;
	}
	return count;
}

void print_students() {
	if (!subjects) {
		printf("you need to set_subjects\n");
		return;
	}else if (!students) {
		printf("you need to set_students\n");
		return;
	}


	struct LinkedList* subjects_node = subjects;
	struct LinkedList* node = students;
	int subjects_length = 0;
	printf("name\tid");
	do {
		char* subject_name = *(char**)subjects_node->data;
		printf("\t%s",subject_name);
		subjects_length++;
	} while (subjects_node = subjects_node->next);
	printf("\n");
	do {
		struct Student student = *(struct Student*)node->data;
		printf("%s\t%d", student.name, student.id);
		for (int i = 0; i < subjects_length; i++) {
			printf("\t%d", student.grades[i]);
		}
		printf("\n");
	} while (node = node->next);
}

void set_students() {
	char name[256];
	int length,count=0,subjects_length=length_linked(subjects);
	struct Student* student;
	printf("when you want to stop, input /end\n");
	while (1)
	{
		printf("input students's name:");
		scanf_s(" %[^\n]", name, 256);
		if (same_string(name,"/end")) {
			break;
		}
		student = malloc(sizeof(struct Student));
		memset(student, 0, sizeof(struct Student));
		student->name = malloc(sizeof(char) * (get_str_size(name) + 1));
		str_copy(student->name, name);
		student->id = count + 1;
		student->grades = malloc(sizeof(struct Subject) * subjects_length);
		memset(student->grades, 0, sizeof(struct Subject) * subjects_length);
		if (students == 0) {
			students = malloc(sizeof(struct LinkedList));
			memset(students, 0, sizeof(struct LinkedList));
			(struct Student*)students->data = student;
		}
		else {
			struct LinkedList* new_linked=append_linked(students);
			(struct Student*)new_linked->data = student;
		}
	}
}
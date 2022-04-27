#include<stdio.h>
#include<stdlib.h>
#include<memory.h>

struct LinkedList
{
	void* data;
	struct LinkedList* next;
};

struct Subject {
	int code;
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

struct LinkedList* subjects = 0;
void print_subjects();
void set_subjects();

void append_linked(struct LinkedList* dst);
struct LinkedList* points2linked(void** src, int size);//문제있음 unit

int main() {
	char commend[1024];

	int a[4] = {1,2,3,4};
	struct LinkedList* b;
	b = points2linked(a, 4);
	do {
		printf("%d\n", *(int*)(b->data));
	} while (b=b->next);

	while (1) {
		scanf_s(" %[^\n]", commend, 1024);
		if (same_string(commend, "/subjects")) {
			print_subjects();
		}else if (same_string(commend, "/set subjects")) {
			set_subjects();
		}else if (same_string(commend, "/help")) {
			printf("help\n");
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
	char* subject_name;
	int subject_code = 0;
	do {
		subject_name = (char*)(node->data);
		printf("[%d]%s\n", subject_code, subject_name);
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
		*(subjects_pointer + i) = malloc(sizeof(char) * get_str_size(name));
		str_copy(*(subjects_pointer + i),name);
	}
	
	 subjects = points2linked(subjects_pointer, subjects_size);
	 free(subjects_pointer);
}

void append_linked(struct LinkedList* dst) {
	struct LinkedList* node;
	struct LinkedList* new_linked;
	node = dst;
	while (node->next) {
		node = node->next;
	}
	new_linked = malloc(sizeof(struct LinkedList));
	memset(new_linked, 0, sizeof(struct LinkedList));
	node->next = new_linked;
}

struct LinkedList* points2linked(void** src, int size) {
	struct LinkedList* result;
	struct LinkedList* node;
	result = malloc(sizeof(struct LinkedList));
	result->data = *src;
	result->next = 0;
	node = result;
	for (int i = 1; i < size; i++) {
		append_linked(node);
		node = node->next;
		node->data = *(src + i);
	}
	return result;
}
#include<stdio.h>
#include<stdlib.h>
#include <stdarg.h>
#include<memory.h>


#define COMPARE_NAME 0
#define COMPARE_ID 1
#define COMPARE_GRADE 2
#define COMPARE_GRADE_AVARAGE 3


struct LinkedList {
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
int str_euc_compare(char* big, char* small);
char* create_merged_str(char* front, char* back);
char** split_str_by_char(char* str, char sp, int* length);
int str2int(char* str);
int isInt(char* str);

struct LinkedList* subjects = 0;
void print_subjects();
void set_subjects();
void modify_subject();
void insert_subject();
void append_subject();


struct LinkedList* students = 0;
void print_students();
void set_students();
void sort_students();

int sort_compare_by_option(struct Student* f, struct Student* b,int option, int args_length,void* args);
int compare_grade(struct Student* f, struct Student* b, int subject_code);
int compare_grade_avarage(struct Student* f, struct Student* b, int args_length,int* subject_codes);
void sort_students_optoin(int option, int args_length,void* args);


void save_by_file_path(char* path);
void load_by_file_path(char* path);

int request_linked_index();

struct LinkedList* append_linked(struct LinkedList* dst);
struct LinkedList* points2linked(void* src, int unit, int size);
void* linked2points(struct LinkedList* src, int unit, int* length);
struct LinkedList* insert_linked(struct LinkedList* dst, struct LinkedList* src, int index);
int length_linked(struct LinkedList* node);
struct LinkedList* pop_linked(struct LinkedList* node, int index);
void delete_linked(struct LinkedList* node);


int main() {
	/*int aa[] = {1,2,3,4,5};
	struct LinkedList* source = points2linked(aa, sizeof(int), 5);
	struct LinkedList* head = source;
	do {
		printf("%d ", *(int*)head->data);
	} while (head = head->next);
	printf("\n");

	
	struct LinkedList* pop = pop_linked(source, -1);
	printf("%d\n", *(int*)pop->data);
	free(pop);
	int length;
	int* bb = linked2points(source, sizeof(int), &length);
	for (int i = 0; i < length; i++) {
		printf("%d ", bb[i]);
	}
	printf("\n");

	delete_linked(source);
	free(bb);
	*/

	/*
	char str[] = "sort students by name";
	int len;
	char** sp = split_str_by_char(str, ' ', &len);
	for (int i = 0; i < len; i++) {
		printf("|%s|\n", sp[i]);
		free(sp[i]);
	}

	free(sp);
	*/

	char commend[1024];
	
	char** command_splits;
	int command_splits_length;


	while (1) {
		scanf_s(" %[^\n]", commend, 1024);
		command_splits = split_str_by_char(commend, ' ', &command_splits_length);
		/*
		for (int i = 0; i < command_splits_length; i++) {
			printf("%s\n", command_splits[i]);
		}
		*/
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
			print_students();
		}
		else if (same_string(commend, "/set students")) {
			set_students();
		}
		else if (command_splits_length >= 2 && same_string(command_splits[0],"/sort") && same_string(command_splits[1], "students")) {
			sort_students(command_splits, command_splits_length);
		}
		else if (same_string(commend, "/save")) {
			if (!subjects) {
				printf("you need to set_subjects\n");
				continue;
			}
			else if (!students) {
				printf("you need to set_students\n");
				continue;
			}
			printf("save_file_path: ");
			char path[1024];
			scanf_s("%s",path,1024);

			save_by_file_path(path);
		}
		else if (same_string(commend, "/load")) {
			printf("load_file_path: ");
			char path[1024];
			scanf_s("%s", path, 1024);

			load_by_file_path(path);
		}
		else if (same_string(commend, "/help")) {
			
			printf("==============");
			printf("[Commands lists]");
			printf("==============\n");
			printf("/subjects\n");
			printf("/append subject\n");
			printf("/insert subject\n");
			printf("/modify subject\n");
			printf("/set subjects\n");

			printf("/students\n");
			printf("/set students\n");

			printf("/sort students\n");

			printf("/save\n");
			printf("/load\n");
			printf("==============");
			printf("================");
			printf("==============\n");
		}
		else {
			printf("If you want to know commands, input /help for help\n");
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

	struct LinkedList* node = subjects;
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
	subjects_pointer = malloc(sizeof(char*) * (subjects_size));
	for (int i = 0; i < subjects_size; i++) {
		char name[256];
		printf("(%d/%d)-th subject's name:", i + 1, subjects_size);
		scanf_s("%s", name, 256);
		*(subjects_pointer + i) = malloc(sizeof(char) * (get_str_size(name) + 1));
		str_copy(*(subjects_pointer + i), name);
	}

	delete_linked(subjects);

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
	scanf_s("%s", name, 256);
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
	for (int i = 0; i < subjects_index - 1; i++) {
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
	byte_copy((char*)result->data, ((char*)src) + 0, unit);
	result->next = 0;
	node = result;
	for (int i = 1; i < size; i++) {
		node = append_linked(node);
		node->data = malloc(unit);
		byte_copy((char*)node->data, ((char*)src) + i * unit, unit);
	}
	return result;
}

void* linked2points(struct LinkedList* src, int unit, int* length) {
	struct LinkedList* head = src;
	void* result;
	*length = length_linked(head);
	result = malloc(unit * (*length));
	for (int i = 0; i < *length; i++) {
		byte_copy(((char*)result) + i * unit, (char*)head->data, unit);
		head = head->next;
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

struct LinkedList* pop_linked(struct LinkedList* node, int index) {
	struct LinkedList* head = node;
	int length = length_linked(node);
	index = length + index;
	index %= length;

	if (index == 0) {
		return head;
	}
	else {
		struct LinkedList* temp = 0;
		for (int i = 0; i < index - 1; i++) {
			head = head->next;
		}
		temp = head->next;
		head->next = head->next->next;
		return temp;
	}
}

void delete_linked(struct LinkedList* node) {
	if (node == 0)return;
	struct LinkedList* pop = 0;
	while (pop != node) {
		if (pop != 0) {
			if (pop->data != 0) {
				free(pop->data);
			}
			free(pop);
		}
		pop = pop_linked(node, -1);
	}
	if (pop->data != 0) {
		free(pop->data);
	}
	free(pop);
}

void print_students() {
	if (!subjects) {
		printf("you need to set_subjects\n");
		return;
	}
	else if (!students) {
		printf("you need to set_students\n");
		return;
	}


	struct LinkedList* subjects_node = subjects;
	struct LinkedList* node = students;
	int subjects_length = 0;
	printf("name\tid");
	do {
		char* subject_name = *(char**)subjects_node->data;
		printf("\t%s", subject_name);
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
	int length, count = 0, subjects_length = length_linked(subjects);
	struct Student* student;
	printf("when you want to stop, input /end\n");
	delete_linked(students);

	while (1)
	{
		printf("input students's name:");
		scanf_s(" %[^\n]", name, 256);
		if (same_string(name, "/end")) {
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
			struct LinkedList* new_linked = append_linked(students);
			(struct Student*)new_linked->data = student;
		}
	}
}

int sort_compare_by_option(struct Student* f, struct Student* b,int option, int args_length,void* args) {
	/*
	* this is sort compare function for descending order
	* 
	* if f is bigger than b -> return 1
	* if b is bigger than f -> return -1
	* if f is same with b -> return 0
	* 
	* default name option
	*/
	switch(option) {
	case COMPARE_NAME:
		return str_euc_compare(f->name, b->name);
		break;
	case COMPARE_ID:
		if (f->id > b->id) {
			return 1;
		}
		else if (f->id < b->id) {
			return -1;
		}
		else {
			return 0;
		}
		break;
	case COMPARE_GRADE:
		return compare_grade(f, b, *(int*)args);
		break;
	case COMPARE_GRADE_AVARAGE:
		return compare_grade_avarage(f, b, args_length,(int*)args);
		break;
	default:
		return str_euc_compare(f->name, b->name);
		break;
	}

}

int compare_grade(struct Student* f, struct Student* b,int subject_code) {
	if (f->grades[subject_code].grade > b->grades[subject_code].grade) {
		return 1;
	}
	else if (f->grades[subject_code].grade < b->grades[subject_code].grade) {
		return -1;
	}
	else {
		return 0;
	}
}

int compare_grade_avarage(struct Student* f, struct Student* b, int args_length,int* subject_codes) {
	
	float avg_f=0, avg_b = 0;
	for (int i = 0; i < args_length; i++) {
		avg_f += f->grades[subject_codes[i]].grade;
		avg_b += b->grades[subject_codes[i]].grade;
	}
	avg_f /= args_length;
	avg_b /= args_length;

	if (avg_f > avg_b) {
		return 1;
	}
	else if (avg_f < avg_b) {
		return -1;
	}
	else {
		return 0;
	}
}

void sort_students_optoin(int option, int args_length,void* args) {
	if (!subjects) {
		printf("you need to set_subjects\n");
		return;
	}
	else if (!students) {
		printf("you need to set_students\n");
		return;
	}

	struct LinkedList* node = students;
	int len;
	struct Student* students_array = linked2points(node, sizeof(struct Student), &len);

	//버블 소트
	for (int i = len-1; i > 0; i--) {
		for (int j = 0; j < i; j++) {
			int compare = sort_compare_by_option(&students_array[j], &students_array[j + 1], option, args_length, args);
			if (compare == 1) {
				struct Student temp;
				temp = students_array[j];
				students_array[j] = students_array[j + 1];
				students_array[j + 1] = temp;
			}
		}
	}

	delete_linked(students);
	students = points2linked(students_array, sizeof(struct Student), len);
}

int str_euc_compare(char* big, char* small) {
	int b_count = 0, s_count = 0;
	int b_num = 0, s_num = 0;


	while(big[b_count]  && small[s_count]){
		if (big[b_count]>>7 == 0) {
			b_num = (unsigned char)big[b_count];
			b_num << 8;
			b_num += (unsigned char)big[b_count + 1];
			b_count += 2;
		}
		else {
			b_num = (unsigned char)big[b_count];
			b_count += 1;
		}

		if (small[s_count] >> 7 == 0) {
			s_num = (unsigned char)small[s_count];
			s_num << 8;
			s_num += (unsigned char)small[s_count + 1];
			s_count += 2;
		}
		else {
			s_num = (unsigned char)small[s_count];
			s_count += 1;
		}

		if (b_num > s_num) {
			return 1;
		}
		else if (b_num < s_num) {
			return -1;
		}
	}

	return 0;
}


void save_by_file_path(char* path) {
	int subjects_length;
	char** subjects_array = linked2points(subjects, sizeof(char*), &subjects_length);
	int students_length;
	struct Student* students_array = linked2points(students, sizeof(struct Student), &students_length);

	FILE* f;
	fopen_s(&f, path, "w");

	fprintf_s(f,"subjects_length=%d\n",subjects_length);

	for (int i = 0; i < subjects_length; i++) {
		fprintf_s(f,"subject_name=%s\n", subjects_array[i]);
	}

	fprintf_s(f,"students_length=%d\n", students_length);

	for (int i = 0; i < students_length; i++) {
		fprintf_s(f,"student_name=%s\n", students_array[i].name);
		fprintf_s(f,"student_id=%d\n", students_array[i].id);
		for (int j = 0; j < subjects_length; j++) {
			fprintf_s(f,"student_%s=%d\n", subjects_array[j], students_array[i].grades[j].grade);
		}
	}
	fclose(f);
}

void load_by_file_path(char* path) {
	FILE* f;
	fopen_s(&f, path, "r");
	int subjects_length;
	fscanf_s(f, "subjects_length=%d\n", &subjects_length);
	char** subjects_array = malloc(sizeof(char*) * subjects_length);

	for (int i = 0; i < subjects_length; i++) {
		char name_temp[256];
		char* name;
		fscanf_s(f,"subject_name=%s\n", name_temp, 255);
		name = malloc(sizeof(char) * (get_str_size(name_temp) + 1));
		str_copy(name, name_temp);
		subjects_array[i] = name;
	}

	delete_linked(subjects);
	subjects = points2linked(subjects_array, sizeof(char*), subjects_length);


	int students_length;
	fscanf_s(f, "students_length=%d\n", &students_length);
	struct Student* students_array = malloc(sizeof(struct Student) * students_length);

	for (int i = 0; i < students_length; i++) {
		char name_temp[256];
		char* name;
		fscanf_s(f,"student_name=%s\n", name_temp, 255);
		name = malloc(sizeof(char) * (get_str_size(name_temp) + 1));
		str_copy(name, name_temp);
		students_array[i].name = name;

		int id;
		fscanf_s(f,"student_id=%d\n", &id);
		students_array[i].id = id;

		students_array[i].grades = malloc(sizeof(struct Student) * subjects_length);
		for (int j = 0; j < subjects_length; j++) {
			int grade;
			char* scan_format_front = create_merged_str("student_", subjects_array[j]);
			char* scan_format = create_merged_str(scan_format_front, "=%d\n");
			fscanf_s(f,scan_format, &grade);
			free(scan_format_front);
			free(scan_format);

			students_array[i].grades[j].grade = grade;
		}
	}

	delete_linked(students);
	students = points2linked(students_array, sizeof(struct Student), students_length);

	free(subjects_array);
	free(students_array);

	fclose(f);
}

char* create_merged_str(char* front, char* back) {
	int front_len = get_str_size(front);
	int back_len = get_str_size(back);
	char* result = malloc(sizeof(char) * (1 + front_len + back_len));
	for (int i = 0; i < front_len; i++) {
		result[i] = front[i];
	}
	for (int i = 0; i < back_len; i++) {
		result[i+front_len] = back[i];
	}
	result[front_len + back_len] = 0;

	return result;
}

char** split_str_by_char(char* str, char sp,int* length) {
	char** result;
	struct LinkedList* result_linked=0;

	int str_length = get_str_size(str);
	struct LinkedList* split_spots = malloc(sizeof(struct LinkedList));
	memset(split_spots, 0, sizeof(struct LinkedList));
	split_spots->data = malloc(sizeof(int));
	*(int*)split_spots->data = 0;

	struct LinkedList* tail=split_spots;
	
	for (int i = 0; i < str_length + 1;i++) {
		if (str[i] == sp) {
			for (int j = 0; j < 2;j++) {
				tail->next = malloc(sizeof(struct LinkedList));
				memset(tail->next, 0, sizeof(struct LinkedList));
				tail->next->data = malloc(sizeof(int));
				*((int*)tail->next->data) = i+j;
				tail = tail->next;
			}
		}
	}
	tail->next = malloc(sizeof(struct LinkedList));
	memset(tail->next, 0, sizeof(struct LinkedList));
	tail->next->data = malloc(sizeof(int));
	*((int*)(tail->next->data)) = str_length+1;
	tail = tail->next;

	int* split_spots_array;
	int split_spots_length;

	split_spots_array = linked2points(split_spots, sizeof(int), &split_spots_length);

	delete_linked(split_spots);

	result = malloc(sizeof(char*) * split_spots_length / 2);

	for (int i = 0; i < split_spots_length-1; i+=2) {
		int byte_size = sizeof(char) * split_spots_array[i + 1] - split_spots_array[i] + 1;
		result[i/2] = malloc(byte_size);
		memset(result[i/2], '\0', byte_size);
		
		for (int j = split_spots_array[i]; j < split_spots_array[i + 1]; j++) {
			result[i/2][j - split_spots_array[i]] = str[j];
		}
	}

	*length = split_spots_length/2;
	return result;
}

int str2int(char* str) {
	int result = 0;
	int count = 0;
	while (str[count]) {
		result += (str[count] - 48) * (count + 1);
		count++;
	}
	return result;
}

int isInt(char* str) {
	int count = 0;
	while (str[count]) {
		if ((str[count] - 48 >= 0 && str[count] - 48 <= 9) || str[count] == '-' || str[count] == '.') {

		}
		else {
			return 0;
		}
		count++;
	}
	return 1;
}

int is_sort_arg_right(int option,void* args,int args_length) {
	int subjects_length = length_linked(subjects);
	switch (option) {
	case COMPARE_NAME:
		return args_length == 0;
		break;
	case COMPARE_ID:
		return args_length == 0;
		break;
	case COMPARE_GRADE:
		return args_length == 1 && ((int*)args)[0] < subjects_length && ((int*)args)[0] >= 0;
		break;
	case COMPARE_GRADE_AVARAGE:
		if (args_length >= 2) {
			for (int i = 0; i < args_length; i++) {
				if (((int*)args)[i] < subjects_length && ((int*)args)[i] >= 0) {

				}
				else {
					return 0;
				}
			}
			return 1;
		}
		return 0;
		break;
	}
}

void sort_students(char** command_splits,int command_splits_length) {
	if (!subjects) {
		printf("you need to set_subjects\n");
		return;
	}
	else if (!students) {
		printf("you need to set_students\n");
		return;
	}


	if (command_splits_length == 2 || str2int(command_splits[2]) > 3) {
		printf("you didn't input right way.\n");
		printf("[example]\n");
		printf("sort students [sort rule code] arg0 arg1 arg2 ...  [reverse = r]\n");
		printf("[codes]\n");
		printf("[0]by name\n");
		printf("[1]by id\n");
		printf("[2]by grade\n");
		printf("[3]by avarage\n");
		printf("args is the code for each subject to be calculated.\n");
		return;
	}
	int code = str2int(command_splits[2]);
	int isReverse = 0;
	int args_length = 0;
	void* args = 0;
	struct LinkedList* args_linked = 0;
	struct LinkedList* tail = 0;

	for (int i = 3; i < command_splits_length; i++) {
		if (isInt(command_splits[i])) {
			struct LinkedList* arg = malloc(sizeof(struct LinkedList));
			memset(arg, 0, sizeof(struct LinkedList));
			if (args_linked == 0) {
				args_linked = arg;
				tail = args_linked;
			}
			else {
				tail->next = arg;
				tail = tail->next;
			}
			tail->data = malloc(sizeof(int));
			*(int*)tail->data = str2int(command_splits[i]);
		}
		else if (same_string(command_splits[i], "r") || same_string(command_splits[i], "reverse")) {
			isReverse = 1;
		}
		else {
			printf("[error] worng args => \"%s\".\n", command_splits[i]);
			delete_linked(args_linked);
			return;
		}
	}

	if (args_linked == 0) {
		args = 0;
	}
	else {
		args = linked2points(args_linked, sizeof(int), &args_length);
		delete_linked(args_linked);
	}

	if (is_sort_arg_right(code, args, args_length)) {
		sort_students_optoin(code, args_length, args);
	}
	else {
		printf("[error]\n");
		printf("a number of args is wrong or subject code is wrong\n");
	}
	
	free(args);
}
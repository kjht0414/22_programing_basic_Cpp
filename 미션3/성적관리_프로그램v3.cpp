#include<iostream>
#include<vector>
#include<conio.h>
#include<Windows.h>
#include<sstream>
#include<map>
#include<algorithm>
#include<fstream>

using namespace std;


#define SPECIAL_KEY 0xE0
#define FUNCTION_KEY 0x00
#define BACK_SPACE_KEY 8
#define DELETE_KEY 83
#define ENTER_KEY 13
#define RIGHT_KEY 77
#define LEFT_KEY 75
#define UP_KEY 72
#define DOWN_KEY 80

#define TAB_KEY '	'

/*
* 성적관리 프로그램 v3
* 추가된 부분:
*  1.자동완성 기능. !!!---Tab 키와 화살표, 엔터키로 작동---!!!
*  2.Grade 클래스를 통한 계산 되는 부분 => 평균 을 추가.
*  3.명령어 뒤에 인자를 넣도록 함.
* 
* 변경점:
*  1.명령어 앞에 '/' 제거.
*  2.Command 클래스에 함수포인터를 통하여 명령어가 작동하도록 변경 --> 메인함수 단순화 and 캡슐화?
*  3.append 를 add 로 변경
*  
*/






class Util {
public:
	static vector<string> split(string src, string target) {
		int split_spot;
		vector<string> result;

		while (true) {
			split_spot = src.find(target);
			if (split_spot == string::npos)
				break;
			result.push_back(src.substr(0, split_spot));
			src = src.substr(split_spot+target.length());
		}
		result.push_back(src);

		return result;
	}

	static string join(vector<string> list, string between) {
		string result;
		for (int i = 0; i < list.size(); i++) {
			result += list.at(i);
			if (i + 1 < list.size()) {
				result += between;
			}
		}
		return result;
	}

	static COORD getConsoleCursorPosition()
	{
		COORD Cur;
		CONSOLE_SCREEN_BUFFER_INFO a;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &a);
		Cur.X = a.dwCursorPosition.X;
		Cur.Y = a.dwCursorPosition.Y;
		return Cur;
	}

	static void setConsoleCursorPosition(int x, int y) {
		COORD Pos;
		Pos.X = x;
		Pos.Y = y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
	}

	static bool is_integer(const string& s) //양수에만 작동함
	{
		int i = 0;
		stringstream ssint(s);
		ssint >> i;

		if (ssint.fail())
			return false;
		return true;
	}

	static int to_integer(const string& s) //양수에만 작동함
	{
		int i = 0;
		stringstream ssint(s);
		ssint >> i;

		if (ssint.fail())
			throw invalid_argument("\""+ s + "\" is not integer");
		return i;
	}
};

class Command {
private:
	static vector<Command*> commands;
protected:
	string name;
	vector<Command*> children;
	Command* parent;
	vector<string> (*auto_args_com)(vector<string>, vector<void*>);

	void (*cmd_fun)(vector<string>, vector<void*>);

	Command(string name, void (*cmd_fun)(vector<string>, vector<void*>), vector<string> (*auto_args_com)(vector<string>, vector<void*>),Command* parent) {
		this->name = name;
		this->cmd_fun = cmd_fun;
		this->auto_args_com = auto_args_com;
		this->parent = parent;
	}
public:
	
	Command(string name) {
		this->name = name;
		this->cmd_fun = 0;
		this->auto_args_com = 0;
		this->commands.push_back(this);
	}

	Command(string name, void (*cmd_fun)(vector<string>, vector<void*>), vector<string> (*auto_args_com)(vector<string>, vector<void*>)) {
		this->name = name;
		this->cmd_fun = cmd_fun;
		this->auto_args_com = auto_args_com;
		this->commands.push_back(this);
	}

	Command* commandNext(string name) {
		Command* child = new Command(name, 0, 0, this);
		this->children.push_back(child);
		return child;
	}

	Command* commandNext(string name, void (*cmd_fun)(vector<string>, vector<void*>), vector<string> (*auto_args_com)(vector<string>, vector<void*>)) {
		Command* child = new Command(name, cmd_fun, auto_args_com, this);
		this->children.push_back(child);
		return child;
	}

	string getName() {
		return name;
	}

	vector<Command*> getChildren() {
		return children;
	}

	static vector<Command*> getCommands() {
		return Command::commands;
	}

	static void act_cmd_fun(string cmd_line, vector<void*> outside_args) {
		vector<string> command_elements = Util::split(cmd_line, " ");
		Command* command_tail = 0;
		int elements_index=0;
		for (string command_element : command_elements) {
			if (command_tail != 0) {
				for (Command* cmd : command_tail->getChildren()) {
					if (cmd->getName() == command_element) {
						command_tail = cmd;
						elements_index++;
					}
				}
			}
			else {
				for (Command* cmd : Command::getCommands()) {
					if (cmd->getName() == command_element) {
						command_tail = cmd;
						elements_index++;
					}
				}
			}
		}
		if (command_tail!=0 and command_tail->cmd_fun != 0) {	
			vector<string> args;
			for (int i = elements_index; i < command_elements.size(); i++) {
				if (command_elements.at(elements_index).size() > 0) {
					args.push_back(command_elements.at(i));
				}
			}
			command_tail->cmd_fun(args, outside_args);
		}
	}

	int isArgsCompletable() {
		if (cmd_fun==0 or auto_args_com == 0)
			return false;
		return true;
	}

	vector<string> argsCom(vector<string> args, vector<void*> outside_args) {
		return auto_args_com(args , outside_args);
	}
};
vector<Command*> Command::commands;
/*
* 탭은 자동완성
*/
class CommandLine {
private:
	string line;
	vector<string> auto_completion_lines;
	int cursor = 0;
	int prev_line_size = 0;
	int auto_com_cursor;
	int cursor_height = 0;
	int auto_completion_pos = -1;
	
public:
	string getCommand(vector<void*> outside_args) {
		line.clear();
		auto_completion_lines.clear();
		cursor = 0;
		prev_line_size = 0;
		cursor_height = Util::getConsoleCursorPosition().Y;
		moveCursor(0, cursor_height);
		while (true) {
			unsigned char c = _getch();
			unsigned int sp=0;

			//cout << (unsigned int)c;
			
			if (c == ENTER_KEY) {
				if (auto_completion_pos == -1) {
					clear();
					clear_auto_com_print();
					moveCursor(0, cursor_height);
					cout << line << endl;
					break;
				}
				else {
					while (line.size() > 0 and cursor - 1 >= 0 and line.at(cursor-1) != ' ') {
						line.erase(cursor - 1);
						cursor--;
					}
					for (char ac : auto_completion_lines.at(auto_completion_pos)) {
						line.insert(cursor, 1, ac);
						cursor++;
					}
					if (cursor < line.size() and line.at(cursor) != ' ') {
						line.insert(cursor, 1, ' ');
						cursor++;
					}

					clear_auto_com_print();

					auto_completion_pos = -1;
					auto_completion_lines.clear();
				}
			}
			else if (c == TAB_KEY) {
				//자동완성 and 빠져나오기
				if (auto_completion_pos != -1) {
					auto_completion_pos = -1;
				}
				else if(auto_completion_lines.size() == 0) {
					search_auto_com(outside_args);
					if (auto_completion_lines.size() == 0 and cursor - 1 < line.size() and line.at(cursor-1)!=' ') {
						line.insert(cursor, 1, ' ');
						cursor++;
					}
				}
			}
			else if (c == SPECIAL_KEY or c == FUNCTION_KEY) {
				sp = _getch();
				if (sp == RIGHT_KEY) {
					if (cursor < line.size()) {
						cursor++;
						update_auto_com(outside_args);
					}
				}
				else if (sp == LEFT_KEY) {
					if (cursor > 0) {
						cursor--;
						update_auto_com(outside_args);
					}	
				}
				else if (sp == UP_KEY and auto_completion_lines.size()>0) {
					auto_completion_pos--;
					if (auto_completion_pos < 0) {
						auto_completion_pos = auto_completion_lines.size()-1;
					}
				}
				else if (sp == DOWN_KEY and auto_completion_lines.size() > 0) {
					auto_completion_pos++;
					if (auto_completion_pos > auto_completion_lines.size() - 1) {
						auto_completion_pos = 0;
					}
				}
				else if (sp == DELETE_KEY) {
					if (line.size() >= 1 and cursor < line.size()) {
						line.erase(cursor,1);
						update_auto_com(outside_args);
					}
				}
			}
			else if (c == BACK_SPACE_KEY) {
				if (line.size() >= 1 and cursor > 0 and cursor <= line.size()) {
					line.erase(cursor-1, 1);
					cursor--;
					update_auto_com(outside_args);
				}
			}
			else {
				line.insert(cursor,1,c);
				cursor++;
				update_auto_com(outside_args);
			}

			
			//system("cls");
			clear();
			moveCursor(0, this->cursor_height);
			cout << this->line;
			clear_auto_com_print();
			autoCompletionPrint();

			moveCursor();

			prev_line_size = line.size();
		}
		return this->line;
	}

	void moveCursor() {
		Util::setConsoleCursorPosition(cursor, cursor_height);
	}

	void moveCursor(int x, int y) {
		Util::setConsoleCursorPosition(x, y);
	}

	void clear() {
		moveCursor(0, cursor_height);
		cout << getBlink(prev_line_size);
		moveCursor();
	}

	void clear_auto_com_print() {
		for (int i = 0; i < auto_completion_lines.size(); i++) {
			moveCursor(auto_com_cursor, cursor_height + i + 1);
			cout << getBlink(auto_completion_lines.at(i).length() + 1);
		}
	}

	void update_auto_com(vector<void*> outside_args) {
		if (auto_completion_lines.size() > 0) {
			clear_auto_com_print();
			search_auto_com(outside_args);
			auto_completion_pos = -1;
		}
	}

	string getBlink(int length) {
		string s = "";
		for (int i = 0; i < length; i++) {
			s += " ";
		}
		return s;
	}

	void autoCompletionPrint() {
		vector<string> command_elements = Util::split(line," ");
		auto_com_cursor = 0;
		for (int i = 0; i < command_elements.size()-1;i++) {
			auto_com_cursor += command_elements.at(i).length();
		}
		auto_com_cursor += command_elements.size() - 1;
		for (int i = 0; i < auto_completion_lines.size(); i++) 
		{
			moveCursor(auto_com_cursor, cursor_height + i + 1);
			if (auto_completion_pos == i) {
				cout << ">";
			}
			else {
				cout << "";
			}
			cout << auto_completion_lines.at(i) << " ";
		}
	}

	void autoCompletion() {
		auto_completion_lines.push_back("test");
	}

	void search_auto_com(vector<void*> outside_args) {
		auto_completion_lines.clear();
		vector<string> command_elements = Util::split(line, " ");
		Command* command_tail=0;
		int command_tail_index = -1;
		int cursor_buf = cursor;
		int cursored_element_index = 0;
		string cursored_element = "";
		for (int index = 0; index < command_elements.size(); index++) {
			string command_element = command_elements[index];
			cursor_buf -= command_element.length();
			
			if (index + 1 < command_elements.size())
				cursor_buf--;
			
			if (cursor_buf <= 0) {
				cursored_element = command_element;
				break;
			}

			cursored_element_index++;
		}


		for (int index = 0; index <= cursored_element_index; index++) {
			string command_element = command_elements[index];
			for (Command* cmd : (command_tail != 0 ? command_tail->getChildren() : Command::getCommands())) {
				if (cmd->getName() == command_element) {
					command_tail = cmd;
					command_tail_index = index;
				}
			}
		}

		for (Command* cmd : (command_tail != 0 ? command_tail->getChildren() : Command::getCommands())) {
			string cmd_name = cmd->getName();
			if ((cursored_element_index + 1 < command_elements.size() and command_elements.at(cursored_element_index+1) == "") or cmd_name.find(cursored_element) != -1 or cursor_buf < 0) {
				auto_completion_lines.push_back(cmd_name);
			}
		}

		if (command_tail != 0 and command_tail->isArgsCompletable()) {
			vector<string> args;
			for (int i = command_tail_index + 1; i <= cursored_element_index; i++) {
				if (command_elements.at(i).size() > 0 and command_elements.at(i) != command_tail->getName())
					args.push_back(command_elements.at(i));
			}
			vector<string> args_auto = command_tail->argsCom(args, outside_args);
			for (string arg_auto : args_auto) {
				if ((cursored_element_index + 1 < command_elements.size() and command_elements.at(cursored_element_index + 1) == "") or arg_auto.find(cursored_element) != -1 or cursor_buf < 0) {
					auto_completion_lines.push_back(arg_auto);
				}
			}
		}
	}
};

class Grade {
protected:
	int grade;
public:
	void setGrade(int grade) {
		this->grade = grade;
	}

	int getGrade() {
		return grade;
	}
};

class Student {
private:
	string name;
	int id;
	map<int, Grade*> grades;

	static vector<Student*> all_student;
public:

	Student(string name) {
		setName(name);
		setId(getUseableId());
		all_student.push_back(this);
	}

	Student(int id,string name) {
		setName(name);
		setId(id);
		all_student.push_back(this);
	}

	~Student() {
		int index = 0;
		for (Student* stud : all_student) {
			if (stud->getId() == id) {
				break;
			}
			index++;
		}

		all_student.erase(all_student.begin() + index);
	}

	void setName(string name) {
		this->name = name;
	}

	string getName() {
		return name;
	}

	void setId(int id) {
		for (Student* stud : all_student) {
			if (id == stud->getId()) {
				throw invalid_argument("Student(" + stud->getName() + ") already has id:" + to_string(id) + ".");
			}
		}

		this->id = id;
		for (Student* stud : all_student) {
			if (stud == this) {
				sort(all_student.begin(), all_student.end(), compareByID);
				break;
			}
		}
	}

	int getId() {
		return id;
	}

	Grade* getGrade(int subject_code) {
		map<int, Grade*>::iterator grade_pair;
		if ((grade_pair = grades.find(subject_code)) != grades.end()) {
			return grade_pair->second;
		}
		else {
			return 0;
		}
	}
	
	void setGrade(int id, Grade* grade) {
		grades[id] = grade;
	}

	static int getUseableId() {
		int id = 0;
		for (Student* stud : all_student) {
			int stud_id = stud->getId();
			id++;
			if (stud_id > id - 1) {
				break;
			}
		}

		return id;
	}

	static bool compareByID(Student* a, Student* b) {
		return a->getId() < b->getId();
	}

	static bool compareByName(Student* a, Student* b) {
		return a->getId() < b->getId();//TODO:remake
	}

	
	static bool compareBySubject(Student* a, Student* b, vector<int> subject_codes) {
		return a->getId() < b->getId();
	}
};
vector<Student*> Student::all_student;

class Subject {
private:
	int code;
	string name;
	static vector<Subject*> all_subject;
protected:
	bool calculating = false;
public:
	
	Subject(int code, string name) {
		setCode(code);
		setName(name);

		all_subject.push_back(this);
	}

	Subject(string name) {
		setCode(getUseableCode());
		setName(name);

		all_subject.push_back(this);
	}

	~Subject() {
		int index = 0;
		for (Subject* sbj : all_subject) {
			if (sbj->getCode() == code) {
				break;
			}
			index++;
		}

		all_subject.erase(all_subject.begin() + index);
	}

	inline bool operator< (Subject& sub) {
		return this->getCode() < sub.getCode();
	}

	void setCode(int code) {
		for (Subject* sbj : all_subject) {
			if (code == sbj->getCode()) {
				throw invalid_argument("Subject(" + sbj->getName() + ") already has code:" + to_string(code) + ".");
			}
		}

		this->code = code;
		for (Subject* sbj : all_subject) {
			if (sbj == this) {
				sort(all_subject.begin(), all_subject.end());
				break;
			}
		}
	}

	int getCode() {
		return code;
	}

	void setName(string name) {
		this->name = name;
	}

	string getName() {
		return name;
	}

	static int getUseableCode() {
		int code = 0;
		for (Subject* sbj : all_subject) {
			int sbj_code = sbj->getCode();
			code++;
			if (sbj_code > code - 1) {
				break;
			}
		}

		return code;
	}

	static Subject* getSubjectByCodeFromVec(vector<Subject*> list, int code) {
		for (Subject* sub : list) {
			if (sub->getCode() == code) {
				return sub;
			}
		}
		return 0;
	}

	static Subject* getSubjectByNameFromVec(vector<Subject*> list, string name) {
		for (Subject* sub : list) {
			if (sub->getName() == name) {
				return sub;
			}
		}
		return 0;
	}

	bool isCalculating() {
		return calculating;
	}
};

class CalculatedSubject : Subject {
private:
	vector<int> calculated_subject_codes;
	int (*calculating_method) (vector<Subject*>, Student*, vector<int>);
public:
	CalculatedSubject(int code, string name, vector<int> calculated_subject_codes) : Subject(code, name) {
		this->calculated_subject_codes = calculated_subject_codes;
		this->calculating = true;
	}

	CalculatedSubject(string name, vector<int> calculated_subject_codes) : Subject(name){
		this->calculated_subject_codes = calculated_subject_codes;
		this->calculating = true;
	}

	int (*getCalculatingMethod()) (vector<Subject*>, Student*, vector<int>) {
		return calculating_method;
	}

	vector<int> getCalculatedSubjectCodes() {
		return calculated_subject_codes;
	}

	void setCalculatingMethod(int (*calculating_method) (vector<Subject*>, Student*, vector<int>)) {
		this->calculating_method = calculating_method;
	}

	bool isCalculating() {
		return true;
	}
};

class CalculatedGrade : Grade {
protected:
	int (*calculating_method) (vector<Subject*>, Student*, vector<int>);
public:
	void setCalculatingMethod(int (*calculating_method) (vector<Subject*>, Student*, vector<int>)) {
		this->calculating_method = calculating_method;
	}

	int getGrade(vector<Subject*> subject_set, Student* student, vector<int> subject_code_list) {
		if (calculating_method == 0) {
			return 0;
		}
		else {
			return calculating_method(subject_set, student, subject_code_list);
		}
	}
};

vector<Subject*> Subject::all_subject;

void creat_grade_by_student(Student* stud, Subject* sub) {
	Grade* grade = 0;
	if (sub->isCalculating()) {
		grade = (Grade*)new CalculatedGrade();
		int (*calculating_method)(vector<Subject*>, Student*, vector<int>) = ((CalculatedSubject*)sub)->getCalculatingMethod();
		((CalculatedGrade*)grade)->setCalculatingMethod(calculating_method);
	}
	else {
		grade = new Grade();
		grade->setGrade(0);
	}
	stud->setGrade(sub->getCode(), grade);
}

void cmd_say(vector<string> args, vector<void*> outside_args) {
	for (string s : args) {
		cout << s << endl;
	}
}

vector<string> cmd_say_args(vector<string> args, vector<void*> outside_args) {
	vector<string> result;
	if (args.size() == 0) {
		result.push_back("hello");
	}
	else {
		vector<string> _spt = Util::split(args.at(args.size() - 1), "_");
		int final_dash_num = -1;
		if (_spt.size() > 1) {
			string final_dash = _spt.at(_spt.size() - 1);
			if (Util::is_integer(final_dash)) {
				final_dash_num  = Util::to_integer(final_dash);
			}
		}
		else {
			_spt.push_back("");
		}
		_spt.at(_spt.size() - 1) = to_string(final_dash_num + 1);
		result.push_back(Util::join(_spt, "_"));
	}
	
	return result;
}

void cmd_set_subjects(vector<string> args, vector<void*> outside_args) {
	vector<Subject*>* subject_set = static_cast<vector<Subject*>*>(outside_args[0]);
	vector<Student*>* student_set = static_cast<vector<Student*>*>(outside_args[1]);

	cout << "과목 목록 초기화 중..." << endl;
	for (int i = 0; i < subject_set->size(); i++) {
		Subject* sub = (*subject_set)[subject_set->size() - 1];

		for (Student* stud : *student_set) {
			Grade* grade = stud->getGrade(sub->getCode());
			if (grade != 0) {
				stud->setGrade(sub->getCode(), 0);
				delete grade;
			}
		}
		subject_set->pop_back();
		cout << "[" << sub->getCode() << "]" << sub->getName() << " 삭제." << endl;
		delete sub;
	}
	cout << "과목 목록 초기화 완료." << endl;

	if (args.size() > 0) {
		cout << "과목 목록 재설정 중..." << endl;
		for (string s : args) {
			subject_set->push_back(new Subject(s));
		}
		cout << "과목 목록 재설정 완료." << endl;
	}
	else {
		string in_buf;
		int count;
		while (true) {
			cout << "지정할 과목들의 수: ";
			cin >> in_buf;
			if (Util::is_integer(in_buf)) {
				count = Util::to_integer(in_buf);
				break;
			}
			else {
				cout << "입력이 잘못되었습니다." << endl;
			}
		}

		for (int i = 0; i < count; i++) {
			string name_buf;
			cout << "[" << i + 1 << "/" << count << "]번째 과목의 이름: ";
			cin >> name_buf;
			Subject* sub = new Subject(name_buf);
			subject_set->push_back(sub);
		}
	}

	for (Subject* sub : *subject_set) {
		for (Student* stud : *student_set) {
			creat_grade_by_student(stud, sub);
		}
	}
}

vector<string> cmd_set_subjects_args(vector<string> args, vector<void*> outside_args) {
	vector<string> subjects = { "국어","영어","수학","사회","과학","역사" };
	for (int i = 0; i < args.size(); i++) {
		string arg = args.at(i);
		for (int j = 0; j < subjects.size(); j++) {
			string subject = subjects.at(j);
			if (arg == subject) {
				subjects.erase(subjects.begin()+j);
				break;
			}
		}
	}
	if (subjects.size() > 0) {
		return subjects;
	}
	else {
		return {};
	}
}

void cmd_subjects(vector<string> args, vector<void*> outside_args) {
	vector<Subject*>* subject_set = static_cast<vector<Subject*>*>(outside_args[0]);
	vector<Student*>* student_set = static_cast<vector<Student*>*>(outside_args[1]);

	if (subject_set->empty()) {
		cout << "과목 목록이 비어있습니다." << endl;
		cout << "명령어 \'set subjects\'를 통해 과목 목록을 설정하세요." << endl;
	}


	for (Subject* sub : *subject_set) {
		cout << "[" << sub->getCode() << "]" << sub->getName() << endl;
	}
}

vector<string> cmd_subjects_args(vector<string> args, vector<void*> outside_args) {
	vector<string> result;
	return result;
}

void cmd_modify_subject(vector<string> args, vector<void*> outside_args) {
	vector<Subject*>* subject_set = static_cast<vector<Subject*>*>(outside_args[0]);
	vector<Student*>* student_set = static_cast<vector<Student*>*>(outside_args[1]);

	if (args.size() <= 2) {
		cout << "잘못 입력 되었습니다." << endl;
		cout << "(예시 0) modify subject name 바꿀과목 과목이름" << endl;
		cout << "(예시 1) modify subject code 바꿀과목 과목ID" << endl;
		return;
	}

	string mode = args[0];
	string target = args[1];
	string modify = args[2];

	Subject* sub = 0;
	if (Util::is_integer(target)) {
		sub = Subject::getSubjectByCodeFromVec(*subject_set, Util::to_integer(target));
	}
	else {
		sub = Subject::getSubjectByNameFromVec(*subject_set, target);
	}

	if (sub == 0) {
		cout << "\'" << target << "\'은 존재하지 않는 과목입니다." << endl;
		return;
	}

	if (mode == "name") {
		sub->setName(modify);
	}
	else if(mode == "code") {
		if (Util::is_integer(target)) {
			int prev_code = sub->getCode();
			sub->setCode(Util::to_integer(modify));
			for (Student* stud : *student_set) {
				Grade* grade = stud->getGrade(prev_code);
				stud->setGrade(prev_code, 0);
				stud->setGrade(sub->getCode(), grade);
			}
		}
		else {
			cout << modify << "은 정수가 아닙니다." << endl;
			return;
		}
	}
}

bool isArgsAutoComed(vector<string> args, int index, vector<string> auto_com_list) {
	if (args.size() == index) {
		return false;
	}
	else if (args.size() == index + 1) {
		for (string s : auto_com_list) {
			if (args[index] != s and s.find(args[index]) != -1) {
				return false;
			}
		}
	}
	return true;
}

vector<string> cmd_modify_subject_args(vector<string> args, vector<void*> outside_args) {
	vector<Subject*>* subject_set = static_cast<vector<Subject*>*>(outside_args[0]);
	vector<Student*>* student_set = static_cast<vector<Student*>*>(outside_args[1]);

	//vector<string> result;

	if (!isArgsAutoComed(args, 0, { "name","code" })) {
		return { "name","code" };
	}

	vector<string> subjects_names;
	for (Subject* sub : *subject_set) {
		subjects_names.push_back(sub->getName());
	}

	if (!isArgsAutoComed(args, 1, subjects_names)) {
		return subjects_names;
	}
	else if (!isArgsAutoComed(args, 2, { "과목이름" })) {
		return { "과목이름" };
	}
	return {"NULL"};
}

void cmd_add_subjects(vector<string> args, vector<void*> outside_args) {
	vector<Subject*>* subject_set = static_cast<vector<Subject*>*>(outside_args[0]);
	vector<Student*>* student_set = static_cast<vector<Student*>*>(outside_args[1]);

	if (args.size() <= 0) {
		cout << "잘못 입력 되었습니다." << endl;
		cout << "(예시) add subjects 과목이름_1 과목이름_2 과목이름_3 ..." << endl;

		return;
	}
	else {
		for (string s : args) {
			Subject* sub = new Subject(s);
			subject_set->push_back(sub);
			for (Student* stud : *student_set) {
				creat_grade_by_student(stud, sub);
			}
		}
		cout << "추가 완료." << endl;
	}
}

void add_students(vector<string> args, vector<void*> outside_args) {
	vector<Subject*>* subject_set = static_cast<vector<Subject*>*>(outside_args[0]);
	vector<Student*>* student_set = static_cast<vector<Student*>*>(outside_args[1]);

	if (args.size() <= 0) {
		cout << "잘못 입력 되었습니다." << endl;
		cout << "(예시) add students 학생이름_1 학생이름_2 학생이름_3 ..." << endl;
		return;
	}
	else {
		for (string s : args) {
			Student* stud = new Student(s);
			student_set->push_back(stud);
			for (Subject* sub : *subject_set) {
				creat_grade_by_student(stud, sub);
			}
		}
		cout << "추가 완료." << endl;
	}
}

vector<string> cmd_add_subjects_args(vector<string> args, vector<void*> outside_args) {
	vector<Subject*>* subject_set = static_cast<vector<Subject*>*>(outside_args[0]);
	vector<Student*>* student_set = static_cast<vector<Student*>*>(outside_args[1]);

	vector<string> subjects = { "국어","영어","수학","사회","과학","역사" };

	for (int i = 0; i < subject_set->size(); i++) {
		Subject* subject = subject_set->at(i);
		for (int j = 0; j < subjects.size(); j++) {
			string sub = subjects.at(j);
			if (sub == subject->getName()) {
				subjects.erase(subjects.begin() + j);
				break;
			}
		}
	}

	for (int i = 0; i < args.size(); i++) {
		string arg = args.at(i);
		for (int j = 0; j < subjects.size(); j++) {
			string sub = subjects.at(j);
			if (sub == arg) {
				subjects.erase(subjects.begin() + j);
				break;
			}
		}
	}

	if (subjects.size() > 0) {
		return subjects;
	}
	else {
		return { "과목이름" };
	}
}


vector<string> cmd_add_students_args(vector<string> args, vector<void*> outside_args) {
	vector<Subject*>* subject_set = static_cast<vector<Subject*>*>(outside_args[0]);
	vector<Student*>* student_set = static_cast<vector<Student*>*>(outside_args[1]);

	return { "학생이름" };
}


void cmd_set_students(vector<string> args, vector<void*> outside_args) {
	vector<Subject*>* subject_set = static_cast<vector<Subject*>*>(outside_args[0]);
	vector<Student*>* student_set = static_cast<vector<Student*>*>(outside_args[1]);

	cout << "학생 목록 초기화 중..." << endl;
	for (int i = 0; i < student_set->size(); i++) {
		Student* student = (*student_set)[student_set->size() - 1];
		student_set->pop_back();
		delete student;
	}
	cout << "학생 목록 초기화 완료." << endl;

	if (args.size() > 0) {
		for (string arg : args) {
			Student* stud = new Student(arg);
			student_set->push_back(stud);
			for (Subject* sub : *subject_set) {
				creat_grade_by_student(stud, sub);
			}
		}
	}
	else {
		string name_buf;
		while (true) {
			getline(cin, name_buf);
			if (name_buf.size() <= 0) {
				break;
			}
			else {
				student_set->push_back(new Student(name_buf));
			}
		}
	}
}

vector<string> cmd_set_students_args(vector<string> args, vector<void*> outside_args) {
	return { "학생이름" };
}

void cmd_students(vector<string> args, vector<void*> outside_args) {
	vector<Subject*>* subject_set = static_cast<vector<Subject*>*>(outside_args[0]);
	vector<Student*>* student_set = static_cast<vector<Student*>*>(outside_args[1]);

	cout << "이름" << "\t";
	cout << "ID" << "\t";

	for (Subject* sub : *subject_set) {
		cout << sub->getName() << "\t";
	}
	cout << endl;


	for (Student* student : *student_set) {
		cout << student->getName() << "\t";
		cout << student->getId() << "\t";
		for (Subject* sub : *subject_set) {
			Grade* grade = student->getGrade(sub->getCode());
			if (grade == 0) {
				cout << "N" << "\t";
			}
			else if(!sub->isCalculating()) {
				cout << grade->getGrade() << "\t";
			}
			else {
				cout << ((CalculatedGrade*)grade)->getGrade(*subject_set,student,((CalculatedSubject*)sub)->getCalculatedSubjectCodes()) << "\t";
			}
		}
		cout << endl;
	}
}
//TODO:students args fun
void cmd_modify_student(vector<string> args, vector<void*> outside_args){
	vector<Subject*>* subject_set = static_cast<vector<Subject*>*>(outside_args[0]);
	vector<Student*>* student_set = static_cast<vector<Student*>*>(outside_args[1]);

	if (args.size() <= 2) {
		cout << "잘못 입력 되었습니다." << endl;
		cout << "(예시 0) modify student name 아이디 바꿀이름" << endl;
		cout << "(예시 1) modify student id 아이디 바꿀아이디" << endl;
		cout << "(예시 1) modify student subject 아이디 과목이름_1 점수_1 과목이름_2 점수_2 ..." << endl;
		return;
	}

	string mode = args[0];
	string target = "";
	string modify = "";
	vector<string> subject_names;
	vector<string> grade_modifies;
	target = args[1];
	if (mode == "name") {
		modify = args[2];
	}
	else if (mode == "id") {
		modify = args[2];
	}
	else if (mode == "subject") {
		for (int i = 2; i < args.size(); i += 2) {
			subject_names.push_back(args[i]);
			if (i + 1 < args.size()) {
				grade_modifies.push_back(args[i + 1]);
			}
			else {
				cout << "잘못 입력 되었습니다." << endl;
				cout << subject_names.at(subject_names.size() - 1) << "의 수정할 점수가 입력되지 않았습니다." << endl;
				return;
			}
		}
		
	}
	else {
		cout << mode << "은 존재하지 않습니다." << endl;
		return;
	}
	
	Student* stud = 0;
	if (Util::is_integer(target)) {
		for (Student* s : *student_set) {
			if (s->getId() == Util::to_integer(target)) {
				stud = s;
				break;
			}
		}

		if (stud == 0) {
			cout << "ID: " << target << "인 학생 정보가 존재하지 않습니다." << endl;
			return;
		}
		else {
			if (mode == "name") {
				stud->setName(modify);
			}
			else if (mode == "id") {
				if (Util::is_integer(modify)) {
					try {
						stud->setId(Util::to_integer(modify));

					}
					catch (invalid_argument& ia) {
						cerr << "ERROR : " << ia.what() << endl;
						return;
					}
				}
				else {
					cout << modify << "은 정수가 아닙니다." << endl;
					return;
				}
			}
			else {
				for (int i = 0; i < subject_names.size();i++) {
					string subject_name = subject_names.at(i);
					string modify = grade_modifies.at(i);
					Subject* sub = 0;
					if (Util::is_integer(subject_name)) {
						sub = Subject::getSubjectByCodeFromVec(*subject_set, Util::to_integer(subject_name));
					}
					else {
						sub = Subject::getSubjectByNameFromVec(*subject_set, subject_name);
					}

					if (sub == 0) {
						cout << "\'" << subject_name << "\'은 존재하지 않는 과목입니다." << endl;
						return;
					}

					if (Util::is_integer(modify)) {

						Grade* grade = stud->getGrade(sub->getCode());
						if (grade == 0) {
							Grade* grade = new Grade();
							grade->setGrade(Util::to_integer(modify));
							stud->setGrade(sub->getCode(), grade);
						}
						else {
							grade->setGrade(Util::to_integer(modify));
						}
					}
					else {
						cout << modify << "은 정수가 아닙니다." << endl;
						return;
					}
				}
			}
		}
	}
	else {
		cout << target << "은 정수가 아닙니다." << endl;
		return;
	}
}

vector<string> cmd_modify_student_args(vector<string> args, vector<void*> outside_args) {
	vector<Subject*>* subject_set = static_cast<vector<Subject*>*>(outside_args[0]);
	vector<Student*>* student_set = static_cast<vector<Student*>*>(outside_args[1]);

	if (!isArgsAutoComed(args, 0, { "name", "id", "subject" })) {
		return { "name", "id", "subject" };
	}
	else if (!isArgsAutoComed(args, 1, { "바꿀_학생의_아이디" })) {
		return {};
	}
	else if (args.at(0) == "name" or args.at(0) == "id") {
		if (!isArgsAutoComed(args, 2, { "바꿀_이름or아이디" })) {
			return {};
		}
	}
	else if (args.at(0) == "subject") {
		vector<string> subjects_names;
		for (Subject* sub : *subject_set) {
			subjects_names.push_back(sub->getName());
		}

		if (!isArgsAutoComed(args, 1, subjects_names)) {
			return subjects_names;
		}
		for (int i = 2; i < args.size(); i += 2) {
			if (!isArgsAutoComed(args, i, { "과목이름" })) {
				return {};//TODO:과목 리스트 구해서 넣기
			}
			else if (!isArgsAutoComed(args, i + 1, { "바꿀_이름or아이디" })) {
				return {};
			}
		}
	}


}

int calculating_method_avg(vector<Subject*> subject_set, Student* stud, vector<int> subject_code_set) {
	int avg = 0, count = 0;
	for (int code : subject_code_set) {
		Grade* grade = stud->getGrade(code);
		if (grade != 0) {
			avg += grade->getGrade();
			count++;
		}
	}

	if (count > 0) {
		avg /= count;
		return avg;
	}
	return -1;
}

void cmd_add_calculated_subjects(vector<string> args, vector<void*> outside_args) {
	vector<Subject*>* subject_set = static_cast<vector<Subject*>*>(outside_args[0]);
	vector<Student*>* student_set = static_cast<vector<Student*>*>(outside_args[1]);

	if (args.size() <= 1) {
		cout << "잘못 입력 되었습니다." << endl;
		cout << "(예시) add avg subjects 과목이름 계산될_과목이름_1 계산될_과목이름_2 ..." << endl;//TODO: 다른 계산 식에대하여 생각

		return;
	}
	else {
		string sub_name = args[0];
		vector<int> code_list;
		for (Subject* sub : *subject_set) {
			
		}
		for (int i = 1; i < args.size(); i++) {
			if (Util::is_integer(args[i])) {
				int code = Util::to_integer(args[i]);
				bool isExist = false;
				for (Subject* sub : *subject_set) {
					if (sub->getCode() == code) {
						code_list.push_back(code);
						isExist = true;
						break;
					}
				}
				if (!isExist) {
					cout << "\'" << code << "\'은 존재하지 않는 과목코드입니다." << endl;
					return;
				}
			}
			else {
				bool isExist = false;
				for (Subject* sub : *subject_set) {
					
					if (sub->getName() == args[i]) {
						code_list.push_back(sub->getCode());
						isExist = true;
						break;
					}
				}
				if (!isExist) {
					cout << "\'" << args[i] << "\'은 존재하지 않는 과목입니다." << endl;
					return;
				}
			}
		}

		if (code_list.size() > 0) {
			CalculatedSubject* c_sub = new CalculatedSubject(sub_name, code_list);
			c_sub->setCalculatingMethod(calculating_method_avg);

			subject_set->push_back((Subject*)c_sub);
			for (Student* stud : *student_set) {
				creat_grade_by_student(stud, (Subject*)c_sub);
			}
		}
		else {
			cout << "계산된 과목이 없습니다." << endl;
			return;
		}
		cout << "추가 완료." << endl;
	}
}

vector<string> cmd_add_calculated_subjects_args(vector<string> args, vector<void*> outside_args) {
	vector<Subject*>* subject_set = static_cast<vector<Subject*>*>(outside_args[0]);
	vector<Student*>* student_set = static_cast<vector<Student*>*>(outside_args[1]);

	if (!isArgsAutoComed(args, 0, { "avg" })) {
		return { "avg" };
	}
	
	vector<string> subject_name_set;

	for (Subject* sub : *subject_set) {
		subject_name_set.push_back(sub->getName());
	}

	if (!isArgsAutoComed(args, args.size(), subject_name_set)) {
		return subject_name_set;
	}
}


void cmd_delete_subjects(vector<string> args, vector<void*> outside_args) {
	vector<Subject*>* subject_set = static_cast<vector<Subject*>*>(outside_args[0]);
	vector<Student*>* student_set = static_cast<vector<Student*>*>(outside_args[1]);

	if (args.size() <= 0) {
		cout << "잘못 입력 되었습니다." << endl;
		cout << "(예시) delete subjects 과목이름_1 과목이름_2 과목이름_3 ..." << endl;
		return;
	}

	for (string arg : args) {
		Subject* subject = 0;
		if (Util::is_integer(arg)) {
			//code
			int code = Util::to_integer(arg);
			for (Subject* sub : *subject_set) {
				if (sub->getCode() == code) {
					subject = sub;
				}
			}
		}
		else {
			//name
			for (Subject* sub : *subject_set) {
				if (sub->getName() == arg) {
					subject = sub;
				}
			}
		}

		if (subject == 0) {
			//error
			cout << "\'" << arg << "\'은 존재하지 않는 과목 또는 과목코드 입니다." << endl;
			return;
		}

		for (int i = 0; i < subject_set->size(); i++) {
			if (subject_set->at(i) == subject) {
				int sub_code = subject->getCode();
				string sub_name = subject->getName();
				subject_set->erase(subject_set->begin() + i);

				for (Student* stud : *student_set) {
					Grade* grade = stud->getGrade(sub_code);
					if (grade != 0) {
						stud->setGrade(sub_code, 0);
						delete grade;
					}
				}

				cout << "[" << sub_code << "]" << sub_name << "가 삭제되었습니다." << endl;
				delete subject;
				break;
			}
		}
	}
}

vector<string> cmd_delete_subjects_args(vector<string> args, vector<void*> outside_args) {
	vector<Subject*>* subject_set = static_cast<vector<Subject*>*>(outside_args[0]);
	vector<Student*>* student_set = static_cast<vector<Student*>*>(outside_args[1]);

	vector<string> auto_args_list;

	for (Subject* sub : *subject_set) {
		bool isExist = false;
		for (string arg : args) {
			bool isNum = Util::is_integer(arg);
			if ((isNum && sub->getCode() == Util::to_integer(arg)) || sub->getName() == arg) {
				isExist = true;
				break;
			}
		}

		if (!isExist) {
			auto_args_list.push_back(sub->getName());
		}
	}

	if (!isArgsAutoComed(args, args.size(), auto_args_list)) {
		return auto_args_list;
	}
	return {};
}

void cmd_delete_students(vector<string> args, vector<void*> outside_args) {
	vector<Subject*>* subject_set = static_cast<vector<Subject*>*>(outside_args[0]);
	vector<Student*>* student_set = static_cast<vector<Student*>*>(outside_args[1]);

	if (args.size() <= 0) {
		cout << "잘못 입력 되었습니다." << endl;
		cout << "(예시) delete students 학생코드_1 학생코드_2 학생코드_3 ..." << endl;
		return;
	}

	for (string arg : args) {
		Student* student = 0;
		if (Util::is_integer(arg)) {
			//code
			int code = Util::to_integer(arg);
			for (Student* stud : *student_set) {
				if (stud->getId() == code) {
					student = stud;
				}
			}
		}
		else {
			//error
			cout << "\'" << arg << "\'은 정수가 아닙니다." << endl;
			return;
		}

		if (student == 0) {
			//error
			cout << "\'" << arg << "\'은 존재하지 않는 과목 또는 과목코드 입니다." << endl;
			return;
		}


		for (int i = 0; i < student_set->size(); i++) {
			if (student_set->at(i) == student) {
				int stud_id = student->getId();
				string stud_name = student->getName();
				student_set->erase(student_set->begin() + i);
				cout << "[" << stud_id << "]" << stud_name << "가 삭제되었습니다." << endl;
				delete student;
				break;
			}
		}
	}
}

vector<string> cmd_delete_students_args(vector<string> args, vector<void*> outside_args) {
	return { "학생코드" };
}


void cmd_save(vector<string> args, vector<void*> outside_args) {
	vector<Subject*>* subject_set = static_cast<vector<Subject*>*>(outside_args[0]);
	vector<Student*>* student_set = static_cast<vector<Student*>*>(outside_args[1]);

	//subject(name:xx,code:xx)
	//calculated subject(name:xx,code:xx,fun:xx,args:[...])
	// 
	//student(name:xx,id:xx)
	//
	//grade_ID(XX:00,XX:00)

	if (args.size() == 0) {
		cout << "파일 위치를 입력하지 않았습니다." << endl;
		return;
	}

	string file_path = args[0];

	ofstream writeFile(file_path.data());
	if (writeFile.is_open()) {

		//subject phase start
		for (Subject* sub : *subject_set) {
			if (!sub->isCalculating()) {
				writeFile << "subject";
				writeFile << "(";
				writeFile << "name:" << sub->getName() << ",";
				writeFile << "code:" << sub->getCode();
				writeFile << ")" << endl;
			}
			else {
				writeFile << "calculated subject";
				writeFile << "(";
				writeFile << "name:" << sub->getName() << ",";
				writeFile << "code:" << sub->getCode() << ",";
				writeFile << "fun:" << "avg" << ",";
				writeFile << "args:" << "[";

				vector<string> subject_codes_string;
				for (int code : ((CalculatedSubject*)sub)->getCalculatedSubjectCodes()) {
					subject_codes_string.push_back(to_string(code));
				}

				writeFile << Util::join(subject_codes_string, "/");
				writeFile << "]";
				writeFile << ")" << endl;
			}
		}
		//subject phase end

		//student phase start
		for (Student* stud : *student_set) {
			writeFile << "student";
			writeFile << "(";
			writeFile << "name:" << stud->getName() << ",";
			writeFile << "id:" << stud->getId();
			writeFile << ")" << endl;
		}	
		//student phase end

		//grade phase start
		for (Student* stud : *student_set) {
			writeFile << "grade_" << stud->getId();
			writeFile << "(";
			vector<string> grade_data_string;
			for (Subject* sub : *subject_set) {
				string s = "";
				if (!sub->isCalculating()) {
					s = to_string(sub->getCode()) + ":" + to_string(stud->getGrade(sub->getCode())->getGrade());
				}
				else {
					s = to_string(sub->getCode()) + ":N";
				}

				grade_data_string.push_back(s);
			}

			writeFile << Util::join(grade_data_string, ",");

			writeFile << ")" << endl;
		}
		//grade phase end

		writeFile.close();

		cout << "저장 완료." << endl;
	}
	else {
		cout << "파일을 열수 없습니다." << endl;
		return;
	}
}


vector<string> cmd_save_args(vector<string> args, vector<void*> outside_args) {
	vector<Subject*>* subject_set = static_cast<vector<Subject*>*>(outside_args[0]);
	vector<Student*>* student_set = static_cast<vector<Student*>*>(outside_args[1]);

	return {"D:\save_1.txt"};
}

void cmd_load(vector<string> args, vector<void*> outside_args) {
	vector<Subject*>* subject_set = static_cast<vector<Subject*>*>(outside_args[0]);
	vector<Student*>* student_set = static_cast<vector<Student*>*>(outside_args[1]);

	if (args.size() == 0) {
		cout << "파일 위치를 입력하지 않았습니다." << endl;
		return;
	}

	string file_path = args[0];

	ifstream readFile(file_path.data());
	if (readFile.is_open()) {
		for (Student* stud : *student_set) {
			for (Subject* sub : *subject_set) {
				Grade* grade = stud->getGrade(sub->getCode());
				delete grade;
				stud->setGrade(sub->getCode(), 0);
			}
		}

		for (int i = 0; i < student_set->size(); i++) {
			Student* student = (*student_set)[student_set->size() - 1];
			student_set->pop_back();
			delete student;
		}

		for (int i = 0; i < subject_set->size(); i++) {
			Subject* sub = (*subject_set)[subject_set->size() - 1];
			subject_set->pop_back();
			delete sub;
		}

		cout << "전 데이터 삭제 완료." << endl;

		string line;
		while (getline(readFile, line)) {
			if (line.find("subject") == 0) {
				int front_len = string("subject").size();
				string bracket = line.substr(front_len + 1, line.size() - front_len - 2);
				vector<string> data = Util::split(bracket, ",");
				map<string, string> data_map;
				for (string s : data) {
					vector<string> ele = Util::split(s, ":");
					data_map[ele.at(0)] = ele.at(1);
				}
				string name = data_map["name"];
				int code = Util::to_integer(data_map["code"]);

				Subject* sub = new Subject(code, name);

				subject_set->push_back(sub);
			}
			else if (line.find("calculated subject") == 0) {
				int front_len = string("calculated subject").size();
				string bracket = line.substr(front_len + 1, line.size() - front_len - 2);
				vector<string> data = Util::split(bracket, ",");
				map<string, string> data_map;
				for (string s : data) {
					vector<string> ele = Util::split(s, ":");
					data_map[ele.at(0)] = ele.at(1);
				}

				string name = data_map["name"];
				string fun_name = data_map["fun"];
				string args_str = data_map["args"];
				int code = Util::to_integer(data_map["code"]);
				vector<int> args;
				for (string arg_str : Util::split(args_str.substr(1, args_str.size() - 2), "/")) {
					args.push_back(Util::to_integer(arg_str));
				}
				
				Subject* sub = (Subject*)new CalculatedSubject(code, name, args);

				subject_set->push_back(sub);
			}
			else if (line.find("student") == 0) {
				int front_len = string("student").size();
				string bracket = line.substr(front_len + 1, line.size() - front_len - 2);
				vector<string> data = Util::split(bracket, ",");
				map<string, string> data_map;
				for (string s : data) {
					vector<string> ele = Util::split(s, ":");
					data_map[ele.at(0)] = ele.at(1);
				}

				string name = data_map["name"];
				int id = Util::to_integer(data_map["id"]);

				Student* stud = new Student(id, name);

				student_set->push_back(stud);
			}
			else if (line.find("grade") == 0) {
				int front_len = string("grade").size();
				string sub_id_str = line.substr(front_len + 1, line.find("(") - front_len - 1);
				front_len += sub_id_str.size() + 1;
				int stud_id = Util::to_integer(sub_id_str);
				Student* student = 0;
				for (Student* stud : *student_set) {
					if (stud->getId() == stud_id) {
						student = stud;
						break;
					}
				}

				if (student == 0) {
					cout << "error:" << line << endl;
					continue;
				}

				string bracket = line.substr(front_len + 1, line.size() - front_len - 2);
				vector<string> data = Util::split(bracket, ",");
				map<string, string> data_map;
				for (string s : data) {
					vector<string> ele = Util::split(s, ":");
					data_map[ele.at(0)] = ele.at(1);
				}

				for (Subject* sub : *subject_set) {
					string sub_data = data_map[to_string(sub->getCode())];
					if (Util::is_integer(sub_data)) {
						Grade* grade = new Grade();
						grade->setGrade(Util::to_integer(sub_data));
						student->setGrade(sub->getCode(), grade);
					}
					else {
						CalculatedGrade* grade = new CalculatedGrade();
						grade->setCalculatingMethod(((CalculatedSubject*)sub)->getCalculatingMethod());
						student->setGrade(sub->getCode(), (Grade*)grade);
					}
				}
			}
		}

		cout << "불러오기 완료." << endl;
	}
	else {
		cout << "파일을 열수 없습니다." << endl;
		return;
	}

	


}


void cmd_sort_students(vector<string> args, vector<void*> outside_args) {
	vector<Subject*>* subject_set = static_cast<vector<Subject*>*>(outside_args[0]);
	vector<Student*>* student_set = static_cast<vector<Student*>*>(outside_args[1]);

	if (args.size() == 0) {
		//TODO: error message
		return;
	}
	string type = args.at(0);
	if (type == "id") {

	}
	else if (type == "name") {

	}
	else if (type == "subjects") {
		sort(student_set->begin(), student_set->end(), [](Student* a, Student* b) {return Student::compareBySubject(a, b,{}); });
	}
}


int main() {
	
	Command C_set("set");
	C_set.commandNext("subjects", cmd_set_subjects, cmd_set_subjects_args);
	C_set.commandNext("students", cmd_set_students, cmd_set_students_args);
	Command C_say("say", cmd_say, cmd_say_args);

	Command C_subjects("subjects", cmd_subjects, cmd_subjects_args);
	Command C_students("students", cmd_students, 0);

	Command C_modify("modify");
	C_modify.commandNext("subject", cmd_modify_subject, cmd_modify_subject_args);
	C_modify.commandNext("student", cmd_modify_student, cmd_modify_student_args);

	Command C_add("add");
	C_add.commandNext("subjects", cmd_add_subjects, cmd_add_subjects_args);
	Command* C_calculated = C_add.commandNext("calculated");//TODO: 변경
	C_calculated->commandNext("subjects", cmd_add_calculated_subjects, cmd_add_calculated_subjects_args);
	C_add.commandNext("students", add_students, cmd_add_students_args);//TODO:

	Command C_delete("delete");
	C_delete.commandNext("subjects", cmd_delete_subjects, cmd_delete_subjects_args);
	C_delete.commandNext("students", cmd_delete_students, cmd_delete_students_args);

	Command C_save("save", cmd_save, cmd_save_args);

	Command C_load("load", cmd_load, cmd_save_args);
	
	string cmd_line;

	CommandLine cl;

	vector<Subject*> subject_set;
	vector<Student*> student_set;


	while (true) {
		cmd_line = cl.getCommand({ &subject_set, &student_set });

		Command::act_cmd_fun(cmd_line, { &subject_set, &student_set });
	}
}
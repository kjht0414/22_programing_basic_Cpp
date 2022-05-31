#include<iostream>
#include<vector>
#include<conio.h>
#include<Windows.h>
#include<sstream>
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
	string (*auto_args_com)(vector<string>, int*);

	void (*cmd_fun)(vector<string>);

	Command(string name, void (*cmd_fun)(vector<string>), string(*auto_args_com)(vector<string>, int*),Command* parent) {
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

	Command(string name, void (*cmd_fun)(vector<string>), string(*auto_args_com)(vector<string>, int*)) {
		this->name = name;
		this->cmd_fun = cmd_fun;
		this->auto_args_com = auto_args_com;
		this->commands.push_back(this);
	}

	void commandNext(string name) {
		Command* child = new Command(name, 0, 0, this);
		this->children.push_back(child);
	}

	void commandNext(string name, void (*cmd_fun)(vector<string>), string(*auto_args_com)(vector<string>, int*)) {
		Command* child = new Command(name, cmd_fun, auto_args_com, this);
		this->children.push_back(child);
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

	static void act_cmd_fun(string cmd_line) {
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
			command_tail->cmd_fun(args);
		}
	}

	int isArgsCompletable() {
		if (cmd_fun==0 or auto_args_com == 0)
			return false;
		return true;
	}

	string argsCom(vector<string> args, int* index) {
		return auto_args_com(args, index);
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
	string getCommand() {
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
					search_auto_com();
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
						update_auto_com();
					}
				}
				else if (sp == LEFT_KEY) {
					if (cursor > 0) {
						cursor--;
						update_auto_com();
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
						update_auto_com();
					}
				}
			}
			else if (c == BACK_SPACE_KEY) {
				if (line.size() >= 1 and cursor > 0 and cursor <= line.size()) {
					line.erase(cursor-1, 1);
					cursor--;
					update_auto_com();
				}
			}
			else {
				line.insert(cursor,1,c);
				cursor++;
				update_auto_com();
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

	void update_auto_com() {
		if (auto_completion_lines.size() > 0) {
			clear_auto_com_print();
			search_auto_com();
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

	void search_auto_com() {
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
			if (cursored_element == "" or cmd_name.find(cursored_element) != -1 or cursor_buf < 0) {
				auto_completion_lines.push_back(cmd_name);
			}
		}

		if (command_tail != 0 and command_tail->isArgsCompletable()) {
			if (cursored_element_index+1 < command_elements.size()) {
				vector<string> args;
				for (int i = command_tail_index + 1; i < command_elements.size(); i++) {
					if(command_elements.at(i).size() > 0)
						args.push_back(command_elements.at(i));
				}
				int index;
				string arg_auto = command_tail->argsCom(args, &index);
				auto_completion_lines.push_back(arg_auto);
			}
		}
	}
};

void cmd_say(vector<string> args) {
	for (string s : args) {
		cout << s << endl;
	}
}

string cmd_say_args(vector<string> args,int* index) {
	string result;
	if (args.size() == 0) {
		result = "hello";
	}
	else {
		vector<string> _spt = Util::split(args.at(args.size() - 1), "_");
		int final_dash_num = 0;
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
		result = Util::join(_spt, "_");
	}
	
	return result;
}



int main() {
	Command set("set");
	set.commandNext("subjects");
	set.commandNext("students");
	Command say("say", cmd_say, cmd_say_args);

	string cmd_line;

	CommandLine cl;
	while (true) {
		cmd_line = cl.getCommand();

		Command::act_cmd_fun(cmd_line);
		
	}
}
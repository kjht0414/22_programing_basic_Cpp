/*
* 2개 이상의 클래스 만들기
* 한 클래스는 다른 클래스의 멤버 변수
* 메인에서 입력후 출력
*/

#include<iostream>
#include<vector>
using namespace std;



class Weapon {
private:
	string name;
	int lv;
	int atk;
public:
	void setWpn(string name, int lv, int atk) {
		this->name = name;
		this->lv = lv;
		this->atk = atk;
	}

	void getWpn(string& name, int& lv, int& atk) {
		name = this->name;
		lv = this->lv;
		atk = this->atk;
	}

	void print(){
		cout << "[" << this->lv << "]" << this->name << endl;
		cout << "atk: " << this->atk;
	}
};

class Character {
private:
	string name;
	int lv;
	Weapon weapon;
public:
	void setActer(string name, int lv, Weapon weapon) {
		this->name = name;
		this->lv = lv;
		this->weapon = weapon;
	}

	void getActer(string& name, int& lv, Weapon& weapon) {
		name = this->name;
		lv = this->lv;
		weapon = this->weapon;
	}

	void print() {
		cout << "[" << this->lv << "]"  <<  this->name << endl << endl;

		cout << "무기:" << endl;
		this->weapon.print();
	}
};

int main() {
	Character character;
	string c_name;
	int c_lv;
	Weapon c_weapon;
	string w_name;
	int w_lv;
	int w_atk;
	cout << "캐릭터의 이름 레벨 입력: ";
	cin >> c_name >> c_lv;

	cout << "무기의 이름 레벨 공격력 입력: ";
	cin >> w_name >> w_lv >> w_atk;

	c_weapon.setWpn(w_name, w_lv, w_atk);
	character.setActer(c_name, c_lv, c_weapon);

	character.print();

	vector<int> v;
	
}
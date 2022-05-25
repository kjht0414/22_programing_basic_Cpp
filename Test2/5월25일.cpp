#include<iostream>
using namespace std;

class Score {
private:

	int kor;
	int eng;
	int math;

public:
	/*
	Score(int k, int e, int m) {
		this->kor = k;
		this->eng = e;
		this->math = m;
	}
	*/
	void setKor(int k) {
		this->kor = k;
	}

	int getKor() {
		return this->kor;
	}

	void setEng(int e) {
		this->eng = e;
	}

	int getEng() {
		return this->eng;
	}

	void setMath(int m) {
		this->math = m;
	}

	int getMath() {
		return this->math;
	}

	void setScore(int k, int e, int m) {
		this->kor = k;
		this->eng = e;
		this->math = m;
	}

	void setScore(int* sc) {
		this->kor = sc[0];
		this->eng = sc[1];
		this->math =sc[2];
	}

	void getScore(int& k, int& e, int& m) {
		k = this->kor;
		e = this->eng;
		m = this->math;
	}

	void inSc() {
		cout << "성적 입력: ";
		cin >> this->kor >> this->eng >> this->math;
	}

	void outSc() {
		cout << "국어: " << this->kor << " "
			<< "영어: " << this->eng << " "
			<< "수학: " << this->math << endl;
	}
};

class Student {
private:

	int stdNo;
	string name;
	Score sc;

public:

	void setStu(int s, string n, Score sco) {
		this->stdNo = s;
		this->name = n;
		this->sc = sco;
	}

	void getStu(int& s, string& n, Score& sco) {
		s = this->stdNo;
		n = this->name;
		sco = this->sc;
	}

	void inStu() {
		cout << "이름 학번 입력:";
		cin >> this->name >> this->stdNo;
		this->sc.inSc();
	}

	void outStu() {
		cout << "학번: " << this->name << " "
			<< "이름: " << this->stdNo << " ";
		this->sc.outSc();
	}
};

int main() {
	Student stu;
	stu.inStu();
	stu.outStu();
}
#include<iostream>

using namespace std;

//�Լ� �����ε�

//int add(int a, int b) {
//	return a + b;
//}
//
//float add(float a, float b) {
//	return a + b;
//}

//���ø� ���
template<typename T>
T add(T a, T b) {
	return a + b;
}

//����ü
struct Vec2
{
	int x;
	int y;
};

class Score{
private:
	int kor;
	int eng;
	int math;
public:
	Score(int kor, int eng, int math) {
		this->kor = kor;
		this->eng = eng;
		this->math = math;
	}

	void set(int kor, int eng, int math) {
		this->kor = kor;
		this->eng = eng;
		this->math = math;
	}


	int getKor() {
		return kor;
	}

	int getEng() {
		return eng;
	}

	int getMath() {
		return math;
	}

	void getScore(int& k, int& e, int& m) {
		k = kor;
		e = eng;
		m = math;
	}
};

//����ü
struct Pos3 {
	int x, y, z;
};

class Vec3 {
private:
	int x, y, z;
public:
	Vec3(int x, int y, int z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vec3(Pos3 p) {
		this->x = p.x;
		this->y = p.y;
		this->z = p.z;
	}

	int getX() {
		return this->x;
	}

	int getY() {
		return this->y;
	}

	int getZ() {
		return this->z;
	}

	struct Pos3 getPos() {
		struct Pos3 v3 = { this->x ,this->y ,this->z };
		return v3;
	}

	void add(Vec3 src) {
		struct Pos3 v3 = src.getPos();
		int x=v3.x, y=v3.y, z=v3.z;
		this->x += x;
		this->y += y;
		this->z += z;
	}
};



int main() {
	/*int x = 7, y = 3;
	int z = add(x, y);
	cout << "x + y = " << z << endl;
	float a = 6.55, b = 3.5;
	float c = add(a, b);
	cout << "a + b = " << c << endl;*/

	//������

	/*int a = 100;
	int& b = a;
	int* p = &a;
	cout << "a = " << a << endl;
	cout << "b = " << b << endl;
	cout << "*p = " << *p << endl;
	cout << "p = " << p << endl;
	cout << "&a = " << &a << endl;
	cout << "&b = " << &b << endl;
	cout << "&p = " << &p << endl;*/

	//����ü

	/*Vec2 v = { 20,30 };*/

	//class !!!!

	/*Score s(100,100,100);
	cout << "����: " << s.getKor() << endl;
	cout << "����: " << s.getEng() << endl;
	cout << "����: " << s.getMath() << endl;

	cout << "=======================" << endl;

	int k, e, m;
	s.getScore(k, e, m);

	cout << "����: " << k << endl;
	cout << "����: " << e << endl;
	cout << "����: " << m << endl;*/

	//������ ����
	//Ŭ���� �ϳ� ����� ���(�����)
	//���� ��Ī�� ����ü ���� ��뿹

	Pos3 p1;
	Pos3 p2;

	cout << "V1�� x y z ���� �Է�:";
	cin >> p1.x >> p1.y >> p1.z;
	cout << "V2�� x y z ���� �Է�:";
	cin >> p2.x >> p2.y >> p2.z;

	Vec3 v1(p1);
	Vec3 v2(p2);

	cout << "V1: " << "  "
		<< "x: " << v1.getX() << "  "
		<< "y: " << v1.getY() << "  "
		<< "z: " << v1.getZ() << endl;

	cout << "V2: " << "  "
		<< "x: " << v2.getX() << "  "
		<< "y: " << v2.getY() << "  "
		<< "z: " << v2.getZ() << endl;

	v1.add(v2);
	cout << "V1+V2: " << "  "
		<< "x: " << v1.getX() << "  "
		<< "y: " << v1.getY() << "  "
		<< "z: " << v1.getZ() << endl;
}
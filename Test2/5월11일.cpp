#include<iostream>
using namespace std;



int main() {
	/*int a, b ,result;
	cout << "�� ������ �ڷḦ �Է�:";
	cin >> a >> b;
	result = a + b;
	cout << a << "�� " << b << "�� �Է¹޾ҽ��ϴ�." << endl;
	cout << a << " + " << b << " = " << result << endl;*/

	//����
	//seed�� ����
	//srand(time(0));
	/*for (int i = 0; i < 10; i++) {
		cout << rand()%6 << endl;
	}*/
	/*cout << "���� �޴�?" << endl;
	char medals[3][3] = {"��","��","��"};
	for (int i = 0; i < 10; i++) {
		int value = rand() % 3;
		cout << medals[value] << "�޴�" << endl;

	}*/

	//������ ����
	//C++�� ����� ������ ������ �ڵ�

	int target, chance, size;
	srand(time(0));
	cout << "size for range: ";
	cin >> size;
	cout << "range: 1~" << size << endl;
	target = rand() % size + 1;
	cout << "chance: ";
	cin >> chance;
	cout << "game start" << endl;
	while (1) {
		if (chance <= 0) {
			cout << "loser!" << endl;
			break;
		}

		int attack;
		cout << "input: ";
		cin >> attack;
		if (target == attack) {
			cout << "correct!!" << endl;
			cout << "winner!" << endl;
			break;
		}
		else if (target > attack) {
			cout << "up" << endl;
		}
		else if (target < attack) {
			cout << "down" << endl;
		}
		chance--;
	}
	cout << "game end" << endl;
}
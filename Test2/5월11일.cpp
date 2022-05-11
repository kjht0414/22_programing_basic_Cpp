#include<iostream>
using namespace std;



int main() {
	/*int a, b ,result;
	cout << "두 정수형 자료를 입력:";
	cin >> a >> b;
	result = a + b;
	cout << a << "와 " << b << "를 입력받았습니다." << endl;
	cout << a << " + " << b << " = " << result << endl;*/

	//난수
	//seed값 설정
	//srand(time(0));
	/*for (int i = 0; i < 10; i++) {
		cout << rand()%6 << endl;
	}*/
	/*cout << "무슨 메달?" << endl;
	char medals[3][3] = {"금","은","동"};
	for (int i = 0; i < 10; i++) {
		int value = rand() % 3;
		cout << medals[value] << "메달" << endl;

	}*/

	//오늘의 연습
	//C++의 입출력 난수을 응용한 코드

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
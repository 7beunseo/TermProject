#include <iostream>
#include <conio.h>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

#define SIZE 20

// ���� - �÷��̾��� ������ ���� 
class Player {
	string stone;
public:
	Player(string stone) {
		this->stone = stone;
	}
	string getStone() {
		return stone;
	}
};

// �ٵ��� ���� -> ���� �����Ҽ��� �ְ�~ �ٵ� ������ ���� �հ�~ 
class BasicSetting {
protected:
	string pan[SIZE][SIZE];
public:
	// �ٵ��� ����
	BasicSetting();
	// �ٵ��� ��Ȳ ���
	void show();
	// �ٵ��ǿ� �� �α�
	bool set(int x, int y, string stone);
	// �̹� ���� ��ġ���ִ��� Ȯ��
	bool check(int x, int y);
	// ���� �ǰ� -> �����Լ� 
	virtual bool configure(string stone)=0;
};
BasicSetting::BasicSetting() {
	// �𼭸� ���� 
	pan[0][0] = "��";
	pan[19][0] = "��";
	pan[0][19] = "��";
	pan[19][19] = "��";

	// �׵θ� ���� 
	for (int i = 1; i < SIZE - 1; i++) {
		pan[i][0] = "��";
		pan[0][i] = "��";
		pan[19][i] = "��";
		pan[i][19] = "��";
	}

	// ���� ���ڰ��� ä��
	for (int i = 1; i < SIZE - 1; i++) {
		for (int k = 1; k < SIZE - 1; k++)
			pan[i][k] = "��";
	}
	
}
bool BasicSetting::check(int x, int y) {
	if (x < 0 || x >= SIZE || y < 0 || y >= SIZE) {
		cout << "       ";
		cout << x << " " << y;
		cout << "�־��� ������ ����ϴ�" << endl;
		return false;
	}
	if (pan[y][x] == "��" || pan[y][x] == "��") {
		cout << "�ش� ��ġ�� �̹� ���� �����մϴ�" << endl;
		return false;
	}
	return true; // ���̾���=�� �� �ִ�
}
void BasicSetting::show() {
	cout << endl << endl << "         <�ٵ��� ��Ȳ>" << endl;
	for (int i = 0; i < SIZE; i++) {
		cout << "       ";
		for (int k = 0; k < SIZE; k++) {
			cout << pan[i][k];
		}
		cout << " " << i + 1 << endl;
	}
	cout << "       ";
	for (int i = 0; i < SIZE; i++)
		cout << (char)(i + 65) << " ";
	cout << endl;
}
bool BasicSetting::set(int x, int y, string stone) { // 1~20���� ������ �����ϴ°�?
	if (!check(x, y))
		return true;
	pan[y ][x] = stone;
	return false; // false�� �����ϸ� ���������� ���� �� 

}
// ������ ���� ȯ�� ���� 
class Setting : public BasicSetting {
public:
	// ������ ���� ���� �ǰ� 
	bool configure(string stone);
};
bool Setting::configure(string stone) {
	// false�� �����ϸ� ���� ���ڸ� �Ǻ��� �� ����.
	// true�� �����ϸ� �ش� ���� �÷��̾ �̱� �� 
	int k;
	for (int x = 0; x < SIZE - 5; x++) {
		for (int y = 0; y < SIZE; y++) {
			for (k = 0; k < 5; k++) {
				if (pan[x + k][y] != stone)
					break;
			}
			if (k == 5)
				return true;
		}
	}
	// ���� 5�� Ȯ�� 
	for (int x = 0; x < SIZE; x++) {
		for (int y = 0; y < SIZE - 5; y++) {
			for (k = 0; k < 5; k++) {
				if (pan[x][y+k] != stone)
					break;
			}
			if (k == 5)
				return true;
		}
	}
	// ������ �밢�� Ȯ��
	for (int x = 0; x < SIZE - 5; x++) {
		for (int y = 0; y < SIZE - 5; y++) {
			for (k = 0; k < 5; k++) {
				if (pan[x + k][y+k] != stone)
					break;
			}
			if (k == 5)
				return true;
		}
	}
	// ���� �밢�� Ȯ��
	for (int x = SIZE - 1; x > 5; x--) {
		for (int y = 0; y < SIZE - 5; y++) {
			for (k = 0; k < 5; k++) {
				if (pan[y + k][x - k] != stone)
					break;
			}
			if (k == 5)
				return true;
		}
	}
	return false;
}

// ��ǻ�� �÷��̾�
class AutoSetting : public Setting {
public:
	bool com_configure(string stone, int i);
	bool random_set(int* arr1, int* arr2);
	void full_random();
};
void AutoSetting::full_random() {
	while (true) {
		int x = rand() % SIZE;
		int y = rand() % SIZE;
		if (check(x, y)) {
			set(x, y, "��");
			return;
		}

	}
}
bool AutoSetting::random_set(int* arr1, int* arr2) {
	// ��ȿ�� �ε��� ���� ���� ���
	if (arr1[0] == -1 && arr2[0] == -1) return false; // �ٵϵ��� set ���� ����
	if (arr1[0] == -1) set(arr2[0], arr2[1], "��");
	else if (arr2[0] == -1) set(arr1[0], arr1[1], "��");
	else {
		int n = rand() % 2;
		if(n==0)
			set(arr1[0], arr1[1], "��");
		else
			set(arr2[0], arr2[1], "��");
	}
	return true;
}
bool AutoSetting::com_configure(string stone, int i) {
	int arr1[2] = { -1,-1 }; // ���� 2���� ���ɼ��� �����Ƿ� �迭 2�� ����, �Ŀ� 2�� ���� ��ȯ
	int arr2[2]={-1,-1};
	int k;
	// false�� �����ϸ� i ���� ���ӵ� ���� ���� -> ���� ������ �������� �� 
	// true�� �����ϸ� i���� ���ӵ� ���� �ִٴ� �� -> ���� ������� ���ƾ� ��
	// {n,m} �迭�� ����  ��ǻ�Ͱ� �ش� ��ġ�� ���� �� 
	// ���� i�� Ȯ��
	for (int x = 0; x < SIZE - i; x++) {
		for (int y = 0; y < SIZE; y++) {
			for (k = 0; k < i; k++) {
				if (pan[y][x + k] != stone)
					break;
			}
			if (k == i) {
				if (check(x + i, y)) {
					arr1[0] = x + i;
					arr1[1] = y;
				}
				if (check(x - 1, y)) {
					arr2[0] =x-1;
					arr2[1] = y;
				}
				if (random_set(arr1, arr2))
					return true;
				}
			}
		}

		// ���� 5�� Ȯ�� 
		for (int x = 0; x < SIZE; x++) {
			for (int y = 0; y < SIZE - i; y++) {
				for (k = 0; k < i; k++) {
					if (pan[y + k][x] != stone)
						break;
				}
				if (k == i) {
						if (check(x, y + i)) {
							arr1[0] = x;
							arr1[1] = y + i;
						}
						if (check(x, y -1)) {
							arr2[0] = x;
							arr2[1] = y -1;
						}
						if (random_set(arr1, arr2))
							return true;

				}
			}
		}

		// �밢�� Ȯ��
		for (int x = 0; x < SIZE - i; x++) {
			for (int y = 0; y < SIZE - i; y++) {
				for (k = 0; k < i; k++) {
					if (pan[y + k][x + k] != stone)
						break;
				}
				if (k == i) {


						if (check(x + i, y + i)) {
							arr1[0] = x + i;
							arr1[1] = y + i;
						}
						if (check(x - 1, y - 1)) {
							arr2[0] = x - 1;
							arr2[1] = y - 1;
						}
						if (random_set(arr1, arr2))
							return true;
					
				}
			}
		}
		//���� �밢�� Ȯ��
		for (int x = SIZE - 1; x > i; x--) {
			for (int y = 0; y < SIZE - i; y++) {
				for (k = 0; k < i; k++) {
					if (pan[y + k][x - k] != stone) {
						break;
					}
				}
				if (k == i) {
					if (check(x - i, y + i)) {
						arr1[0] = x - i;
						arr1[1] = y + i;
					}
					if (check(x + 1, y - 1)) {
						arr2[0] = x + 1;
						arr2[1] = y - 1;
					}
					if (random_set(arr1, arr2))
						return true;
				}
			}
		}


		return false;
	}
	


class AutoFiveRoll {
	AutoSetting pan;
public:
	void play() {
		srand((unsigned)time(0));
		Player p("��");
		for (int i = 1; ; i = (++i) % 2) {
			system("cls"); // ȭ�������
			pan.show();
			char x;
			int y;
			if (i == 1) {
				cout << "       ";
				cout << "**player" << i << " (" << p.getStone() << ") " << "**" << endl;
				do {
					cout << "       ";
					cout << "x ��ǥ�� �� �Է� >> ";
					cin >> x;
					cout << "       ";
					cout << "y ��ǥ�� �� �Է� >> ";
					cin >> y;
				} while (pan.set((int)x - 65, y-1, p.getStone()));

				if (pan.configure(p.getStone())) {
					system("cls"); // ȭ�������
					pan.show();
					cout << "player" << i << "�¸�!";
					return;
				}
			}
			else {
			
				if (!pan.com_configure("��", 4)) { // �ڱ� �� ���� Ȯ�� 
					if (!pan.com_configure("��", 4)) {
						if (!pan.com_configure("��", 3)) {
							if (!pan.com_configure("��", 3)) {
								if (!pan.com_configure("��", 2)) {
									if (!pan.com_configure("��", 2)) {
										if (!pan.com_configure("��", 1))
											;
									}
								
								}
							}
						}
					}
				}
				if (pan.configure("��")) {
					system("cls"); // ȭ�������
					pan.show();
					cout << "com" << "�¸�!";
					return;
				}

			}
		}
	}
};


class FiveRoll {
	Setting pan;
public:
	void play() {
		Player p[2] = { Player("��"), Player("��") };
		for (int i = 0; ; i = (++i) % 2) {
			system("cls"); // ȭ�������
			pan.show();
			cout << "       ";
			cout << "**player" << i+1 << " (" << p[i].getStone() << ") " << "**" << endl;
			char x;
			int y;
			do {
				cout << "       ";
				cout << "x ��ǥ�� �� �Է� >> ";
				cin >> x;
				cout << "       ";
				cout << "y ��ǥ�� �� �Է� >> ";
				cin >> y;
			} while (pan.set((int)x - 65 , y-1, p[i].getStone()));
			if (pan.configure(p[i].getStone())) {
				system("cls"); // ȭ�������
				pan.show();
				cout << "player" << i+1 << "�¸�!";
				return;
			}
		}
	}
};

class BaseBall {
	int setNum[3];
	int baseball[3];
	int getStrike() {
		int count = 0;
		for (int i = 0; i < 3; i++) {
			if (baseball[i] == setNum[i])
				count++;
		}
		return count;
	}
	int getBall() {
		int count = 0;
		for (int i = 0; i < 3; i++) {
			for (int k = 0; k < 3; k++) {
				if (i != k && setNum[i] == baseball[k])
					count++;
			}
		}
		return count;
	}
public:
	void play() {
		system("cls");
		cout << endl << endl << "         <���󿡼� �ι�°�� ����ִ� ���� �߱� ����>" << endl;
		
		int i=0;
		srand((unsigned)time(0));
		while (i < 3) {
			int n = rand() % 9 + 1;
			int k=i;
			for (k = 0; k < i; k++) {
				if (n == setNum[k])
					break;
			}
			if (i == k) {
				setNum[i] = n;
				i++;
			}
		}

		
		while (true) {
			cout << "           ���� 3���� �Է��ϼ��� >> ";
			for (int i = 0; i < 3; i++)
				
				cin >> baseball[i];
			int strike = getStrike();
			int ball = getBall();
			cout << "           ";
			if (strike == 3) {
				cout << "           ";
				cout << "Ȩ��!" << endl;
				break;
			}
			if (!ball && strike)
				cout << strike << " s" << endl;
			else if (ball && strike)
				cout << strike << " s, " << ball << "b" << endl;
			else if (ball) {
				cout << ball << " b" << endl;
			}
			else {
				cout << "out" << endl;
			}
		}
	}
};

// ��ü �޴� ���
class GameMenu {
public:
	static void choice() {
		cout << "� ������ �����Ͻðڽ��ϱ�?" << endl;
		cout << "1 : ���� �߱�����, 2 : 2�� ����, 3 : 1�� ���� >> ";
		int num;
		cin >> num;
		switch (num) {
		case 2: {
			FiveRoll s;
			s.play();
			break;
		}
		case 1: {
			BaseBall g;
			g.play();
			break;
		}
		case 3: {
			AutoFiveRoll a;
			a.play();
			break;
		}
		default:
			cout << "���� �߸� �Է�";
		}
	}
};

int main() {
	GameMenu::choice();
}
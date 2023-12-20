#include <iostream>
#include <conio.h>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

#define SIZE 20

// 오목 - 플레이어의 정보를 담음 
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

// 바둑판 생성 -> 오목 생성할수도 있고~ 바둑 생성할 수도 잇고~ 
class BasicSetting {
protected:
	string pan[SIZE][SIZE];
public:
	// 바둑판 구성
	BasicSetting();
	// 바둑판 현황 출력
	void show();
	// 바둑판에 돌 두기
	bool set(int x, int y, string stone);
	// 이미 돌이 위치해있는지 확인
	bool check(int x, int y);
	// 승자 판결 -> 가상함수 
	virtual bool configure(string stone)=0;
};
BasicSetting::BasicSetting() {
	// 모서리 설정 
	pan[0][0] = "┌";
	pan[19][0] = "└";
	pan[0][19] = "┐";
	pan[19][19] = "┘";

	// 테두리 설정 
	for (int i = 1; i < SIZE - 1; i++) {
		pan[i][0] = "├";
		pan[0][i] = "┬";
		pan[19][i] = "┸";
		pan[i][19] = "┤";
	}

	// 내부 십자가로 채움
	for (int i = 1; i < SIZE - 1; i++) {
		for (int k = 1; k < SIZE - 1; k++)
			pan[i][k] = "╂";
	}
	
}
bool BasicSetting::check(int x, int y) {
	if (x < 0 || x >= SIZE || y < 0 || y >= SIZE) {
		cout << "       ";
		cout << x << " " << y;
		cout << "주어진 범위를 벗어납니다" << endl;
		return false;
	}
	if (pan[y][x] == "●" || pan[y][x] == "○") {
		cout << "해당 위치에 이미 돌이 존재합니다" << endl;
		return false;
	}
	return true; // 돌이없다=둘 수 있다
}
void BasicSetting::show() {
	cout << endl << endl << "         <바둑판 현황>" << endl;
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
bool BasicSetting::set(int x, int y, string stone) { // 1~20값의 범위에 존재하는가?
	if (!check(x, y))
		return true;
	pan[y ][x] = stone;
	return false; // false를 리턴하면 정상적으로 돌을 둠 

}
// 오목을 위한 환경 설정 
class Setting : public BasicSetting {
public:
	// 오목을 위한 승자 판결 
	bool configure(string stone);
};
bool Setting::configure(string stone) {
	// false를 리턴하면 아직 승자를 판별할 수 없다.
	// true를 리턴하면 해당 돌의 플레이어가 이긴 것 
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
	// 세로 5개 확인 
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
	// 오른쪽 대각선 확인
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
	// 왼쪽 대각선 확인
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

// 컴퓨터 플레이어
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
			set(x, y, "○");
			return;
		}

	}
}
bool AutoSetting::random_set(int* arr1, int* arr2) {
	// 유효한 인덱스 값이 없는 경우
	if (arr1[0] == -1 && arr2[0] == -1) return false; // 바둑돌을 set 하지 못함
	if (arr1[0] == -1) set(arr2[0], arr2[1], "○");
	else if (arr2[0] == -1) set(arr1[0], arr1[1], "○");
	else {
		int n = rand() % 2;
		if(n==0)
			set(arr1[0], arr1[1], "○");
		else
			set(arr2[0], arr2[1], "○");
	}
	return true;
}
bool AutoSetting::com_configure(string stone, int i) {
	int arr1[2] = { -1,-1 }; // 양쪽 2개의 가능성이 있으므로 배열 2개 설정, 후에 2개 랜덤 반환
	int arr2[2]={-1,-1};
	int k;
	// false를 리턴하면 i 개의 연속된 돌이 없다 -> 다음 순위로 내려가야 함 
	// true를 리턴하면 i개의 연속된 돌이 있다는 것 -> 높은 순위대로 막아야 함
	// {n,m} 배열에 따라  컴퓨터가 해당 위치에 돌을 둠 
	// 가로 i개 확인
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

		// 세로 5개 확인 
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

		// 대각선 확인
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
		//왼쪽 대각선 확인
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
		Player p("●");
		for (int i = 1; ; i = (++i) % 2) {
			system("cls"); // 화면지우기
			pan.show();
			char x;
			int y;
			if (i == 1) {
				cout << "       ";
				cout << "**player" << i << " (" << p.getStone() << ") " << "**" << endl;
				do {
					cout << "       ";
					cout << "x 좌표의 값 입력 >> ";
					cin >> x;
					cout << "       ";
					cout << "y 좌표의 값 입력 >> ";
					cin >> y;
				} while (pan.set((int)x - 65, y-1, p.getStone()));

				if (pan.configure(p.getStone())) {
					system("cls"); // 화면지우기
					pan.show();
					cout << "player" << i << "승리!";
					return;
				}
			}
			else {
			
				if (!pan.com_configure("○", 4)) { // 자기 돌 먼저 확인 
					if (!pan.com_configure("●", 4)) {
						if (!pan.com_configure("○", 3)) {
							if (!pan.com_configure("●", 3)) {
								if (!pan.com_configure("○", 2)) {
									if (!pan.com_configure("●", 2)) {
										if (!pan.com_configure("●", 1))
											;
									}
								
								}
							}
						}
					}
				}
				if (pan.configure("○")) {
					system("cls"); // 화면지우기
					pan.show();
					cout << "com" << "승리!";
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
		Player p[2] = { Player("●"), Player("○") };
		for (int i = 0; ; i = (++i) % 2) {
			system("cls"); // 화면지우기
			pan.show();
			cout << "       ";
			cout << "**player" << i+1 << " (" << p[i].getStone() << ") " << "**" << endl;
			char x;
			int y;
			do {
				cout << "       ";
				cout << "x 좌표의 값 입력 >> ";
				cin >> x;
				cout << "       ";
				cout << "y 좌표의 값 입력 >> ";
				cin >> y;
			} while (pan.set((int)x - 65 , y-1, p[i].getStone()));
			if (pan.configure(p[i].getStone())) {
				system("cls"); // 화면지우기
				pan.show();
				cout << "player" << i+1 << "승리!";
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
		cout << endl << endl << "         <세상에서 두번째로 재미있는 숫자 야구 게임>" << endl;
		
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
			cout << "           숫자 3개를 입력하세요 >> ";
			for (int i = 0; i < 3; i++)
				
				cin >> baseball[i];
			int strike = getStrike();
			int ball = getBall();
			cout << "           ";
			if (strike == 3) {
				cout << "           ";
				cout << "홈런!" << endl;
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

// 전체 메뉴 출력
class GameMenu {
public:
	static void choice() {
		cout << "어떤 게임을 선택하시겠습니까?" << endl;
		cout << "1 : 숫자 야구게임, 2 : 2인 오목, 3 : 1인 오목 >> ";
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
			cout << "숫자 잘못 입력";
		}
	}
};

int main() {
	GameMenu::choice();
}
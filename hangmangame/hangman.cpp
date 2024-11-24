#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<vector>
#include<string>
#include<fstream>
#include<conio.h>
#include<Windows.h>
#include<filesystem>
using namespace std;
namespace fs = std::filesystem;

// 콘솔 내부 특정 위치로 커서 이동시키는 함수. 해당 위치부터 문자 출력시켜 원하는 그림 만든다.
void gotoxy(int x, int y)
{
	COORD Pos;
	Pos.X = 2 * x; // 그냥 x로 하는 것보다 그림 예쁘게 만들기 위해..
	Pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos); // 받은 위치로 커서 옮김.
}

void SetColor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

//keyboard 입력
int GetKeyDown()
{
	if (_kbhit() != 0) // _kbhit() : 키보드가 눌리면 0 이외의 값 return
	{
		return _getch(); // _getch() : scanf 처럼 입력 받는 기능이다. 다만 scanf, cout 처럼 입력 받은 값을 출력하기 위해 쓰지 않음.
	}
	return 0;
}

//콘솔 세팅
void SetConsoleView()
{
	//system("mode con:cols=50 lines=20"); // 콘솔창 사이즈 조절 
	system("title HANG MAN GAME");		// 콘솔창 제목 "HANG MAN GAME" 으로 설정.
}

void CursorView(char show) // 커서 보이지 않게, 숨기는 함수.
{
	HANDLE hConsole; // 운영체제의 리소스는 대부분 메모리로 구성되어 주소를 가지는데, 이 리소스를 응용프로그램이 직접 사용 시
	// 운영체제가 내부의 정보를 안전하게 관리할 수 없다. 그래서 사용자 공간에서 실행되는 응용 프로그램은 포인터로 직접
	// 운영체제의 리소스 주소에 접근이 불가능하다. 대신 핸들이라는 개념을 사용한다. 핸들은 운영체제 내부의 리소스 주소를
	// 정수로 치환한 값이다. 그리고 리소스 주소와 이 핸들값을 한 쌍으로 묶어서 관리하는데, 이를 handle table이라고 한다.
	// 쉽게 handle = os 내부에서 관리하는 자료구조 라고 생각하면 편함.
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // console에 출력하는 커서DML handle(ID)를 받아 hConsole에 저장.

	ConsoleCursor.bVisible = show; // .bVisible : 0 입력 시 커서 안 보임, 그외 정수 입력 시 보임.. 
	ConsoleCursor.dwSize = 1;  // .dwSize : 커서 굵기. 1~100 까지 가능

	SetConsoleCursorInfo(hConsole, &ConsoleCursor); // 지정된 콘솔 화면 버퍼에 대한 커서의 크기와 표시 유형을 설정
}


//시작화면 draw
void DrawReadyGame()
{
	CursorView(0);
	system("cls");
	gotoxy(5, 2);
	cout << "==============================";
	gotoxy(5, 3);
	cout << "======= HANG MAN GAME ========";
	gotoxy(5, 4);
	cout << "==============================";
	gotoxy(9, 6);
	cout << " - 시작하기 -";
	gotoxy(9, 7);
	cout << " - 게임종료 -";
	gotoxy(0, 10);
	cout << "메뉴는 ENTER키로 선택가능합니다.";

}

//게임화면 draw
void DrawStartGame(int life, int score, vector<string>& pastWord)
{
	system("cls"); // system("명령어") : console창에서 cmd 명령어를 사용할 수 있게 해줌. 또는 파일이름 넣어서 파일 실행도 가능..
	gotoxy(5, 1);  // 예를 들어 명령어에 "explorer http://naver.com" 넣으면 네이버 켜짐..
	cout << "life = " << life;
	gotoxy(5, 2);
	cout << "score = " << score;
	gotoxy(5, 8);
	cout << "past = ";
	for (int i = 0; i < static_cast<int>(pastWord.size()); ++i) // 게임 처음엔 pastWord.size()=0 이므로 빈칸 출력됨.
	{
		cout << pastWord[i] << " ";
	}

	gotoxy(5, 12);
	cout << "input = ";
	gotoxy(13, 14);
	cout << "나가기" << endl;
}

//사전을 세팅하는 함수 입니다.
void SetDictionary(vector<string>& strArr)
{
	static const int INIT_NUM = 4;
	static const string str[INIT_NUM] = { "apple", "banana", "code", "dog" };	//샘플 단어.
	string dict_file = "words.txt";
	ifstream readFromFile(dict_file); // input file stream 약자. ifstream readFromFile("words.txt") : 읽을 파일 경로를 words.txt 로 지정함.
										// 해당 파일을 readFromFile로 별명짓는다.
	if(!fs::exists(dict_file)) // std:filesystem::exists("파일명") : 해당 파일 존재하면 1 return , 존재 x 시 0 return 
	{				// 파일이 없다면, 파일이 닫힌 상태일테니 이 if문에 걸림. words.txt 파일을 만들어주자.
		ofstream writeToFile(dict_file); // output file stream 약자. ofstream writeToFile("파일경로") : 쓸 파일 경로를 지정함. 
		for (int i = 0; i < INIT_NUM; ++i) // INIT_NUM = 4, 4회 반복.. 파일 수정 시 line 수만큼 반복함.
		{
			string tmp = str[i]; // tmp에 "apple"부터 다른 word 들을 차례대로 넣음.
			if (i != INIT_NUM - 1) // 단어개수 - 1(줄바꿈 수)와 같아질 때까지 줄바꿈("\n") 넣음.
			{
				tmp += "\n"; // string+"\n"형태로 만듬.마지막 단어빼고..
			}
			writeToFile.write(tmp.c_str(), tmp.size());  // 객체.write(입력값, 입력사이즈) : 파일에 쓰는(write) 함수임.
				// tmp.c_str() : string class 보면 string은 사실 char형의 배열이다. 그래서 .c_str() 하면 string 시작 주소를 반환함.
				// tmp.size() : string tmp의 size(크기)를 반환한다. sizeof와 달리 byte size가 아니라.. 문자열의 경우 길이, 벡터나 배열의 경우 요소 개수를 반환함. 
			strArr.push_back(str[i]); // strArr(string 벡터)에 str 벡터 요소 값{ "apple", "banana", "code", "program" }을 넣는다.
		}
		writeToFile.close();	// 파일 닫기.
		return;
	}
	
	while (!readFromFile.eof()) // .eof() : 함수 실행될 때마다 첫 line부터 계속 다음 line 읽음.. 읽을 것이 있으면 0 반환하다가 읽을 것이 없으면 1 반환함.
	{
		string tmp;
		getline(readFromFile, tmp); // readFromFile을 줄단위로 읽어서 tmp에 저장.
		strArr.push_back(tmp);      // 저장된 단어를 strArr 벡터에 넣음.
	}
	readFromFile.close(); // 파일 readFromFile을 닫는다.
	return;
}

//시작화면 기능
bool ReadyGame()
{
	DrawReadyGame(); // 게임 준비화면 그리는 함수.
	while (true)
	{
		int key = GetKeyDown(); // 키보드가 눌리면 눌린 값을 int형 변수 key에 저장.
		if (key == 's' || key == 'S') // 대/소문자 s일 경우 -> 1(True)반환
		{
			return true;
		}
		else if (key == 'q' || key == 'Q') // // 대/소문자 q일 경우 0(false) 반환.
		{
			return false;
		}
	}
}

int GameOver() {
	system("cls");
	cout << "GAME OVER\n";
	cout << "ESC 입력 시 게임이 종료됩니다.\n";
	cout << "재시작을 위해 그외의 아무 키나 입력해주세요.^_^";
	int key;
	while (1) {					 // 입력 받을때까지 while문 돌림.
		key =  GetKeyDown();	// key에 입력받음. _kbhit 쓰므로 입력 없을 시 계속 0 return 됨.
		if (key != 0) break;
	}
	if (key == 27) return 0; // esc ASCII CODE가 27
	else return 1;
}

void solve_or_out(string word, int len_word, int mode){
	//draw question
	if (mode == 1) { // mode 1 일 경우 입력창이 선택됨.
		SetColor(11);
		gotoxy(5, 12);
		cout << "input = ";

		SetColor(15);
		gotoxy(13, 14);
		cout << "나가기" << endl;
	}
	else if (mode == 2) { // mode 2일 경우 메인화면 qq 선택됨.
		SetColor(11);
		gotoxy(13, 14);
		cout << "나가기" << endl;
		
		SetColor(15);
		gotoxy(5, 12);
		cout << "input = ";
		
	}
	
	return;
}

//게임 시작 함수.
void StartGame()
{
	int score = 0;
	vector<string> pastWord;
	vector<string> strArr;
	SetDictionary(strArr);		//"word.txt" 파일에서 단어 읽어 strArr에 저장하는 함수.

	while (true)	//하나의 탄을 표현하는 루프
	{
		//1 play
		int num = 0;
		srand((unsigned int)time(NULL));		
		// srand(seed값) : seed 값에 따라 0~32367 사이에서 한가지 숫자 랜덤하게 골라줌. 
		// (unsigned int)time(NULL) : 1970년 1월 1일 0시 0분 0초로부터 지난 시간을 반환.
		num = rand() % static_cast<int>(strArr.size()); // strArr 배열의 크기에 맞춰 0 ~ (배열크기 - 1)의 값을 랜덤으로 num에 저장.
		// rand() : 0~32367(2byte 최대표현범위)에서 한가지 숫자 랜덤하게 골라줌. 단 1개만 계속 골라줌..
		// static_cast<int> : int형으로 형변환하되, 명시적 형변환[(int*)(double형포인터)] 같은 경우도 잡도록 코드 상에서 오류 잡아줌.
		string strQuestion;
		const string strOriginal = strArr[num];	//배열 크기에 맞춰 랜덤하게 뽑은 값 num에 맞는 단어를 정답확인용으로 저장
		const int originLen = static_cast<int>(strOriginal.length());

		//init
		for (int i = 0; i < originLen; ++i) // "_"를 단어 수만큼 출력하기 위해 strOriginal.length를 받아서 출력.
		{
			strQuestion += "_";
		}

		int life = originLen + 2; // 기회(life)는 단어 개수 + 2로 정함.
		string strInput;
		//1 question
		while (true)
		{
			DrawStartGame(life, score, pastWord); // 게임 시작 화면 그리는 함수. 인자로 기회, 점수, 지난 단어를 받아 화면 그림.
			gotoxy(5, 5);
			for (int i = 0; i < originLen; ++i)
			{
				cout << strQuestion[i] << " "; // 단어 수 알려주기 위해 단어 수만큼 "_" 써줌. 배열이라 for문으로 써줌..
			}
			solve_or_out(strQuestion, originLen, 2);
			int pos_game = 2;
			while (1) {
				Sleep(100);
				int cnt=0;;
				int i = GetKeyDown();  // while문에 돌리면 화살표 입력시 한 번은 224로 넣고,자동으로 한번 더 넣는다. 넣는 값은 GetAsyncKeyState(방향) 반환 값과 같다.
				if (pos_game == 1) {
					if (GetAsyncKeyState(VK_RETURN)) { // 여기에 (GetAsyncKeyState(VK_UP)) || (GetAsyncKeyState(VK_DOWN)) || (GetAsyncKeyState(VK_LEFT)) || (GetAsyncKeyState(VK_RIGHT)) 이런 식으로 넣으면?
						pos_game = 2; // get 두번째값에 또 걸려서 2번씩 움직여지니 하면 안됨..
						solve_or_out(strQuestion, originLen, 2);
					}
					else { 
						gotoxy(9, 12);   // input 넣는 위치
						cin >> strInput;
						break; // 처음엔 i에 엔터 들어가는 경우, 위아래좌우 방향키 눌리는 경우 모두 제외
					}
				}
				else if (pos_game == 2) {
					if ((i == 224)) {
						pos_game = 1;
						solve_or_out(strQuestion, originLen, 1);
					}
					else if (GetAsyncKeyState(VK_RETURN) & 0x8000) return;
				}
			}
			
			pastWord.push_back(strInput);

			if (strInput.length() == 1) // input 길이가 한 자리인 경우(알파벳 넣었다면..)
			{
				//alphabet
				for (int i = 0; i < originLen; ++i)
				{
					if (strOriginal[i] == strInput[0])// 정답이 들어있는 strOriginal 과 비교해서
					{
						strQuestion[i] = strInput[0]; // '_'로 이루어진 strQuestion의 같은 알파벳있는 자리에 그 알파벳 넣고
					}
				}
			}
			else if (strInput.length() > 1) // 그외 단어를 선택했다면
			{
				//word
				if (strOriginal == strInput) // 정답이 맞다면
				{
					//score up !!
					score += 5;			// 점수 5점 주고
					pastWord.clear();   // pastWord 배열의 모든 값을 지운다.
					break;				// while문 끝냄.  line 167 while문 탈출이므로 기회 다시 채우고 게임 재시작.
				}
			}

			life -= 1;					// 틀렸다면 기회 -1
			if (life < 0)			// 기회가 0이하로 떨어졌다면
			{
				score -= 5;			// 점수 -5한다.
				if (score < 0)		// 점수가 이미 0점이라면 그냥 냅둠.
				{
					score = 0;
				}
				pastWord.clear();  // 기회 < 0 인 경우에도 pastWord 배열 비워줌.
				if (GameOver() == 0) return;
				break;             // while문 탈출. line 167 while문 탈출이므로 기회 다시 채우고 게임 재시작.
			}
		}
	}
}



//메인함수
int main(void)
{
	int POS = 1;
	system("COLOR 0F");
	SetConsoleView(); // SetConsoleView()는 콘솔창의 크기를 출력할 텍스트들의 크기에 맞게끔 조정하며, 제목줄에 타이틀이 출력할 수 있도록 설정해놓는다.	
	while (1) {
		DrawReadyGame(); // 게임 준비화면 그리는 함수.
		while (true)
		{	
			Sleep(100); // 컴퓨터 연산속도에 방향키,엔터 입력 속도 맞추는 용도. 
						// 여기 넣어야 컴퓨터 연산속도에 StartGame 끝내려고 qq 입력할 때,gameover 후 esc키 입력할 때도 연속 입력되는 것 막을 수 있음. 
			// int enter_press = GetAsyncKeyState(VK_RETURN);
			if (POS == 1) {
				if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(VK_UP) || GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState(VK_DOWN)) POS = 2; // GetAsyncKeyState(입력키) : 입력키가 호출 시점 이전에 눌렸었거나, 호출 시점 현재 눌려있는 상태인지 확인.
			}
			else {
				if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(VK_UP) || GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState(VK_DOWN)) POS = 1; 	
			}	
			if (GetAsyncKeyState(VK_RETURN) & 0X8000) {//엔터를 눌렀을 때
				break;
			}
			switch (POS) {  // POS : 현재 선택된 값을 나타내는 정수
			case 1:
				SetColor(11); // 선택된 값은 색상 11, 선택되지 않은 메뉴들은 색상 15로 설정
				gotoxy(9, 6); printf(" - 시작하기 -");
				SetColor(15);
				gotoxy(9, 7); printf(" - 게임종료 -");
				break; // case문 break. case문만 탈출.
			case 2:
				SetColor(11);
				gotoxy(9, 7); printf(" - 게임종료 -");
				SetColor(15);
				gotoxy(9, 6); printf(" - 시작하기 -");
				break;
			default: break;
			}
		}
		if (POS == 1) StartGame(); //게임시작. if isStart에 True 입력시..
		else {
			gotoxy(0, 11);
			break;
		}
	}
	return 0;
}
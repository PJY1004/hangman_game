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

// �ܼ� ���� Ư�� ��ġ�� Ŀ�� �̵���Ű�� �Լ�. �ش� ��ġ���� ���� ��½��� ���ϴ� �׸� �����.
void gotoxy(int x, int y)
{
	COORD Pos;
	Pos.X = 2 * x; // �׳� x�� �ϴ� �ͺ��� �׸� ���ڰ� ����� ����..
	Pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos); // ���� ��ġ�� Ŀ�� �ű�.
}

void SetColor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

//keyboard �Է�
int GetKeyDown()
{
	if (_kbhit() != 0) // _kbhit() : Ű���尡 ������ 0 �̿��� �� return
	{
		return _getch(); // _getch() : scanf ó�� �Է� �޴� ����̴�. �ٸ� scanf, cout ó�� �Է� ���� ���� ����ϱ� ���� ���� ����.
	}
	return 0;
}

//�ܼ� ����
void SetConsoleView()
{
	//system("mode con:cols=50 lines=20"); // �ܼ�â ������ ���� 
	system("title HANG MAN GAME");		// �ܼ�â ���� "HANG MAN GAME" ���� ����.
}

void CursorView(char show) // Ŀ�� ������ �ʰ�, ����� �Լ�.
{
	HANDLE hConsole; // �ü���� ���ҽ��� ��κ� �޸𸮷� �����Ǿ� �ּҸ� �����µ�, �� ���ҽ��� �������α׷��� ���� ��� ��
	// �ü���� ������ ������ �����ϰ� ������ �� ����. �׷��� ����� �������� ����Ǵ� ���� ���α׷��� �����ͷ� ����
	// �ü���� ���ҽ� �ּҿ� ������ �Ұ����ϴ�. ��� �ڵ��̶�� ������ ����Ѵ�. �ڵ��� �ü�� ������ ���ҽ� �ּҸ�
	// ������ ġȯ�� ���̴�. �׸��� ���ҽ� �ּҿ� �� �ڵ鰪�� �� ������ ��� �����ϴµ�, �̸� handle table�̶�� �Ѵ�.
	// ���� handle = os ���ο��� �����ϴ� �ڷᱸ�� ��� �����ϸ� ����.
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // console�� ����ϴ� Ŀ��DML handle(ID)�� �޾� hConsole�� ����.

	ConsoleCursor.bVisible = show; // .bVisible : 0 �Է� �� Ŀ�� �� ����, �׿� ���� �Է� �� ����.. 
	ConsoleCursor.dwSize = 1;  // .dwSize : Ŀ�� ����. 1~100 ���� ����

	SetConsoleCursorInfo(hConsole, &ConsoleCursor); // ������ �ܼ� ȭ�� ���ۿ� ���� Ŀ���� ũ��� ǥ�� ������ ����
}


//����ȭ�� draw
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
	cout << " - �����ϱ� -";
	gotoxy(9, 7);
	cout << " - �������� -";
	gotoxy(0, 10);
	cout << "�޴��� ENTERŰ�� ���ð����մϴ�.";

}

//����ȭ�� draw
void DrawStartGame(int life, int score, vector<string>& pastWord)
{
	system("cls"); // system("��ɾ�") : consoleâ���� cmd ��ɾ ����� �� �ְ� ����. �Ǵ� �����̸� �־ ���� ���൵ ����..
	gotoxy(5, 1);  // ���� ��� ��ɾ "explorer http://naver.com" ������ ���̹� ����..
	cout << "life = " << life;
	gotoxy(5, 2);
	cout << "score = " << score;
	gotoxy(5, 8);
	cout << "past = ";
	for (int i = 0; i < static_cast<int>(pastWord.size()); ++i) // ���� ó���� pastWord.size()=0 �̹Ƿ� ��ĭ ��µ�.
	{
		cout << pastWord[i] << " ";
	}

	gotoxy(5, 12);
	cout << "input = ";
	gotoxy(13, 14);
	cout << "������" << endl;
}

//������ �����ϴ� �Լ� �Դϴ�.
void SetDictionary(vector<string>& strArr)
{
	static const int INIT_NUM = 4;
	static const string str[INIT_NUM] = { "apple", "banana", "code", "dog" };	//���� �ܾ�.
	string dict_file = "words.txt";
	ifstream readFromFile(dict_file); // input file stream ����. ifstream readFromFile("words.txt") : ���� ���� ��θ� words.txt �� ������.
										// �ش� ������ readFromFile�� �������´�.
	if(!fs::exists(dict_file)) // std:filesystem::exists("���ϸ�") : �ش� ���� �����ϸ� 1 return , ���� x �� 0 return 
	{				// ������ ���ٸ�, ������ ���� �������״� �� if���� �ɸ�. words.txt ������ ���������.
		ofstream writeToFile(dict_file); // output file stream ����. ofstream writeToFile("���ϰ��") : �� ���� ��θ� ������. 
		for (int i = 0; i < INIT_NUM; ++i) // INIT_NUM = 4, 4ȸ �ݺ�.. ���� ���� �� line ����ŭ �ݺ���.
		{
			string tmp = str[i]; // tmp�� "apple"���� �ٸ� word ���� ���ʴ�� ����.
			if (i != INIT_NUM - 1) // �ܾ�� - 1(�ٹٲ� ��)�� ������ ������ �ٹٲ�("\n") ����.
			{
				tmp += "\n"; // string+"\n"���·� ����.������ �ܾ��..
			}
			writeToFile.write(tmp.c_str(), tmp.size());  // ��ü.write(�Է°�, �Է»�����) : ���Ͽ� ����(write) �Լ���.
				// tmp.c_str() : string class ���� string�� ��� char���� �迭�̴�. �׷��� .c_str() �ϸ� string ���� �ּҸ� ��ȯ��.
				// tmp.size() : string tmp�� size(ũ��)�� ��ȯ�Ѵ�. sizeof�� �޸� byte size�� �ƴ϶�.. ���ڿ��� ��� ����, ���ͳ� �迭�� ��� ��� ������ ��ȯ��. 
			strArr.push_back(str[i]); // strArr(string ����)�� str ���� ��� ��{ "apple", "banana", "code", "program" }�� �ִ´�.
		}
		writeToFile.close();	// ���� �ݱ�.
		return;
	}
	
	while (!readFromFile.eof()) // .eof() : �Լ� ����� ������ ù line���� ��� ���� line ����.. ���� ���� ������ 0 ��ȯ�ϴٰ� ���� ���� ������ 1 ��ȯ��.
	{
		string tmp;
		getline(readFromFile, tmp); // readFromFile�� �ٴ����� �о tmp�� ����.
		strArr.push_back(tmp);      // ����� �ܾ strArr ���Ϳ� ����.
	}
	readFromFile.close(); // ���� readFromFile�� �ݴ´�.
	return;
}

//����ȭ�� ���
bool ReadyGame()
{
	DrawReadyGame(); // ���� �غ�ȭ�� �׸��� �Լ�.
	while (true)
	{
		int key = GetKeyDown(); // Ű���尡 ������ ���� ���� int�� ���� key�� ����.
		if (key == 's' || key == 'S') // ��/�ҹ��� s�� ��� -> 1(True)��ȯ
		{
			return true;
		}
		else if (key == 'q' || key == 'Q') // // ��/�ҹ��� q�� ��� 0(false) ��ȯ.
		{
			return false;
		}
	}
}

int GameOver() {
	system("cls");
	cout << "GAME OVER\n";
	cout << "ESC �Է� �� ������ ����˴ϴ�.\n";
	cout << "������� ���� �׿��� �ƹ� Ű�� �Է����ּ���.^_^";
	int key;
	while (1) {					 // �Է� ���������� while�� ����.
		key =  GetKeyDown();	// key�� �Է¹���. _kbhit ���Ƿ� �Է� ���� �� ��� 0 return ��.
		if (key != 0) break;
	}
	if (key == 27) return 0; // esc ASCII CODE�� 27
	else return 1;
}

void solve_or_out(string word, int len_word, int mode){
	//draw question
	if (mode == 1) { // mode 1 �� ��� �Է�â�� ���õ�.
		SetColor(11);
		gotoxy(5, 12);
		cout << "input = ";

		SetColor(15);
		gotoxy(13, 14);
		cout << "������" << endl;
	}
	else if (mode == 2) { // mode 2�� ��� ����ȭ�� qq ���õ�.
		SetColor(11);
		gotoxy(13, 14);
		cout << "������" << endl;
		
		SetColor(15);
		gotoxy(5, 12);
		cout << "input = ";
		
	}
	
	return;
}

//���� ���� �Լ�.
void StartGame()
{
	int score = 0;
	vector<string> pastWord;
	vector<string> strArr;
	SetDictionary(strArr);		//"word.txt" ���Ͽ��� �ܾ� �о� strArr�� �����ϴ� �Լ�.

	while (true)	//�ϳ��� ź�� ǥ���ϴ� ����
	{
		//1 play
		int num = 0;
		srand((unsigned int)time(NULL));		
		// srand(seed��) : seed ���� ���� 0~32367 ���̿��� �Ѱ��� ���� �����ϰ� �����. 
		// (unsigned int)time(NULL) : 1970�� 1�� 1�� 0�� 0�� 0�ʷκ��� ���� �ð��� ��ȯ.
		num = rand() % static_cast<int>(strArr.size()); // strArr �迭�� ũ�⿡ ���� 0 ~ (�迭ũ�� - 1)�� ���� �������� num�� ����.
		// rand() : 0~32367(2byte �ִ�ǥ������)���� �Ѱ��� ���� �����ϰ� �����. �� 1���� ��� �����..
		// static_cast<int> : int������ ����ȯ�ϵ�, ����� ����ȯ[(int*)(double��������)] ���� ��쵵 �⵵�� �ڵ� �󿡼� ���� �����.
		string strQuestion;
		const string strOriginal = strArr[num];	//�迭 ũ�⿡ ���� �����ϰ� ���� �� num�� �´� �ܾ ����Ȯ�ο����� ����
		const int originLen = static_cast<int>(strOriginal.length());

		//init
		for (int i = 0; i < originLen; ++i) // "_"�� �ܾ� ����ŭ ����ϱ� ���� strOriginal.length�� �޾Ƽ� ���.
		{
			strQuestion += "_";
		}

		int life = originLen + 2; // ��ȸ(life)�� �ܾ� ���� + 2�� ����.
		string strInput;
		//1 question
		while (true)
		{
			DrawStartGame(life, score, pastWord); // ���� ���� ȭ�� �׸��� �Լ�. ���ڷ� ��ȸ, ����, ���� �ܾ �޾� ȭ�� �׸�.
			gotoxy(5, 5);
			for (int i = 0; i < originLen; ++i)
			{
				cout << strQuestion[i] << " "; // �ܾ� �� �˷��ֱ� ���� �ܾ� ����ŭ "_" ����. �迭�̶� for������ ����..
			}
			solve_or_out(strQuestion, originLen, 2);
			int pos_game = 2;
			while (1) {
				Sleep(100);
				int cnt=0;;
				int i = GetKeyDown();  // while���� ������ ȭ��ǥ �Է½� �� ���� 224�� �ְ�,�ڵ����� �ѹ� �� �ִ´�. �ִ� ���� GetAsyncKeyState(����) ��ȯ ���� ����.
				if (pos_game == 1) {
					if (GetAsyncKeyState(VK_RETURN)) { // ���⿡ (GetAsyncKeyState(VK_UP)) || (GetAsyncKeyState(VK_DOWN)) || (GetAsyncKeyState(VK_LEFT)) || (GetAsyncKeyState(VK_RIGHT)) �̷� ������ ������?
						pos_game = 2; // get �ι�°���� �� �ɷ��� 2���� ���������� �ϸ� �ȵ�..
						solve_or_out(strQuestion, originLen, 2);
					}
					else { 
						gotoxy(9, 12);   // input �ִ� ��ġ
						cin >> strInput;
						break; // ó���� i�� ���� ���� ���, ���Ʒ��¿� ����Ű ������ ��� ��� ����
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

			if (strInput.length() == 1) // input ���̰� �� �ڸ��� ���(���ĺ� �־��ٸ�..)
			{
				//alphabet
				for (int i = 0; i < originLen; ++i)
				{
					if (strOriginal[i] == strInput[0])// ������ ����ִ� strOriginal �� ���ؼ�
					{
						strQuestion[i] = strInput[0]; // '_'�� �̷���� strQuestion�� ���� ���ĺ��ִ� �ڸ��� �� ���ĺ� �ְ�
					}
				}
			}
			else if (strInput.length() > 1) // �׿� �ܾ �����ߴٸ�
			{
				//word
				if (strOriginal == strInput) // ������ �´ٸ�
				{
					//score up !!
					score += 5;			// ���� 5�� �ְ�
					pastWord.clear();   // pastWord �迭�� ��� ���� �����.
					break;				// while�� ����.  line 167 while�� Ż���̹Ƿ� ��ȸ �ٽ� ä��� ���� �����.
				}
			}

			life -= 1;					// Ʋ�ȴٸ� ��ȸ -1
			if (life < 0)			// ��ȸ�� 0���Ϸ� �������ٸ�
			{
				score -= 5;			// ���� -5�Ѵ�.
				if (score < 0)		// ������ �̹� 0���̶�� �׳� ����.
				{
					score = 0;
				}
				pastWord.clear();  // ��ȸ < 0 �� ��쿡�� pastWord �迭 �����.
				if (GameOver() == 0) return;
				break;             // while�� Ż��. line 167 while�� Ż���̹Ƿ� ��ȸ �ٽ� ä��� ���� �����.
			}
		}
	}
}



//�����Լ�
int main(void)
{
	int POS = 1;
	system("COLOR 0F");
	SetConsoleView(); // SetConsoleView()�� �ܼ�â�� ũ�⸦ ����� �ؽ�Ʈ���� ũ�⿡ �°Բ� �����ϸ�, �����ٿ� Ÿ��Ʋ�� ����� �� �ֵ��� �����س��´�.	
	while (1) {
		DrawReadyGame(); // ���� �غ�ȭ�� �׸��� �Լ�.
		while (true)
		{	
			Sleep(100); // ��ǻ�� ����ӵ��� ����Ű,���� �Է� �ӵ� ���ߴ� �뵵. 
						// ���� �־�� ��ǻ�� ����ӵ��� StartGame �������� qq �Է��� ��,gameover �� escŰ �Է��� ���� ���� �ԷµǴ� �� ���� �� ����. 
			// int enter_press = GetAsyncKeyState(VK_RETURN);
			if (POS == 1) {
				if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(VK_UP) || GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState(VK_DOWN)) POS = 2; // GetAsyncKeyState(�Է�Ű) : �Է�Ű�� ȣ�� ���� ������ ���Ⱦ��ų�, ȣ�� ���� ���� �����ִ� �������� Ȯ��.
			}
			else {
				if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(VK_UP) || GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState(VK_DOWN)) POS = 1; 	
			}	
			if (GetAsyncKeyState(VK_RETURN) & 0X8000) {//���͸� ������ ��
				break;
			}
			switch (POS) {  // POS : ���� ���õ� ���� ��Ÿ���� ����
			case 1:
				SetColor(11); // ���õ� ���� ���� 11, ���õ��� ���� �޴����� ���� 15�� ����
				gotoxy(9, 6); printf(" - �����ϱ� -");
				SetColor(15);
				gotoxy(9, 7); printf(" - �������� -");
				break; // case�� break. case���� Ż��.
			case 2:
				SetColor(11);
				gotoxy(9, 7); printf(" - �������� -");
				SetColor(15);
				gotoxy(9, 6); printf(" - �����ϱ� -");
				break;
			default: break;
			}
		}
		if (POS == 1) StartGame(); //���ӽ���. if isStart�� True �Է½�..
		else {
			gotoxy(0, 11);
			break;
		}
	}
	return 0;
}
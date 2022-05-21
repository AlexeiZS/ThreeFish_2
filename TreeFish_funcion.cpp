
#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
using namespace std;

typedef unsigned long u32;
typedef unsigned long long u64;
typedef unsigned char u8;

#define random(a,b)(a+rand()%b-a+1)
#define blok 64 
#define nr 72   
#define nw 8   

//возведение в степень.
int pow_16(int footing, int a) {
	int tmp = footing;
	if (a == 0) return 1;
	for (int i = 1; i < a; i++)
		footing *= tmp;
	return footing;
}
//проверка строки (адрес) или (хеш) "\".
int check_hash(string str) {
	for (int i = 0; i < size(str); i++)
		if (int(str[i]) == 92) return 1;
	return 0;
}

// форматирование + запись в файл.
void recordfile(string path, u64* ar) {
	ofstream of;
	of.open(path);
	if (of.is_open()) {
		for (int i = 0; i < nw; i++) {
			of << hex << ar[i];    // флаг(hex,oct,dec).
		}
	}
	else cout << path << " not found!" << endl;
	of.close();
}


//возвращает размер массива в байтах.
int sizeStr(string str) {
	int size = 0;
	for (int i = 0; str[i] != '\0'; i++) size++;
	return size;
}

// выводит содержимое строки.
void showStr(string path) {
	for (int i = 0; path[i] != '\0'; i++)
		cout << path[i];
	cout << endl;
}

//выводит массив.
void showArr(u64* ar, int size) {
	for (int i = 0; i < size; i++) {
		cout << ar[i] << " ";
	}
	cout << endl;
}

// выводит содержимое файла (текст).
void showFile(string path) {
	cout << "\t" << path << " : " << endl;
	string str;
	ifstream in(path);
	if (in.is_open()) {
		while (getline(in, str))
			cout << str << endl;
		cout << "size: " << sizeStr(str) << endl;
	}
	else cout << path << " not found!" << endl;
	in.close();
}

// size str
int sizeStr(char str[]) {
	int size = 0;
	for (int i = 0; str[i] != '\0'; i++)size++;
	return size;
}


// Threefish - kryptoalgoritm.
#include "ThreeFish_funcion.h"
#include <iostream>
#include <ctime>
#include <string>
#include <fstream>
#include <bitset>

using namespace std;

#define random(a,b)(a+rand()%b-a+1)
#define blok 64 
#define nr 72   
#define nw 200  //8    


typedef unsigned long u32;
typedef unsigned long long u64;
typedef unsigned char u8;

u64 sub_key[19][8]{};                //подключ для раундов.
u64 knw = 6148914691236517205L;
u64 t[3]{};                          //массив для хранения Tweek значений.
u64 subkeys[19][8]{};                //массив для Key раунда.
u32 r[8][4]{
	{38,30,50,53},
	{48,20,43,31},
	{34,14,15,27},
	{26,12,58,7},
	{33,49,8,42},
	{39,27,41,14},
	{29,26,11,9},
	{33,51,39,35} };

void mix(u64 x[], u32 r, u64 y[])
{
	y[0] = x[0] + x[1];  
	y[1] = (x[1] << r) | (x[1] >> (64 - r));// сдвиги и логоческое ИЛИ.
	y[1] ^= y[0];                           //XOR
}

void demix(u64 y[], u32 r, u64 x[])
{
	y[1] ^= y[0];
	x[1] = (y[1] >> r) | (y[1] << (64 - r));
	x[0] = y[0] - x[1];
}

//генератор псевдослучайных чисел.
void generation_key(u64* key) {
	u8 l,r,arr[255]{};
	for (int i = 0; i < 255; i++) {
		arr[i] = random(10, 1000);
		//cout << int(arr[i]) << " ";
	}
	for (int i = 0; i < nw; i++) {
		for (int j = 0;  j< nw; j++) {
			l = random(0, 254);
			r = random(0, 254);
			key[i]+= arr[l] ^= arr[r];
		}
	}
	//showArr(key,nw);//вывод
}


//инициализация ключа для decrypt.
int init_key(u64* key, string path_key)
{
	string str;
	if (check_hash(path_key)) {   //проверка наличия символа "\".
		ifstream in;
		in.open(path_key);
		if (in.is_open())
			while (getline(in, str));
		else
			cout << path_key << " not found!" << endl;
		in.close();
	}
	else if (size(path_key) == 24) str = path_key;
	else {
		cout << " error: не соответствует длина ключа !" << endl;
		return 1;
	}
		int footing = 16; //основание сист. счисления.
		for (int i = 0, value = 0, j = 0; i < nw; i++) {
			for (int k = 2; j < size(str); j++, k--) {
				if (k >= 0 && int(str[j]) < 58) 
					value += (int(str[j]) - 48) * pow_16(footing, k);
				else if (k >= 0 && int(str[j]) > 58) 
					value += (int(str[j]) - 87) * pow_16(footing, k);
				if (k == 0) {
					key[i] = value;
					k = 2;
					value = 0;
					j++;
					break;
				}
			}
		}
		/*cout << "key: ";///
		for (int i = 0; i < nw; i++)cout << key[i] << " ";///
		cout << endl;///*/
		return 0;
}

//запись результата kryptofunction в файл.
void result_record(string path, u64* result1) {
	string str;//
	ofstream of;
	of.open(path);
	if (of.is_open()) {
		of << result1;
	}
	else 
		cout << path << " not found!" << endl;
	of.close();
	
}

//инициализация массива данными из файла.
void readingFile(string path_sourse, u64* sourse_data) {
	string str;
	ifstream in;
	in.open(path_sourse,ios::in,ios::binary);
	if (in.is_open()) {
		while (getline(in, str));
		//str.push_back('\0');
	}
	else
		cout << path_sourse << " not found!" << endl;
	in.close();

	//memcpy(&sourse_data[0], &str, 64);

	for (int i = 0; i <size(str); i++) {
		sourse_data[i]= str[i];  
	}
}

void crypt(u64* v, u64* res1) {
	int round = 0;
	u64 f[200]{}; 
	u64 e[200]{};  
	u64 y[2]{};  
	u64 x[2]{};  
	u32 s;     
	for (round = 0; round < nr; round++)
	{
		if (round % 4 == 0) {
			s = round / 4;
			for (int i = 0; i < nw; i++)
				e[i] = v[i] + sub_key[s][i];
		}
		else {
			for (int i = 0; i < nw; i++)
				e[i] = v[i];
		}
		for (int i = 0; i < nw / 2; i++) {
			x[0] = e[i * 2];
			x[1] = e[i * 2 + 1];
			mix(x, r[round % 8][i], y);
			f[i * 2] = y[0];
			f[i * 2 + 1] = y[1];
		}
		for (int i = 0; i < nw; i++)
			v[i] = f[i];
	}
	for (int i = 0; i < nw; i++)
		res1[i] = v[i] + sub_key[nr / 4][i];

}

//расшифровывание.
void decrypt(u64* v, u64* res2) {
	int round = 0;
	u64 f[200]{};  //массив для хранения  блока 512байт (64 бит.эл.) после mix.
	u64 e[200]{};  //
	u64 y[2]{};  //
	u64 x[2]{};  // 
	u32 s;       //
	for (round = nr; round > 0; round--)
	{
		if (round % 4 == 0) {
			s = round / 4;
			for (int i = 0; i < nw; i++)
				f[i] = v[i] - sub_key[s][i];
		}
		else {
			for (int i = 0; i < nw; i++)
				f[i] = v[i];
		}
		for (int i = 0; i < nw; i++)
			e[i] = f[i];

		for (int i = 0; i < nw / 2; i++) {
			y[0] = e[i * 2];
			y[1] = e[i * 2 + 1];
			demix(y, r[(round - 1) % 8][i], x);
			v[i * 2] = x[0];
			v[i * 2 + 1] = x[1];
		}
	}
	for (int i = 0; i < nw; i++)
		res2[i] = v[i] - sub_key[0][i];
}

//инициализация субключа.
void init_sub_key(u64* K, u64* T) {
	u32  round;
	u64 key2[200]{};
	u64 knw = 6148914691236517205l; //целочисленная константа.

	for (int i = 0; i < nw; i++) {
		knw ^= K[i];
		key2[i] = K[i];
	}
	key2[8] = knw;
	t[0] = T[0];
	t[1] = T[1];
	t[2] = T[0] ^ T[1];

	//создание раундовых ключей.
	for (round = 0; round <= nr / 4; round++) {
		for (int i = 0; i < nw; i++) { //добавление tweek значений.
			sub_key[round][i] = key2[(round + i) % (nw + 1)];
			if (i == nw - 3)
				sub_key[round][i] += t[round % 3];
			else if (i == nw - 2)
				sub_key[round][i] += t[(round + 1) % 3];
			else if (i == nw - 1)
				sub_key[round][i] += round;
		}
	}
}


////шифрование.
////пинимает указатели блока для шифра и массива приемник для шифроблока.
//void crypt(u64* data, u64* res1) {
//	int round = 0;
//	u64 f[8]{};  //массив для хранения  блока 512байт (64 бит.эл.) после mix.
//	u64 e[8]{};  //
//	u64 y[2]{};  //
//	u64 x[2]{};  // 
//	u64 v[8]{};  //
//	u32 s;     // (32 бита)
//	memcpy(&v[0], data, 64); //копирует 64 байта из data в массив v.
//	for (round = 0; round < nr; round++)
//	{
//		if (round % 4 == 0){
//			s = round / 4;
//			for (int i = 0; i < nw; i++)
//				e[i] = v[i] + sub_key[s][i];
//		}
//		else {
//			for (int i = 0; i < nw; i++)
//				e[i] = v[i];
//		}
//		for (int i = 0; i < nw / 2; i++){
//			x[0] = e[i * 2];
//			x[1] = e[i * 2 + 1];
//			mix(x, r[round % 8][i], y);
//			f[i * 2] = y[0];
//			f[i * 2 + 1] = y[1];
//		}
//		for (int i = 0; i < nw; i++) 
//			v[i] = f[p[i]];
//	}
//	for (int i = 0; i < nw; i++) 
//		res1[i] = v[i] + sub_key[nr / 4][i];
//	
//}

////расшифровывание.
//void decrypt(u64* data, u64* res2) {
//	int round = 0;
//	u64 f[8]{};  //массив для хранения  блока 512байт (64 бит.эл.) после mix.
//	u64 e[8]{};  //
//	u64 y[2]{};  //
//	u64 x[2]{};  // 
//	u64 v[8]{};  //
//	u32 s;       //
//	memcpy(&v[0], data, 64); //копирует 64 байта из data в массив v.
//	for (round = nr; round > 0; round--)
//	{
//		if (round % 4 == 0){
//			s = round / 4;
//			for (int i = 0; i < nw; i++)
//				f[i] = v[i] - sub_key[s][i];
//		}
//		else{
//			for (int i = 0; i < nw; i++)
//				f[i] = v[i];
//		}
//		for (int i = 0; i < nw; i++)
//			e[i] = f[p1[i]];
//
//		for (int i = 0; i < nw / 2; i++){
//			y[0] = e[i * 2];
//			y[1] = e[i * 2 + 1];
//			demix(y, r[(round - 1) % 8][i], x);
//			v[i * 2] = x[0];
//			v[i * 2 + 1] = x[1];
//		}
//	}
//	for (int i = 0; i < nw; i++)
//		res2[i] = v[i] - sub_key[0][i];
//}

////инициализация субключа.
//void init_sub_key(u64* key, u64* tweek) {
//	u32  round;
//	u64 k[8]{};
//	u64 t[2]{};
//	u64 key2[9]{};
//	u64 knw = 6148914691236517205l; //целочисленная константа.
//	memcpy(&k[0], &key[0], 64);
//	memcpy(&t[0], &tweek, 16);
//	for (int i = 0; i < nw; i++){
//		knw ^= k[i];
//		key2[i] = k[i];
//	}
//	key2[8] = knw;
//	t[0] = t[0];
//	t[1] = t[1];
//	t[2] = t[0] ^ t[1];
//
//	//создание раундовых ключей.
//	for (round = 0; round <= nr / 4; round++){
//		for (int i = 0; i < nw; i++){ //добавление tweek значений.
//			sub_key[round][i] = key2[(round + i) % (nw + 1)];
//			if (i == nw - 3)
//				sub_key[round][i] += t[round % 3];
//			else if (i == nw - 2)
//				sub_key[round][i] += t[(round + 1) % 3];
//			else if (i == nw - 1)
//				sub_key[round][i] += round;
//		}
//	}
//}


int main()
{
	setlocale(LC_ALL, "ru");
	srand(time(NULL));
    
	cout << endl << "\tThreefish" << endl;
	cout << " Симетричный, блочный, криптоалгоритм." << endl;

	//u64 key_data[nw]{};     //ключ (8 байт 1 эл.)( общ.512 бит).
	//u64  tweek[2]{0x243f6a8885a308d3,0x13198a2e03707344}; // доп. параметр (128 бит).
	//u64 sourse_data[nw];          //исходные данные(1 блок 64 байта).
	//u64 result1[nw]{};      
	//u64 result2[nw]{};     


	u64 key_data[200]{};     //ключ (8 байт 1 эл.)( общ.512 бит).
	u64  tweek[2]{ 12380474345028,38624651632845 }; // доп. параметр (128 бит).         
	u64 sourse_data[200]{}; ///??? 
	u64 result1[200]{};
	u64 result2[200]{};

	char* new_path = new char[] {"files\\.txt"};//////
	string path_sourse = "Threefish\\Sourstext.txt";  //адрес удалить!!!////
	string path_key = "Threefish\\Key.txt";
	string path_decod = "Threefish\\Decod.txt";
	string path_cod = "Threefish\\Codtext.txt";
	string path = " ";                                //для user ввода.???

	u32 x1;
	char x2;
	bool flag = false;
	do {
		cout << endl;
		cout << " Желаете зашифровать файл, введите < 1 > " << endl;
		cout << " Желаете раcшифровать файл, введите < 0 > " << endl;
		cin >> x1;
		do {
			switch (x1)
			{
			case 1:
				cout << " Введите адрес файла для шифрования." << endl;
				cout<< " Например: Threefish\\Sourstext.txt" << endl;
				cin >> path_sourse;
				showFile(path_sourse);                 // вывод.
				readingFile(path_sourse, sourse_data); // инициализация массива данными.
	            generation_key(key_data);              // генерация ключа.
	            recordfile(path_key, key_data);        // запись key в файл.
	            showFile(path_key);                    // вывод key files;////
			    init_sub_key(key_data,tweek);          // генерация раундового ключа.
				crypt(sourse_data, result1);           // шифрование.
				result_record(path_cod,result1);       // запись результата шифр.в файл. 
				showFile(path_cod);                    // вывод в консоль шифротекста.
				break;
			case 0: 
				cout << " Введите адрес файла для расшифрования." << endl;
				cout << " Например: Threefish\\Сodtext.txt" << endl;
				cin >>path_cod;
				cout << " Введите адрес файла с ключём или ключ." << endl;
				cout << " Например: Threefish\\Key.txt" << endl;
				cin >> path_key;
				if (!init_key(key_data, path_key)) {//инициализация key для decrypt.
					cout << "Ok!" << endl;///
				    readingFile(path_sourse, sourse_data); // инициализация массива данными.
				    decrypt( sourse_data,  result2);       // расшифровывание.
			        result_record(path_decod,result2);     // запись результата шифр.в файл.
					showFile(path_decod);                  // вывод в консоль.
				}
				break;
			default: cout << " error! (повторите выбор:)" << endl;
				flag = true;
			}
		} while (flag);
		cout << endl << " Желаете выйти из системы криптоалгоритма, введите: 'y', продолжить: 'n'";
		cin >> x2;
	} while (x2 == 'n');

	
	//showFile(path_sourse);            // выводит содержимое файла в консоль
	//showArr(key_data, nw);



	/*u64 a = 0x243f6a8885a308d3;
	cout << " a : " << a << endl;
	cout << uppercase << hex << a << endl;*/




	return 0;
}


















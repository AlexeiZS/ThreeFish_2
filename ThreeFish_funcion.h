//#pragma once
#include <iostream>
#include <string>
using namespace std;
typedef unsigned long u32;
typedef unsigned long long u64;
typedef unsigned char u8;

void showFile(string path);//вывод в консоль.
void showArr(u64* ar, int size);
int sizeStr(char str[]);
void recordfile(string path, u64* ar);
int check_hash(string str);
int pow_16(int footing, int a);
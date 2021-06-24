#pragma once
#include<iostream>
#include<map>
#include<vector>
#include<stdio.h>
#include<cmath>
#include<cstdlib>
#include<algorithm>
#include<fstream>

 

using namespace std;
typedef int tLabel;
typedef double tData;
typedef pair<int, double>  PAIR;
const int colLen = 2;
const int rowLen = 35;
const int Class1 = 7;
const int clCount1 = 5;
const int Class2 = 12;
const int clCount2 = 1;
 
class KNN
{
private:
	//tData dataSet[rowLen][colLen];
	tLabel labels[rowLen];
	//tData testData[colLen];
	int k;
	map<int, double> map_index_dis;
	map<tLabel, int> map_label_freq;
	//double get_distance(tData *d1, tData *d2);
	
public:

	KNN(int k);
	double get_distance(int i, int j,vector<vector<double>>matrix1, vector<vector<double>>matrix2);
	void get_all_distance(vector<vector<double> > matrix1);
	void get_all_distance_static(vector<vector<double> > matrix1);
	double get_all_distance_state(vector<vector<double>> matrix1);
	double get_all_distance_key(int typ, vector<vector<double>> matrix1);
	int get_max_freq_label();
	void get_d_gesture_name(int i);
	void get_s_gesture_name(int i);
	struct CmpByValue
	{
		bool operator() (const PAIR& lhs, const PAIR& rhs)
		{
			return lhs.second < rhs.second;
		}
	};

};





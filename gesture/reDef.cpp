/******************************************************************************\
* Copyright (C) 2012-2016 Leap Motion, Inc. All rights reserved.               *
* Leap Motion proprietary and confidential. Not for distribution.              *
* Use subject to the terms of the Leap Motion SDK Agreement available at       *
* https://developer.leapmotion.com/sdk_agreement, or another agreement         *
* between Leap Motion and you, your company or other organization.             *
\******************************************************************************/



#include <string>

#include "knn.h"
#include <vector>
#include <list>
#include <map>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include "time.h"
//#include "ts.h"
//#include "dtw.h"
#include "ut.h"
#include <opencv2/opencv.hpp>
#include <iostream>
//#include <cstring>
//#include<stdio.h>
#include <windows.h>
#include "SampleListener.h"

using namespace std;
using namespace cv;
std::vector<cv::Mat> std_set;
std::vector<cv::Mat> state_set;
std::vector<std::vector<double>> key_set;
double nine_angle;
double one_angle;
double yaw_3;
double yaw_4;
double thumb_2;
double thumb_6;
void get_std_set(std::vector<cv::Mat>&vec1, std::vector<cv::Mat>&vec2, std::vector<std::vector<double>> &vec3)
{

	for (int i = 1; i <= Class1; i++)
	{
		for (int j = 1; j <= clCount1; j++)
		{
			char url_str[50];
			sprintf(url_str, ".\\traindata\\new_dynamic\\gesture%d_%d.csv", i, j);
			Ptr<ml::TrainData> train_data;
			train_data = ml::TrainData::loadFromCSV(url_str, 0, -2, 0);//first 0 is skip how much row

			vec1.push_back(train_data->getSamples());

			//cout << i;

		}
	}

	for (int i = 0; i < Class2; i++)
	{
		char url_str[50];
		sprintf(url_str, ".\\traindata\\new_static\\gesture%d_%d.csv", i, i);
		Ptr<ml::TrainData> train_data;
		train_data = ml::TrainData::loadFromCSV(url_str, 0, -2, 0);//first 0 is skip how much row
		vec2.push_back(train_data->getSamples());

	}
	for (int i = 0; i < 5; i++)
	{
		char url_str1[50];
		sprintf(url_str1, ".\\traindata\\key_frame\\gesture%d_%d.csv", i+1, i+1);
		ifstream fp(url_str1); //定义声明一个ifstream对象，指定文件路径
		string line;
		//getline(fp,line); //跳过列名，第一行不做处理
		getline(fp, line);
		//循环读取每行数据
		vector<double> data_line;
		string number;
		istringstream readstr(line); //string数据流化
	//将一行数据按'，'分割

		for (int j = 0; j <= 11; j++)
		{ //可根据数据的实际情况取循环获取
			getline(readstr, number, ','); //循环读取数据
			data_line.push_back(atof(number.c_str())); //字符串传int
		}
		vec3.push_back(data_line); //插入到vector中	
		//std::vector<std::vector<double>> vec5 = vec3;
	}
	
	
}
double KNN::get_distance(int i, int j, cv::Mat NewSamples, cv::Mat stdSamples)
{
	//char url_str[50];
	double arr[12];
	double weight[12];
	double dtwDistance = 0;
	double AdtwDistance = 0;

	//sprintf(url_str, ".\\gesture%d_%d.csv", i, j);
	//Ptr<ml::TrainData> train_data;
	//train_data = ml::TrainData::loadFromCSV(url_str, 0, -2, 0);//first 0 is skip how much row
	//cv::Mat stdSamples = train_data->getSamples();
	////cv::Mat C = m.colRange(0,1).clone();
	////std::cout << C.t() <<std::endl;

	for (int p = 0; p < stdSamples.cols; p++)
	{
	
		cv::Mat Ct1 = stdSamples.colRange(p, p + 1).clone().t();
		cv::Mat Ct2 = NewSamples.colRange(p, p + 1).clone().t();
		if (p==7)
		{
			Ct1 *= 80;
			Ct2 *= 80;

		}
		if (p >= 3 && p <= 6)
		{
			Ct1 *= 80;
			Ct2 *= 80;
		}
		if(p>=10 && p<=11)
		{
			Ct1 *= 80;
			Ct2 *= 80;
		}
		std::vector<double> series1(Ct1); // get first col data
		std::vector<double> series2(Ct2);

		//// max-min convert
		//double max1 = *max_element(series1.begin(), series1.end());
		//double min1 = *min_element(series1.begin(), series1.end());
		//for (vector<double>::iterator iter1 = series1.begin(); iter1 != series1.end(); iter1++)
		//{
		//	*iter1 = (*iter1 - min1) / (max1 - min1);
		//}
		//double max2 = *max_element(series2.begin(), series2.end());
		//double min2 = *min_element(series2.begin(), series2.end());
		//for (vector<double>::iterator iter2 = series2.begin(); iter2 != series2.end(); iter2++)
		//{
		//	*iter2 = (*iter2 - min2) / (max2 - min2);
		//}

		UnivariateTimeSeries ts1(series1);
		const UnivariateTimeSeries ts2(series2);
		
		arr[p] = TSdist::computeNormalizedDTW(ts1, ts2, 1, 2);
		//arr[p] = TSdist::computeDTW(ts1, ts2, 1, 2, 2);
		//cout << arr[p] << endl;
		//cout << "cDTW distance is: " << TSdist::computeDTW(ts1, ts2, 1, 2, 2) << endl;
		dtwDistance +=arr[p];
		//cout << arr[p] << endl;
		//cout << "nDTW distance is: " << TSdist::computeNormalizedDTW(ts1, ts2, 1, 2) << endl;
		
	}
	if (i == 1)
	{
		one_angle = arr[4];
	}
	if (i == 9)
	{
		nine_angle = arr[4];
	}
	if (i == 3)
	{
		yaw_3 = arr[7];
	}
	if (i == 4)
	{
		yaw_4 = arr[7];
	}
	if (i == 2)
	{
		thumb_2 = arr[0];
	}
	if (i == 6)
	{
		thumb_6 = arr[0];
	}
	//cout << i << " " << dtwDistance << endl;
	AdtwDistance = dtwDistance / 12;
	//double di = 0;
	////double all_weight = 0;
	//for (int m = 0; m <= 11; ++m)
	//{
	//	di += (1 - exp(-((dtwDistance-arr[m]) / dtwDistance)));
	//}
	//for (int n = 0; n <= 11; ++n)
	//{
	//	weight[n] = (1 - exp(-((dtwDistance-arr[n]) / dtwDistance))) / di;
	//	
	//	//all_weight += weight[n];
	//	//cout << n << " " << weight[n] << " " << weight[n] * arr[n] << endl;
	//	AdtwDistance += weight[n] * arr[n];
	//}
	////cout << all_weight<< endl;
	//cout << i << " " << AdtwDistance << endl<<endl;

	return AdtwDistance;
}

double KNN::get_state_distance(vector<vector<double> > matrix)
{
	double tmpdistance;
	cv::Mat NewSamples(0, matrix[0].size(), cv::DataType<double>::type);
	for (unsigned int i = 0; i < matrix.size(); ++i)
	{
		// Make a temporary cv::Mat row and add to NewSamples _without_ data copy
		cv::Mat Sample(1, matrix[0].size(), cv::DataType<double>::type, matrix[i].data());

		NewSamples.push_back(Sample);
	}
	tmpdistance = get_distance(0, 0, NewSamples, state_set[0]);
	//if (tmpdistance <= 80)
	if (tmpdistance <=2.0)
	{
		cout << "监测到起始手势，请做好手势准备" << endl;
		//cout << "3s" << endl;
		//waitKey(1000);
		//cout << "2s" << endl;
		//waitKey(1000);
		//cout << "1s" << endl;
		//waitKey(1000);
		cout << "开始" << endl;
		return 1;
		
	}
	else 
	{ 
		cout << "持续监测起始手势中" << endl;
		return 0; 
	}
}

double KNN::get_key_distance(vector<double> vec)
{
	double AdtwDistance[5];
	//char url_str[50];
	double weight[12];
	
	for (int i = 0; i <= 4; i++)
	{
		double arr[12];
		double dtwDistance = 0;
		for (int j = 0; j <= 11; j++)
		{
			std::vector<double> series1; // get first col data
			series1.push_back(key_set[i][j]);
			std::vector<double> series2;
			series2.push_back(vec[j]);
			if (j == 7||j==6)
			{
				series1[0] *= 0;
				series2[0] *= 0;

			}
			if (j >= 3 && j <= 5)
			{
				series1[0] *= 60;
				series2[0] *= 60;
			}
			if (j >= 10 && j <= 11)
			{
				series1[0] *= 60;
				series2[0] *= 60;
			}
			UnivariateTimeSeries ts1(series1);
			const UnivariateTimeSeries ts2(series2);

			arr[j] = TSdist::computeNormalizedDTW(ts1, ts2, 1, 2);
			//arr[p] = TSdist::computeDTW(ts1, ts2, 1, 2, 2);
			//cout << arr[p] << endl;
			//cout << "cDTW distance is: " << TSdist::computeDTW(ts1, ts2, 1, 2, 2) << endl;
			dtwDistance += arr[j];
			//cout <<i<<" "<< arr[j] << endl;
		}
		AdtwDistance[i] = dtwDistance / 10;
		//cout << endl;
	}
	//cout << 1 << " " << AdtwDistance[0] << endl;
	//cout << 2 << " " << AdtwDistance[1] << endl;
	//cout << 3 << " " << AdtwDistance[2] << endl;
	//cout << 4 << " " << AdtwDistance[3] << endl;
	//cout << 5 << " " << AdtwDistance[4] << endl;
	//cout << vec[0] << " " << vec[1] << endl;
	double result = *min_element(AdtwDistance, AdtwDistance + 5);
	double result1 = AdtwDistance[3] == result ? 10 : result;
	//cout << result1 << endl; 
	//cout << endl;
	return result1;
	//double result = AdtwDistance[0] > AdtwDistance[1] ? AdtwDistance[1] : AdtwDistance[0];	
}

double KNN::get_start_distance(vector<double> vec)
{
	double AdtwDistance = 0;
	//char url_str[50];


	double arr[12];
	double dtwDistance = 0;
	for (int j = 0; j <= 11; j++)
	{
		std::vector<double> series1; // get first col data
		series1.push_back(key_set[3][j]);
		std::vector<double> series2;
		series2.push_back(vec[j]);
		if (j == 7 || j == 6)
		{
			series1[0] *= 0;
			series2[0] *= 0;

		}
		if (j >= 3 && j <= 5)
		{
			series1[0] *= 80;
			series2[0] *= 80;
		}
		if (j >= 10 && j <= 11)
		{
			series1[0] *= 80;
			series2[0] *= 80;
		}
		UnivariateTimeSeries ts1(series1);
		const UnivariateTimeSeries ts2(series2);

		arr[j] = TSdist::computeNormalizedDTW(ts1, ts2, 1, 2);
		//arr[p] = TSdist::computeDTW(ts1, ts2, 1, 2, 2);
		//cout << arr[p] << endl;
		//cout << "cDTW distance is: " << TSdist::computeDTW(ts1, ts2, 1, 2, 2) << endl;
		dtwDistance += arr[j];
		//cout <<i<<" "<< arr[j] << endl;
	}
	AdtwDistance = dtwDistance / 10;
	//cout << endl;
	
	//cout << 1 << " " << AdtwDistance[0] << endl;
	//cout << 2 << " " << AdtwDistance[1] << endl;
	//cout << vec[0] << " " << vec[1] << endl;
	//cout << AdtwDistance << endl;
	return AdtwDistance;
}

void KNN::get_all_distance(vector<vector<double> > matrix1)
{

	double tempdistance;

	cv::Mat NewSamples(0, matrix1[0].size(), cv::DataType<double>::type);
	for (unsigned int i = 0; i < matrix1.size(); ++i)
	{
		// Make a temporary cv::Mat row and add to NewSamples _without_ data copy
		cv::Mat Sample(1, matrix1[0].size(), cv::DataType<double>::type, matrix1[i].data());

		NewSamples.push_back(Sample);
	}


	int temp = 0;
	for (int i = 1; i <= Class1; i++)
	{
		for (int j = 1; j <= clCount1; j++)
		{
			tempdistance = get_distance(i, j, NewSamples, std_set[temp]);

						
			if (tempdistance >= 111)
			{
				map_index_dis[temp] = tempdistance;
				labels[temp] = i;
				temp+=(clCount1+1-j);;   // clCount+1-j
				break;
			}
			map_index_dis[temp] = tempdistance;
			labels[temp] = i;
			temp++;;
		}

	}

	
}

void KNN::get_all_distance_static(vector<vector<double> > matrix1)
{

	double tempdistance;

	cv::Mat NewSamples(0, matrix1[0].size(), cv::DataType<double>::type);
	for (unsigned int i = 0; i < matrix1.size(); ++i)
	{
		// Make a temporary cv::Mat row and add to NewSamples _without_ data copy
		cv::Mat Sample(1, matrix1[0].size(), cv::DataType<double>::type, matrix1[i].data());

		NewSamples.push_back(Sample);
	}


	int temp = 0;
	for (int i = 1; i < Class2; i++)
	{
		tempdistance = get_distance(i, i, NewSamples, state_set[temp+1]);	
		map_index_dis[temp] = tempdistance;
		labels[temp] = i;
		temp++;;
	}	

}

void KNN::get_s_gesture_name(int i)
{
	switch (i)
	{
	case 1:
		if (nine_angle > one_angle)
		{
			cout << "数字1" << endl;
		}
		else
		{ 
			cout << "数字9" << endl;
		}
		break;
	case 2:
		cout << "数字2" << endl;
		break;
	case 3:
		cout << "数字3" << endl;
		break;
	case 4:
		cout << "数字4" << endl;
		break;
	case 5:
		cout << "数字5" << endl;
		break;
	case 6:
		cout << "数字6" << endl;
		break;
	case 7:
		cout << "数字7" << endl;
		break;
	case 8:
		cout << "数字8" << endl;
		break;
	case 9:
		cout << "数字9" << endl;	
		break;
	case 10:
		cout << "确定" << endl;
		break;
	case 11:
		cout << "数字9" << endl;
		break;
	default:
		break;
	}
}

void KNN::get_d_gesture_name(int i)
{
	switch (i)
	{
	case 1:
		cout << "前进" << endl;
		break;
	case 2:
		cout << "后退" << endl;
		break;
	case 3: case 4:
		if(yaw_3 < yaw_4)
		cout << "左转" << endl;
		else
		cout << "右转" << endl;
		break;
	case 5:
		cout << "捏" << endl;	
		break;
	case 6:
		cout << "抓" << endl;
		break;
	case 7:
		cout << "扔" << endl;
		break;
	default:
		break;
	}
}

SampleListener::SampleListener()
{

}

SampleListener::~SampleListener()
{
}

void SampleListener::onInit(const Controller& controller) {
	std::cout << "Initialized" << std::endl;
}

void SampleListener::onConnect(const Controller& controller) {
	std::cout << "Connected" << std::endl;
}

void SampleListener::onDisconnect(const Controller& controller) {
	// Note: not dispatched when running in a debugger.
	std::cout << "Disconnected" << std::endl;
}

void SampleListener::onExit(const Controller& controller) {
	std::cout << "Exited" << std::endl;
}









void SampleListener::onFocusGained(const Controller& controller) {
	std::cout << "Focus Gained" << std::endl;
}

void SampleListener::onFocusLost(const Controller& controller) {
	std::cout << "Focus Lost" << std::endl;
}

void SampleListener::onDeviceChange(const Controller& controller) {
	std::cout << "Device Changed" << std::endl;
	const DeviceList devices = controller.devices();

	for (int i = 0; i < devices.count(); ++i) {
		std::cout << "id: " << devices[i].toString() << std::endl;
		std::cout << "  isStreaming: " << (devices[i].isStreaming() ? "true" : "false") << std::endl;
		std::cout << "  isSmudged:" << (devices[i].isSmudged() ? "true" : "false") << std::endl;
		std::cout << "  isLightingBad:" << (devices[i].isLightingBad() ? "true" : "false") << std::endl;
	}
}

void SampleListener::onServiceConnect(const Controller& controller) {
	std::cout << "Service Connected" << std::endl;
	
	get_std_set(std_set, state_set, key_set);
	//cout << key_set.size() << endl;
	//std::cout << "5s" << std::endl;
	////Sleep(1000);
	////std::cout << "4s" << std::endl;
	////Sleep(1000);
	////std::cout << "3s" << std::endl;
	////Sleep(1000);
	//std::cout << "2s" << std::endl;
	//Sleep(1000);
	//std::cout << "1s" << std::endl;
	//Sleep(1000);
	//std::cout << "开始" << std::endl;

	//Ptr<ml::TrainData> train_data; 
	//train_data = ml::TrainData::loadFromCSV(".\\data.csv", 0,-2,0);//first 0 is skip how much row
	//cv::Mat m = train_data->getSamples().colRange(0,1).clone();	
	////cv::Mat C = m.colRange(0,1).clone();
	////std::cout << C.t() <<std::endl;
	//cv::Mat Ct = m.t();
	//std::vector<float> c(Ct); // get first col data

}

void SampleListener::onServiceDisconnect(const Controller& controller) {
	std::cout << "Service Disconnected" << std::endl;
}

void SampleListener::onServiceChange(const Controller& controller) {
	std::cout << "Service Changed" << std::endl;
}

void SampleListener::onDeviceFailure(const Controller& controller) {
	std::cout << "Device Error" << std::endl;
	const Leap::FailedDeviceList devices = controller.failedDevices();

	for (FailedDeviceList::const_iterator dl = devices.begin(); dl != devices.end(); ++dl) {
		const FailedDevice device = *dl;
		std::cout << "  PNP ID:" << device.pnpId();
		std::cout << "    Failure type:" << device.failure();
	}
}

void SampleListener::onLogMessage(const Controller&, MessageSeverity s, int64_t t, const char* msg) {
	switch (s) {
	case Leap::MESSAGE_CRITICAL:
		std::cout << "[Critical]";
		break;
	case Leap::MESSAGE_WARNING:
		std::cout << "[Warning]";
		break;
	case Leap::MESSAGE_INFORMATION:
		std::cout << "[Info]";
		break;
	case Leap::MESSAGE_UNKNOWN:
		std::cout << "[Unknown]";
	}
	std::cout << "[" << t << "] ";
	std::cout << msg << std::endl;
}


//
// dtw_distance_only.cpp
//
// Copyright (c) 2019 Charles Jekel
// MIT licensed
//

#define _WIN32_WINNT 0x0400			// avoid winsock problem
#include "knn.h"
#include <DTW.hpp>
#include <math.h>
#include <sstream>
#include "time.h"
#include "assertion-macros.h"
#include <vhtBase.h>


#define PI acos(-1)
std::vector<std::vector<std::vector<double>>> std_set;
std::vector<std::vector<std::vector<double>>> state_set;
std::vector<std::vector<std::vector<double>>> key_set;
double yaw_3,yaw_4;
using namespace std;
template<typename T>
std::vector<double> linspace(T start_in, T end_in, int num_in)
{
  // source for this template from user Akavall
  // https://stackoverflow.com/questions/27028226/python-linspace-in-c
	std::vector<double> linspaced;

	double start = static_cast<double>(start_in);
	double end = static_cast<double>(end_in);
	double num = static_cast<double>(num_in);

	if (num == 0) { return linspaced; }
	if (num == 1) 
    {
		  linspaced.push_back(start);
		  return linspaced;
    }

	double delta = (end - start) / (num - 1);

	for(int i=0; i < num-1; ++i)
    {
		linspaced.push_back(start + delta * i);
    }
	linspaced.push_back(end); // I want to ensure that start and end
                            // are exactly the same as the input
	return linspaced;
}


vector<vector<double>> transpose(vector<vector<double>>& A)
{
    int leny=A[0].size();
    int lenx=A.size();
    vector<vector<double>>v(leny,vector<double>());
    if(A.empty())return vector<vector<double>>();
    for(int i=0;i<lenx;i++)
        for(int j=0;j<leny;j++){
            v[j].push_back(A[i][j]);
            
        }
    return v;

}

void load_csv_k(std::vector<std::vector<std::vector<double>>> &vec1){
    
	for (int i = 0; i <= 4; i++)
	{
		for (int j = 1; j <= 1; j++)
		{
			vector<vector<double>> user_arr;
			char url_str[50];
			sprintf(url_str, "./traindata/key/gesture%d_%d.csv", i, i);
			ifstream fp(url_str); //定义声明一个ifstream对象，指定文件路径
			string line;
		  //getline(fp,line); //跳过列名，第一行不做处理
			while (getline(fp,line))
			{ //循环读取每行数据
				vector<double> data_line;
				string number;
				istringstream readstr(line); //string数据流化
        //将一行数据按'，'分割
	
				for(int j = 0;j <= 22;j++)
				{ //可根据数据的实际情况取循环获取
					getline(readstr,number,','); //循环读取数据
					data_line.push_back(atof(number.c_str())); //字符串传int
				} 
				user_arr.push_back(data_line); //插入到vector中	
				
			}		
			user_arr=transpose(user_arr);
			vec1.push_back(user_arr);
		}
	}
	//cout<< std_set[34][17][4]<<endl;
}

void load_csv(std::vector<std::vector<std::vector<double>>> &vec1){
    
	for (int i = 0; i <= 10; i++)
	{
		for (int j = 1; j <= 1; j++)
		{
			vector<vector<double>> user_arr;
			char url_str[50];
			sprintf(url_str, "./traindata/static/gesture%d_%d.csv", i, i);
			ifstream fp(url_str); //定义声明一个ifstream对象，指定文件路径
			string line;
		  //getline(fp,line); //跳过列名，第一行不做处理
			while (getline(fp,line))
			{ //循环读取每行数据
				vector<double> data_line;
				string number;
				istringstream readstr(line); //string数据流化
        //将一行数据按'，'分割
	
				for(int j = 0;j <= 22;j++)
				{ //可根据数据的实际情况取循环获取
					getline(readstr,number,','); //循环读取数据
					data_line.push_back(atof(number.c_str())); //字符串传int
				} 
				user_arr.push_back(data_line); //插入到vector中	
				
			}		
			user_arr=transpose(user_arr);
			vec1.push_back(user_arr);
		}
	}
	//cout<< std_set[34][17][4]<<endl;
}
void load_csv_d(std::vector<std::vector<std::vector<double>>> &vec1){
    
	for (int i = 1; i <= Class1; i++)
	{
		for (int j = 1; j <= clCount1; j++)
		{
			vector<vector<double>> user_arr;
			char url_str[50];
			sprintf(url_str, "./traindata/dynamic/gesture%d_%d.csv", i, j);
			ifstream fp(url_str); //定义声明一个ifstream对象，指定文件路径
			string line;
		  //getline(fp,line); //跳过列名，第一行不做处理
			while (getline(fp,line))
			{ //循环读取每行数据
				vector<double> data_line;
				string number;
				istringstream readstr(line); //string数据流化
        //将一行数据按'，'分割
	
				for(int j = 0;j <= 22;j++)
				{ //可根据数据的实际情况取循环获取
					getline(readstr,number,','); //循环读取数据
					data_line.push_back(atof(number.c_str())); //字符串传int
				} 
				user_arr.push_back(data_line); //插入到vector中	
				
			}		
			user_arr=transpose(user_arr);
			vec1.push_back(user_arr);
		}
	}
	//cout<< std_set[34][17][4]<<endl;
}

double KNN::get_distance(int i, int j, vector<vector<double>>matrix1, vector<vector<double>>matrix2)
{
	//char url_str[50];
	double arr[23];
	double dtwDistance = 0;
	double AdtwDistance = 0;

	//sprintf(url_str, ".\\gesture%d_%d.csv", i, j);
	//Ptr<ml::TrainData> train_data;
	//train_data = ml::TrainData::loadFromCSV(url_str, 0, -2, 0);//first 0 is skip how much row
	//cv::Mat stdSamples = train_data->getSamples();
	////cv::Mat C = m.colRange(0,1).clone();
	////std::cout << C.t() <<std::endl;

	//for (int p = 0; p < matrix2.size(); p++)
	for (int p = 0; p < 23; p++)
	{
		//if((p%4==3)) continue;
		arr[p] = DTW::dtw_distance_only(matrix1[p], matrix2[p], 2.0);
		//arr[p] = TSdist::computeDTW(ts1, ts2, 1, 2, 2);
		//cout << arr[p] << endl;
		//cout << "cDTW distance is: " << TSdist::computeDTW(ts1, ts2, 1, 2, 2) << endl;
		dtwDistance +=arr[p];
		//cout << arr[p] << endl;
		//cout << "nDTW distance is: " << TSdist::computeNormalizedDTW(ts1, ts2, 1, 2) << endl;
		
	}
	if(i==3)
	{
		yaw_3 = arr[22];
	}
	if(i==4)
	{
		yaw_4 = arr[22];
	}
	//cout << i << " " << dtwDistance << endl;
	//cout << endl;
	AdtwDistance = dtwDistance / 23;
	return AdtwDistance;
}

void KNN::get_all_distance(vector<vector<double>> matrix1)
{
	
	double tempdistance;
	int temp = 0;
	for (int i = 1; i <= Class1; i++)
	{
		for (int j = 1; j <= clCount1; j++)
		{
			tempdistance = get_distance(i, j, matrix1, std_set[temp]);

						
			if (tempdistance >= 4)
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

void KNN::get_all_distance_static(vector<vector<double>> matrix1)
{
	
	double tempdistance;
	int temp = 1;
	for (int i = 1; i <= 10; i++)
	{
		for (int j = 1; j <= 1; j++)
		{
			tempdistance = get_distance(i, j, matrix1, state_set[temp]);

						
			if (tempdistance >= 1000)
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

double KNN::get_all_distance_state(vector<vector<double>> matrix1)
{
	
	double tempdistance;
	int temp = 0;

	tempdistance = get_distance(0, 0, matrix1, state_set[temp]);
	//cout<<tempdistance<<endl;
	if (tempdistance <= 0.8)
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

double KNN::get_all_distance_key(int typ, vector<vector<double>> matrix1 )
{
	double tempdistance[5];
	int temp = 0;
	double result;
	for (int i = 0; i <= 4; i++)
	{
		tempdistance[i] = get_distance(i, i, matrix1, key_set[temp]);
		temp++;
	}
	double ret = *min_element(tempdistance, tempdistance + 5);
	//cout<<" "<<tempdistance[0]<<" ";
	//cout<<" "<<tempdistance[1]<<" ";
	//cout<<" "<<tempdistance[2]<<" ";
	//cout<<" "<<tempdistance[3]<<" ";
	//cout<<" "<<ret<<" ";
	if(typ == 0)
	result = tempdistance[0] == ret ? 100 : ret;
	if(typ == 1)
	result = tempdistance[0] == ret ? ret : 100;
	//cout<<" "<<result<<endl;
	return result;
}
void KNN::get_s_gesture_name(int i)
{
	switch (i)
	{
	case 1:
		cout << "数字1" << endl;
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
//int main () 
//{
//	int success = 0;
//
//  // ========= Test case 1 ========= 
//  //std::vector<std::vector<double> > a = { {0, 2}, {1, 2}, {2, 2}, {3, 2}, {4, 2} };
//  //std::vector<std::vector<double> > b = { {0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0} };
//	double ans = 10.0;
//	load_csv(std_set);
//	clock_t start, finish;
//	double duration;
//	start = clock();
//	
//	KNN knn(35);
//	knn.get_all_distance(std_set[0]);
//	knn.get_max_freq_label();
//
//	finish = clock();
//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
//	cout << "识别耗时：";
//	printf("%f seconds\n", duration);
//
//	//std::cout << dtw << std::endl;
// 
//	return success;
//}

//
// Main function for the demo.
//
int main( int argc, char *argv[] )
{
	
	// This defines how long to sleep, in milliseconds, between captures
	int pollingInterval = 27;

	// If an argument was supplied
	if ( argc > 1 )
	{
		// Assume it is the polling interval
		pollingInterval = atoi( argv[ 1 ] );
	}

	vhtIOConn *gloveDict;
	vhtCyberGlove *glove;
	vhtIOConn *trackerDict;
	vhtTracker *tracker;

	try
	{
		// Get the default glove connection object
		//gloveDict = vhtIOConn::getDefault( vhtIOConn::glove );

		// You could alternatively connect to a specific glove
		gloveDict = new vhtIOConn( "cyberglove", "localhost", "12345", "com3", "115200" );

		// If that failed, then try to connect to the default touch
		if ( gloveDict == NULL )
		{
			gloveDict = vhtIOConn::getDefault( vhtIOConn::touch );
		}

		// Create the glove from our connection object
		glove = new vhtCyberGlove(gloveDict);
	}
	catch ( vhtBaseException * e )
	{
		printf( "Could not connect to any CyberGlove: %s\nPress <enter> to exit.\n", e->getMessage() );
		getchar();
		return 0;
	}

	try
	{
		// Get the default tracker connection object
		trackerDict = vhtIOConn::getDefault( vhtIOConn::tracker );

		// Create the tracker from our connection object
		tracker = new vhtTracker( trackerDict );
	}
	catch ( vhtBaseException * e )
	{
		// If connecting to a real tracker failed, use the emulator
		printf( "Could not connect to tracker, using emulator instead\n" );
		tracker = new vhtTrackerEmulator();
	}

	//
	// The demo loop: get the finger angles from the glove.
	//
	load_csv(state_set);
	load_csv_d(std_set);
	load_csv_k(key_set);
	vector<vector<double>> seqVector;
	vector<double> toolVector;
	double FrameVelocity[5];
	int tmp = 0;
	int state = 0;
	int count_5 = 0;
	double baseT = glove->getLastUpdateTime();
	double turnT = baseT;
	while( true )
	{
		// update data from the physical device
		glove->update();
		tracker->update();
		vector<double> tempVector;
		double accAngle = 0;
		// Get update time delta
		
		//cout << "deltaTime: " << glove->getLastUpdateTime() - baseT  << "\n";
		//cout << "frameFrequency: " << 1/( glove->getLastUpdateTime() - turnT) << "\n";
		
		turnT = glove->getLastUpdateTime();
		
		// For every finger
		//cout << "Glove: \n";
		for( int finger = 0; finger < GHM::nbrFingers; finger++ )
		{
		/*	cout << finger << " ";*/

			// For every joint on this finger
			for( int joint = 0; joint < GHM::nbrJoints; joint++ )
			{
				// Print the joint angle
				tempVector.push_back(glove->getData( (GHM::Fingers)finger, (GHM::Joints)joint ));
				//cout << *(tempVector.end()-1)<< " ";
				//double x= glove->getData( (GHM::Fingers)finger, (GHM::Joints)joint ) ;
			}

			// Print the abduction value
			tempVector.push_back(glove->getData( (GHM::Fingers)finger, GHM::abduct ));
			//cout << *(tempVector.end()-1)<< " ";
			//cout << "\n";
		}

		// Print the palmArch, wristFlexion, and wristAbduction values (treated as a 6th finger)


		tempVector.push_back( glove->getData( GHM::palm, GHM::palmArch ));
		tempVector.push_back( glove->getData( GHM::palm, GHM::wristFlexion ));
		tempVector.push_back( glove->getData( GHM::palm, GHM::wristAbduction ));
		if(!toolVector.empty())
		{
			for(int i = 0; i <toolVector.size(); i++)
			{
				accAngle += abs(toolVector[i]-tempVector[i]);
			}
		}
		toolVector = tempVector;
		tempVector.push_back(accAngle);
		seqVector.push_back(tempVector);
		FrameVelocity[tmp%5]=accAngle;
		tmp++;
		//if(state != -1 && state!= 0 && tmp > 160) 
		//{
		//	state = -1;
		//}
		//if(state!= 0 && tmp > 350）
		//{

		//}
		//cout << 5 << " ";
		//cout << glove->getData( GHM::palm, GHM::palmArch ) << " ";
		//cout << glove->getData( GHM::palm, GHM::wristFlexion ) << " ";
		//cout << glove->getData( GHM::palm, GHM::wristAbduction );
		//cout << "\n";
		//cout << "deltaAngle: " << accAngle << "\n";		
		//cout << "seqVector.size: " << seqVector.size() << endl<< endl;	

		//if(seqVector.size() >= 50)
		//{
		//	seqVector.erase(seqVector.begin(), seqVector.begin() + 25);
		//	seqVector.erase(seqVector.end() - 20, seqVector.end());	
		//	vector<vector<double>> matrix = transpose(seqVector);
		//	KNN knn(10);
		//	knn.get_all_distance_static(matrix);
		//	knn.get_max_freq_label();
		//	std::vector<std::vector<double>>().swap(seqVector);
		//	Sleep(100000);
		//}


		//if(state == 0&&seqVector.size() >= 5 &&*min_element(FrameVelocity, FrameVelocity + 4) > 0.03)
		//{
		//	seqVector.erase(seqVector.begin(), seqVector.end()- 5);	
		//	state = 1;
		//}
		//if(state == 1 && seqVector.size() >= 10 &&*max_element(FrameVelocity, FrameVelocity + 4) < 0.1)
		//{
		//	vector<vector<double>> matrix = transpose(seqVector);
		//	clock_t start, finish;
		//	double duration;
		//	start = clock();
		//	KNN knn(5);
		//	knn.get_all_distance(matrix);
		//	knn.get_max_freq_label();
		//	finish = clock();
		//	duration = (double)(finish - start) / CLOCKS_PER_SEC;
		//	cout << "识别耗时：";
		//	printf("%f seconds\n", duration);
		//	Sleep(100000);

		//}
//if (state == -1)
//{
//	cout << -1;
//}
//if (state == 1)
//{
//	cout << 1;
//}
//if (state == 2)
//{
//	cout << 2;
//}
//if (state == 3)
//{
//	cout << 3;
//}
//if (state == 4)
//{
//	cout << 4;
//}
		vector<vector<double>> sinVector;
		sinVector.push_back(tempVector);
		sinVector = transpose(sinVector);

		if (state == -1 && accAngle < 0.05)
		{
			
			KNN knn(1);
			if (knn.get_all_distance_key(1,sinVector) < 0.3)
			{
				state = 1;
				std::vector<std::vector<double>>().swap(seqVector);
				tmp = 0;
			}
		}

		if (state == 0 && seqVector.size() >= 25)
		{
			seqVector.erase(seqVector.begin(), seqVector.begin() + 10);
			seqVector.erase(seqVector.end()-10, seqVector.end());																																																						
			//cout << seqVector.size() << endl;
			KNN knn(1);
			vector<vector<double>> matrix = transpose(seqVector);
			state = knn.get_all_distance_state(matrix);
			std::vector<std::vector<double>>().swap(seqVector);
		}
		//if (state == 1)
		//{

		//	cout << "state = 1" << endl;
		//}
		if (state == 1 && seqVector.size() > 5 && *min_element(FrameVelocity, FrameVelocity + 4) > 0.03)
		{
			state = 2;
			std::vector<std::vector<double>>().swap(seqVector);
			
		}

		if (state == 2 )
		{
			//cout << speed_now << endl;
			if (seqVector.size() >= 30)
			{
				state = 3;
			}
			else if (*min_element(FrameVelocity, FrameVelocity + 4) >= 0.03)
			{
				std::vector<std::vector<double>>().swap(seqVector);//////////
				count_5 = 0;
			}
			//else if(speed_now < 100 && !vec_obo.empty())
			else if (accAngle <0.03 && !sinVector.empty())
			{
				KNN knn(1);
				if(knn.get_all_distance_key(0,sinVector)< 0.22)
				{
					count_5++;
				}
				else
				{
					count_5 = 0;
				}

				if (count_5 >= 2)
				{
					state = 3;
					std::vector<std::vector<double>>().swap(seqVector);
					count_5 = 0;
				}	
			}
			else
			{
				count_5 = 0;
			}
			//cout <<state << count_5 << " "<<seqVector.size()<<endl;
		}

		if (state == 3 && seqVector.size()> 5 && *min_element(FrameVelocity, FrameVelocity + 4) > 0.03)
		{
			state = 4;
			seqVector.erase(seqVector.begin(), seqVector.end() - 5);
			//cout << seqVector.size() << endl;
			//std::vector<std::vector<double>>().swap(seqVector);
			//cout << "start" << endl;
		}
		else if (state == 3 && seqVector.size() >= 40)
		{
			seqVector.erase(seqVector.begin(), seqVector.begin() + 18);
			seqVector.erase(seqVector.end() - 17, seqVector.end());
			vector<vector<double>> matrix = transpose(seqVector);
			cout << "静态手势" << endl;
			clock_t start, finish;
			double duration;
			start = clock();

			KNN knn(1);
			knn.get_all_distance_static(matrix);
			int s;
			s = knn.get_max_freq_label();
			knn.get_s_gesture_name(s);

			finish = clock();
			duration = (double)(finish - start) / CLOCKS_PER_SEC;
			cout << "识别用时" << " ";
			printf("%f seconds\n", duration);

			std::vector<std::vector<double>>().swap(seqVector);
			state = -1;
			tmp = 0;
		}

		if (state == 4 && seqVector.size() >= 15 &&*max_element(FrameVelocity, FrameVelocity + 4) < 0.1)
		{
			std::cout << "动态手势" << std::endl;			
			seqVector.erase(seqVector.end() - 5, seqVector.end());
			vector<vector<double>> matrix = transpose(seqVector);
			clock_t start, finish;
			double duration;
			start = clock();
			KNN knn(5);
			knn.get_all_distance(matrix);
			int s;
			s = knn.get_max_freq_label();
			knn.get_d_gesture_name(s);

			finish = clock();
			duration = (double)(finish - start) / CLOCKS_PER_SEC;
			cout << "识别耗时：";
			printf("%f seconds\n", duration);
			std::vector<std::vector<double>>().swap(seqVector);
			state = -1;
			tmp = 0;
		}
		//static int t = 0;
		//std::ofstream outFile;
		//outFile.open("./traindata/dynamic/gesture7_6.csv", std::ios::out | std::ios::app);
		//if (t==0 && *min_element(FrameVelocity, FrameVelocity + 4) > 0.03)
		//{
		//	outFile << 100000 << endl;
		//	t++;
		//}
		//if (t==1 && *max_element(FrameVelocity, FrameVelocity + 4) < 0.1)
		//{
		//	outFile << 200000 << endl;
		//	t++;
		//}
		//outFile << tempVector[0] << ',' << tempVector[1] << ',' << tempVector[2] << ','
		//	<< tempVector[3] << ',' << tempVector[4] << ',' << tempVector[5] << ',' << tempVector[6] << ','
		//	<< tempVector[7] << ',' << tempVector[8] << ',' << tempVector[9] << ',' << tempVector[10] << ','
		//	<< tempVector[11] << ','<< tempVector[12] << ',' << tempVector[13] << ',' << tempVector[14] << ',' << tempVector[15] << ','
		//	<< tempVector[16] << ',' << tempVector[17] << ',' << tempVector[18] << ',' << tempVector[19] << ','
		//	<< tempVector[20] << ',' << tempVector[21] << ','<< tempVector[22] << ','<< tempVector[23] <<std::endl;
		//outFile.close();



		
		
		// Wait for the specified interval
		Sleep( pollingInterval );
	}

	return 0;
}

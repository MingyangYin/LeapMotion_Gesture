//#include"vectomat.h"
#include "knn.h"
#include <vector>
//#include <list>
//#include <map>
#include <fstream>
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
#include <sys/timeb.h> 
#include <windows.h>


#include "SampleListener.h"

using namespace std;
using namespace cv;
//const std::string fingerNames[] = { "a", "b", "c", "d" ,"e"};
//const std::string boneNames[] = { "1", "2", "3", "4" };
float fingerVelocity[5];
float FrameVelocity[8];
float FrameVelocity_s[5];
int state = 0;
int tmp = 0;
int count_5 = 0;
int reset = 0;
int nofun;
//const std::string fingerNames[] = { "Thumb", "Index", "Middle", "Ring", "Pinky" };
//const std::string boneNames[] = { "Metacarpal", "Proximal", "Middle", "Distal" };


// Example
void SampleListener::onFrame(const Controller& controller) {

	
	// Get the most recent frame and report some basic information
	//const Frame frame = controller.frame();
	m_SrcFrame = controller.frame();
	const Frame frame = m_SrcFrame;
	/*std::cout << "Frame id: " << frame.id()
	<< ", timestamp: " << frame.timestamp()
	<< ", hands: " << frame.hands().count()
	<< ", extended fingers: " << frame.fingers().extended().count() << std::endl;
	*/

	//float instantaneousFrameRate = frame.currentFramesPerSecond();//帧率
	//std::cout << instantaneousFrameRate <<std::endl;

	//int64_t framePeriod = frame.timestamp() - controller.frame(1).timestamp();//捕获一帧约*s
	//std::cout << framePeriod << std::endl;

	HandList hands = frame.hands();
	//HandList hands1 = controller.frame(1).hands();

	Mat bImg(800, 800, CV_8UC3, Scalar(255, 255, 255));
	//std::map<std::string, Leap::Vector> tempMap;
	std::vector<Leap::Vector> tempVector;
	static std::vector<std::vector<double>> seqVector;
	double speed_now;
	for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
		// Get the first hand
		
		const Hand hand = *hl;

		/*const Hand hand1 = hands1[hand.id()];*/

		//std::string handType = hand.isLeft() ? "Left hand" : "Right hand";
		//std::cout << handType << " ID:" << hand.id() << std::endl;
		//std::cout << std::string(2, ' ') << handType << ", id: " << hand.id()
		//	<< ", palm position: " << hand.palmPosition() << std::endl;
		// Get the hand's normal vector and direction
		const Leap::Vector normal = hand.palmNormal();
		const Leap::Vector direction1 = hand.direction();
		
		tempVector.push_back(hand.palmPosition());
		tempVector.push_back(normal);

		

		// Get fingers
		const FingerList fingers = hand.fingers();
		int tp = 0;
		
		double speed[5] = {0,0,0,0,0};
		for (FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl)
		{
			const Finger finger = *fl;
			Leap::Vector direction = finger.direction();
			
			//cout << finger.tipVelocity().magnitude() << endl;
			//读取字符串格式的fingername
			//std::cout << std::string(4, ' ') << fingerNames[finger.type()]<<std::endl;
			/*

			<< " finger, id: " << finger.id()
			<< ", length: " << finger.length()
			<< ", width: " << finger.width() << std::endl;
			*/

			// Get finger bones
			//for (int b = 0; b < 4; ++b) {
			Bone::Type boneType = static_cast<Bone::Type>(3);
			Bone bone = finger.bone(boneType);

			Leap::Vector middle = bone.center();
			//Leap::Vector normiddle = middle.normalized();
			//std::vector<float> A{normiddle[0],normiddle[1],normiddle[2]};
			if (tempVector.size() < 13)
			{
				tempVector.push_back(middle);
				tempVector.push_back(direction);
				speed[tp] = finger.tipVelocity().magnitude();
				tp++;
				//cout << fingerVelocity[tp-1] << endl;
			}
			if (tp == 5)
			{
				FrameVelocity[tmp % 8] = (speed[0] + speed[1]) / 2;
				speed_now = (speed[0] + speed[1]) / 2;
				FrameVelocity_s[tmp % 5] = (speed[0] + speed[1]) / 2;
				/*if(FrameVelocity[tmp % 8]<60)
				cout << 1;*/
				tmp++;
			}





	
		/*std::cout << boneNames[boneType] << std::endl;
		std::cout << middle[0] << "," << middle[1] << "," << "," << middle[2] << std::endl;*/
		//为什么要加300，为了把效果移动到中央
		Point2f center = Point2f(middle[0] + 300, middle[2] + 300);
		circle(bImg, center, 8, Scalar(0, 0, 0), -1);
		//得到，掌心的球的坐标，一样移动300




		}

		tempVector.push_back(direction1);

	}
	std::vector<double> vec_obo;
	//std::cout << tempVector.size() << std::endl;
	if (tempVector.size() == 13)
	{
		std::vector<double> v1{ tempVector[2].distanceTo(tempVector[0]),
			tempVector[4].distanceTo(tempVector[0]) ,
	
			tempVector[10].distanceTo(tempVector[0]),
			tempVector[1].angleTo(tempVector[3]),
			tempVector[1].angleTo(tempVector[5]),
			
			tempVector[1].angleTo(tempVector[11]),
			//tempVector[0].magnitude(),
			tempVector[12].pitch(),
			tempVector[12].yaw(),
			tempVector[6].distanceTo(tempVector[0]),
			tempVector[8].distanceTo(tempVector[0]),
			tempVector[1].angleTo(tempVector[7]),
			tempVector[1].angleTo(tempVector[9]),
		};
		//保证40帧帧率；
		vec_obo = v1;
		//cout << vec_obo.size() << endl;

		if (frame.currentFramesPerSecond() <= 50)
		{
			seqVector.push_back(v1);
		}
		else if (frame.id() % 2 == 0)
		{
			seqVector.push_back(v1);
		}

		//static int t = 0;
		//std::ofstream outFile;
		//outFile.open(".\\traindata\\key_frame\\gesture3_3.csv", std::ios::out | std::ios::app);
		//if (t==0 && *min_element(FrameVelocity, FrameVelocity + 7) > 80)
		//{
		//	outFile << 100000 << endl;
		//	t++;
		//}
		//if (t==1 && *max_element(FrameVelocity_s, FrameVelocity_s + 4) < 60)
		//{
		//	outFile << 200000 << endl;
		//	t++;
		//}
		//outFile << v1[0] << ',' << v1[1] << ',' << v1[2] << ','
		//	<< v1[3] << ',' << v1[4] << ',' << v1[5] << ',' << v1[6] << ','
		//	<< v1[7] << ',' << v1[8] << ',' << v1[9] << ',' << v1[10] << ','
		//	<< v1[11] << std::endl;

		//outFile.close();

		//std::cout << seqVector.size() << std::endl;
	}
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

	if (state == -1 && speed_now < 100 && !vec_obo.empty())
	{
		KNN knn(1);
		if (knn.get_start_distance(vec_obo) < 5.0)
		{
			state = 1;
			std::vector<std::vector<double>>().swap(seqVector);
			tmp = 0;
			reset = 0;
		}
	}
	if (state == 0 && seqVector.size() >= 50)
	{
		seqVector.erase(seqVector.begin(), seqVector.begin() + 15);
		seqVector.erase(seqVector.end()-15, seqVector.end());																																																						
		//cout << seqVector.size() << endl;
		KNN knn(1);
		state = knn.get_state_distance(seqVector);
		std::vector<std::vector<double>>().swap(seqVector);
	}
	/*if (state == 1)
	{
		cout << "state = 1" << endl;
	}*/
	if (state == 1 && seqVector.size() > 7 && *min_element(FrameVelocity, FrameVelocity + 7) > 100)
	{
		state = 2;
		std::vector<std::vector<double>>().swap(seqVector);
		
	}

	if (state == 2 )
	{
		//cout << speed_now << endl;
		if (seqVector.size() >= 40)
		{
			state = 3;
		}
		else if (*min_element(FrameVelocity, FrameVelocity + 7) >=100)
		{
			std::vector<std::vector<double>>().swap(seqVector);//////////
			count_5 = 0;
		}
		//else if(speed_now < 100 && !vec_obo.empty())
		else if (!vec_obo.empty())
		{
			KNN knn(1);
			if(knn.get_key_distance(vec_obo)<5)
			{
				count_5++;
			}
			else
			{
				count_5 = 0;
			}

			if (count_5 >= 4)
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

	if (state == 3 && seqVector.size()>7 && *min_element(FrameVelocity, FrameVelocity + 7) > 100)
	{
		state = 4;
		seqVector.erase(seqVector.begin(), seqVector.end() - 8);
		//cout << seqVector.size() << endl;
		//std::vector<std::vector<double>>().swap(seqVector);
		//cout << "start" << endl;
	}
	else if (state == 3 && seqVector.size() >= 50)
	{
		seqVector.erase(seqVector.begin(), seqVector.begin() + 15);
		seqVector.erase(seqVector.end() - 15, seqVector.end());

		cout << "静态手势" << endl;
		clock_t start, finish;
		double duration;
		start = clock();

		
	

		KNN knn(1);
		knn.get_all_distance_static(seqVector);
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
	if (state == 4 && seqVector.size() >= 20 && *max_element(FrameVelocity_s, FrameVelocity_s + 4) < 60)
	{
		seqVector.erase(seqVector.end()-5, seqVector.end());
		std::cout << "动态手势" << std::endl;

		
		clock_t start, finish;
		double duration;
		start = clock();

		KNN knn(5);
		knn.get_all_distance(seqVector);
		int s;
		s = knn.get_max_freq_label();
		knn.get_d_gesture_name(s);

		finish = clock();
		duration = (double)(finish - start) / CLOCKS_PER_SEC;
		cout << "识别用时" << " ";
		printf("%f seconds\n", duration);

		std::vector<std::vector<double>>().swap(seqVector);
		state = -1;
		tmp = 0;
	}

	if (state != 0) reset++;
	if (reset > 300)
	{
		std::vector<std::vector<double>>().swap(seqVector);
		state = -1;
		tmp = 0;
		//reset = 0;
	}
	if (reset > 600)
	{
		std::vector<std::vector<double>>().swap(seqVector);
		state = 0;
		tmp = 0;
		reset = 0;
		cout << "长时间未有手势输入，重新检测起始手势中" << endl;
	}


	//if (state == 1 && seqVector.size()>8 && *min_element(FrameVelocity, FrameVelocity + 7) > 100)
	//{
	//	state = 2;
	//	seqVector.erase(seqVector.begin(), seqVector.end() - 8);
	//	//cout << seqVector.size() << endl;
	//	//std::vector<std::vector<double>>().swap(seqVector);
	//	//cout << "start" << endl;
	//}
	//else if (state == 1 && seqVector.size() >= 40)
	//{
	//	seqVector.erase(seqVector.begin(), seqVector.begin() + 10);
	//	seqVector.erase(seqVector.end() - 10, seqVector.end());
	//	cout << "静态手势" << endl;
	//	KNN knn(1);
	//	knn.get_all_distance_static(seqVector);
	//	int s;
	//	s = knn.get_max_freq_label();
	//	knn.get_s_gesture_name(s);

	//	std::vector<std::vector<double>>().swap(seqVector);
	//	state = 0;
	//	tmp = 0;
	//}

	//if (state == 2 && seqVector.size() >= 20 && *max_element(FrameVelocity_s, FrameVelocity_s + 4) < 60)
	//{
	//	seqVector.erase(seqVector.end()-5, seqVector.end());
	//	std::cout << "动态" << std::endl;
	//	KNN knn(5);
	//	knn.get_all_distance(seqVector);
	//	int s;
	//	s = knn.get_max_freq_label();
	//	knn.get_d_gesture_name(s);

	//	std::vector<std::vector<double>>().swap(seqVector);
	//	state = 0;
	//	tmp = 0;
	//}

	imshow("控制效果", bImg);
	waitKey(1);
	//if (!frame.hands().isEmpty()) {
	//	std::cout << std::endl;
	//}


}
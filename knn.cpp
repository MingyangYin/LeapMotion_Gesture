#include "knn.h"
using namespace std;

KNN::KNN(int k)
{
	this->k = k;


	//fin.open("data.txt");

	//if (!fin)
	//{
	//	cout << "can not open the file data.txt" << endl;
	//	exit(1);
	//}

	///* input the dataSet */
	//for (int i = 0; i<rowLen; i++)
	//{
	//	for (int j = 0; j<colLen; j++)
	//	{
	//		fin >> dataSet[i][j];
	//	}
	//	fin >> labels[i];
	//}

	//cout << "please input the test data :" << endl;
	///* inuput the test data */
	//for (int i = 0; i<colLen; i++)
	//	cin >> testData[i];

}

/*
* calculate the distance between test data and dataSet[i]
*/
//double KNN::get_distance(tData *d1, tData *d2)
//{
//	double sum = 0;
//	for (int i = 0; i<colLen; i++)
//	{
//		sum += pow((d1[i] - d2[i]), 2);
//	}
//
//	//	cout<<"the sum is = "<<sum<<endl;
//	return sqrt(sum);
//}

/*
* calculate all the distance between test data and each training data
*/
//void KNN::get_all_distance()
//{
//	double distance;
//	int i;
//	for (i = 0; i<rowLen; i++)
//	{
//		distance = get_distance(dataSet[i], testData);
//		//<key,value> => <i,distance>
//		map_index_dis[i] = distance;
//	}
//
//	//traverse the map to print the index and distance
//	map<int, double>::const_iterator it = map_index_dis.begin();
//	while (it != map_index_dis.end())
//	{
//		cout << "index = " << it->first << " distance = " << it->second << endl;
//		it++;
//	}
//}







/*
* check which label the test data belongs to to classify the test data
*/
int KNN::get_max_freq_label()
{
	//transform the map_index_dis to vec_index_dis
	vector<PAIR> vec_index_dis(map_index_dis.begin(), map_index_dis.end());
	//sort the vec_index_dis by distance from low to high to get the nearest data
	sort(vec_index_dis.begin(), vec_index_dis.end(), CmpByValue());

	for (int i = 0; i<k; i++)
	{
		cout << "the index = " << vec_index_dis[i].first << " the distance = " << vec_index_dis[i].second << " the label = " << labels[vec_index_dis[i].first] << endl;
		//calculate the count of each label
		map_label_freq[labels[vec_index_dis[i].first]]++;
	}

	map<tLabel, int>::const_iterator map_it = map_label_freq.begin();
	tLabel label;
	int max_freq = 0;
	//find the most frequent label
	while (map_it != map_label_freq.end())
	{
		if (map_it->second > max_freq)
		{
			max_freq = map_it->second;
			label = map_it->first;
		}
		map_it++;
	}
	cout << "The test data belongs to the " << label << " label" << endl;
	return label;
}
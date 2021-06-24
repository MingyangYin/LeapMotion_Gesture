///********************************************************************
// AUTHOR: Chris Ullrich.
// Description: Base demonstration.
//		   -- COPYRIGHT VIRTUAL TECHNOLOGIES, INC. 1999 --
//********************************************************************/
//
//#define _WIN32_WINNT 0x0400			// avoid winsock problem
//#include "knn.h"
//#include <vhtBase.h>
//
//using namespace std;
//
//
////
//// Main function for the demo.
////
//int main( int argc, char *argv[] )
//{
//	
//	// This defines how long to sleep, in milliseconds, between captures
//	int pollingInterval = 17;
//
//	// If an argument was supplied
//	//if ( argc > 1 )
//	//{
//	//	// Assume it is the polling interval
//	//	pollingInterval = atoi( argv[ 1 ] );
//	//}
//
//	vhtIOConn *gloveDict;
//	vhtCyberGlove *glove;
//	vhtIOConn *trackerDict;
//	vhtTracker *tracker;
//
//	try
//	{
//		// Get the default glove connection object
//		//gloveDict = vhtIOConn::getDefault( vhtIOConn::glove );
//
//		// You could alternatively connect to a specific glove
//		gloveDict = new vhtIOConn( "cyberglove", "localhost", "12345", "com3", "115200" );
//
//		// If that failed, then try to connect to the default touch
//		if ( gloveDict == NULL )
//		{
//			gloveDict = vhtIOConn::getDefault( vhtIOConn::touch );
//		}
//
//		// Create the glove from our connection object
//		glove = new vhtCyberGlove(gloveDict);
//	}
//	catch ( vhtBaseException * e )
//	{
//		printf( "Could not connect to any CyberGlove: %s\nPress <enter> to exit.\n", e->getMessage() );
//		getchar();
//		return 0;
//	}
//
//	try
//	{
//		// Get the default tracker connection object
//		trackerDict = vhtIOConn::getDefault( vhtIOConn::tracker );
//
//		// Create the tracker from our connection object
//		tracker = new vhtTracker( trackerDict );
//	}
//	catch ( vhtBaseException * )
//	{
//		// If connecting to a real tracker failed, use the emulator
//		printf( "Could not connect to tracker, using emulator instead\n" );
//		tracker = new vhtTrackerEmulator();
//	}
//
//	//
//	// The demo loop: get the finger angles from the glove.
//	//
//	
//	vector<vector<double>> seqVector;
//	vector<double> toolVector;
//	double baseT = glove->getLastUpdateTime();
//	double turnT = baseT;
//	while( true )
//	{
//		// update data from the physical device
//		glove->update();
//		tracker->update();
//		vector<double> tempVector;
//		double accAngle = 0;
//		// Get update time delta
//		
//		cout << "deltaTime: " << glove->getLastUpdateTime() - baseT  << "\n";
//		cout << "frameFrequency: " << 1/( glove->getLastUpdateTime() - turnT) << "\n";
//		
//		turnT = glove->getLastUpdateTime();
//		
//		// For every finger
//		cout << "Glove: \n";
//		for( int finger = 0; finger < GHM::nbrFingers; finger++ )
//		{
//			cout << finger << " ";
//
//			// For every joint on this finger
//			for( int joint = 0; joint < GHM::nbrJoints; joint++ )
//			{
//				// Print the joint angle
//				tempVector.push_back(glove->getData( (GHM::Fingers)finger, (GHM::Joints)joint ));
//				cout << *(tempVector.end()-1)<< " ";
//				//double x= glove->getData( (GHM::Fingers)finger, (GHM::Joints)joint ) ;
//			}
//
//			// Print the abduction value
//			tempVector.push_back(glove->getData( (GHM::Fingers)finger, GHM::abduct ));
//			cout << *(tempVector.end()-1)<< " ";
//			cout << "\n";
//		}
//
//		// Print the palmArch, wristFlexion, and wristAbduction values (treated as a 6th finger)
//
//
//		tempVector.push_back( glove->getData( GHM::palm, GHM::palmArch ));
//		tempVector.push_back( glove->getData( GHM::palm, GHM::wristFlexion ));
//		tempVector.push_back( glove->getData( GHM::palm, GHM::wristAbduction ));
//		if(!toolVector.empty())
//		{
//			for(int i = 0; i <toolVector.size(); i++)
//			{
//				accAngle += abs(toolVector[i]-tempVector[i]);
//			}
//		}
//		toolVector = tempVector;
//		tempVector.push_back(accAngle);
//		seqVector.push_back(tempVector);
//
//		if(seqVector.size()>50)
//		{
//			seqVector.erase(seqVector.begin(), seqVector.begin() + 25);
//			seqVector.erase(seqVector.end() - 20, seqVector.end());	
//			KNN knn(1);
//			knn.get_all_distance£¨seqVector);
//			knn.get_max_freq_label();
//		}
//		//static int t = 0;
//		//std::ofstream outFile;
//		//outFile.open("./traindata/static/gesture10_10.csv", std::ios::out | std::ios::app);
//		////if (t==0 && *min_element(FrameVelocity, FrameVelocity + 7) > 80)
//		////{
//		////	outFile << 100000 << endl;
//		////	t++;
//		////}
//		////if (t==1 && *max_element(FrameVelocity_s, FrameVelocity_s + 4) < 60)
//		////{
//		////	outFile << 200000 << endl;
//		////	t++;
//		////}
//		//outFile << tempVector[0] << ',' << tempVector[1] << ',' << tempVector[2] << ','
//		//	<< tempVector[3] << ',' << tempVector[4] << ',' << tempVector[5] << ',' << tempVector[6] << ','
//		//	<< tempVector[7] << ',' << tempVector[8] << ',' << tempVector[9] << ',' << tempVector[10] << ','
//		//	<< tempVector[11] << ','<< tempVector[12] << ',' << tempVector[13] << ',' << tempVector[14] << ',' << tempVector[15] << ','
//		//	<< tempVector[16] << ',' << tempVector[17] << ',' << tempVector[18] << ',' << tempVector[19] << ','
//		//	<< tempVector[20] << ',' << tempVector[21] << ','<< tempVector[22] <<std::endl;
//		//outFile.close();
//
//
//		cout << 5 << " ";
//		cout << glove->getData( GHM::palm, GHM::palmArch ) << " ";
//		cout << glove->getData( GHM::palm, GHM::wristFlexion ) << " ";
//		cout << glove->getData( GHM::palm, GHM::wristAbduction );
//		cout << "\n";
//		cout << "deltaAngle: " << accAngle << "\n";		
//		cout << "seqVector.size: " << seqVector.size() << "\n";	
//		
//
//		// Wait for the specified interval
//		Sleep( pollingInterval );
//	}
//
//	return 0;
//}

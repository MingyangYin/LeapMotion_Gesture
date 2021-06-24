#pragma once
#include "Leap.h"
//#include <opencv2/opencv.hpp>
//#include <iostream>
//#include <cstring>
//#include<opencv2\highgui\highgui.hpp>
//#include<opencv2\imgproc\imgproc.hpp>
//#include<opencv2\core\core.hpp>
//#include<stdio.h>
//#include <windows.h>
using namespace Leap;


class SampleListener : public Listener
{
public:
	SampleListener();
	~SampleListener();

	Frame m_SrcFrame;
	//Mat m_HandImg;
public:
	virtual void onInit(const Controller&);
	virtual void onConnect(const Controller&);
	virtual void onDisconnect(const Controller&);
	virtual void onExit(const Controller&);
	virtual void onFrame(const Controller&);
	virtual void onFocusGained(const Controller&);
	virtual void onFocusLost(const Controller&);
	virtual void onDeviceChange(const Controller&);
	virtual void onServiceConnect(const Controller&);
	virtual void onServiceDisconnect(const Controller&);
	virtual void onServiceChange(const Controller&);
	virtual void onDeviceFailure(const Controller&);
	virtual void onLogMessage(const Controller&, MessageSeverity severity, int64_t timestamp, const char* msg);
};



#pragma once

//#include "ofxMacamPs3Eye.h"





#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

#include "pkmDetector.h"

#include "ofxOpenCv.h"
#include "ofShader.h"

#include <opencv2/opencv.hpp>
#include <opencv2/features2d/features2d.hpp>

const int CAM_WIDTH = 320;
const int CAM_HEIGHT = 240;

class testApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	/*
	vector<ofxMacamPs3Eye*> cameras;  //multiple camera example
    
    ofxMacamPs3Eye *			vidInput;
	ofxCvColorImage			colorImg, testImg;
	ofxCvGrayscaleImage		grayImg, testGrayImg;
	int						x_start, x_end, y_start, y_end;
	bool					choosing_img, chosen_img;
	pkmDetector				detector;
	int						detection;
    */
    
    
};

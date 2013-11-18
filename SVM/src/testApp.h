/*
*  Created by Parag K. Mital - http://pkmital.com 
*  Contact: parag@pkmital.com
*
*  Copyright 2011 Parag K. Mital. All rights reserved.
* 
*	Permission is hereby granted, free of charge, to any person
*	obtaining a copy of this software and associated documentation
*	files (the "Software"), to deal in the Software without
*	restriction, including without limitation the rights to use,
*	copy, modify, merge, publish, distribute, sublicense, and/or sell
*	copies of the Software, and to permit persons to whom the
*	Software is furnished to do so, subject to the following
*	conditions:
*	
*	The above copyright notice and this permission notice shall be
*	included in all copies or substantial portions of the Software.
*
*	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,	
*	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
*	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
*	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
*	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
*	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
*	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
*	OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

#include "pkmDetector.h"

#include "ofxOpenCv.h"
#include "ofShader.h"

#include <opencv2/opencv.hpp>
#include <opencv2/features2d/features2d.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>

const int CAM_WIDTH = 512;
const int CAM_HEIGHT = 512;
const int SCREEN_WIDTH = CAM_WIDTH*2;
const int SCREEN_HEIGHT = CAM_HEIGHT + 75;


using namespace cv;
//using namespace ofxCv;
class testApp : public ofBaseApp {

	public:

	//~testApp();
	void setup();
	 
	void update();
	void draw();

	void keyPressed  (int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);

	
	ofxCvColorImage			colorImg, testImg;
    ofxCvColorImage			svmImg;
    ofxCvColorImage			svmImg_calc;
	ofxCvGrayscaleImage		grayImg, testGrayImg;
    
	int						x_start, x_end, y_start, y_end;
	bool					choosing_img, chosen_img;
	pkmDetector				detector;
	int						detection;
    
    cv::Mat image;
	
};

#endif

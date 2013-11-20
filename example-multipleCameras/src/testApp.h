#pragma once



#include "ofMain.h"
#include "ofxMacamPs3Eye.h"



#include "pkmDetector.h"

#include "ofxOpenCv.h"
#include "ofShader.h"

#include <opencv2/opencv.hpp>
#include <opencv2/features2d/features2d.hpp>

const int CAM_WIDTH = 320;
const int CAM_HEIGHT = 240;
const int FRAME_RATE = 30;
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
	
	vector<ofxMacamPs3Eye*> cameras;
    
    
    ofxMacamPs3Eye *			vidInput;
	
	ofxCvColorImage			colorImg, testImg;
	ofxCvGrayscaleImage		grayImg, testGrayImg;
    
	int						x_start, x_end, y_start, y_end;
	bool					choosing_img, chosen_img;
	pkmDetector				detector;
	int						detection;
    
    
    
    
    /*EYE*/
    
    /*Eye Movie*/
    ofxMacamPs3Eye *	 vidPlayer;
    ofxCvColorImage			colorImage_Player;
    ofxCvGrayscaleImage 	grayImage_Player;
    int 				threshold;
    ofxCvContourFinder 	contourFinder;
    
    
	clock_t start_selection_time;
    
    
    
    int                     obj_center_x;
    int                     obj_center_y;
    int                     eye_center_x;
    int                     eye_center_y;
    int                     eye_radius;
    int                     max_blob_index;
    
    int select_time;
    float max_area;
    bool obj_detected;
    bool eye_detected;
    bool obj_highlight;
    
    
    
    
    /*Aduino Code*/
	ofImage				bgImage;
	ofTrueTypeFont		font;
    ofTrueTypeFont      smallFont;
	ofArduino	ard;
	bool		bSetupArduino;			// flag variable for setting up arduino once
    
private:
    
    void setupArduino(const int & version);
    void digitalPinChanged(const int & pinNum);
    void analogPinChanged(const int & pinNum);
	void updateArduino();
    
    string buttonState;
    string potValue;
    bool pin_on;
    
};

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

#include "testApp.h"

//--------------------------------------------------------------

extern const int width = 640;
extern const int height = 480;
testApp::~testApp(){
}
void testApp::setup(){
    
    /*Enviroment Camera*/
	
	// init video input
	vidGrabber.initGrabber(CAM_WIDTH,CAM_HEIGHT);
	vidGrabber.setUseTexture(true);
	
	// window setup
	//ofSetWindowShape(SCREEN_WIDTH, SCREEN_HEIGHT);
	ofSetVerticalSync(true);
	ofSetFrameRate(30);
	ofBackground(0,0,0);
	
	// allocate stuff
	colorImg.allocate(CAM_WIDTH, CAM_HEIGHT);
	grayImg.allocate(CAM_WIDTH, CAM_HEIGHT);
    
	choosing_img = false;
	chosen_img = false;
    
    
    /*Eye Video*/
    
    vidPlayer.loadMovie("EyeCaptureSamples/Eye_Med_Movement_Lights.mov");
    vidPlayer.play();
    colorImage_Player.allocate(CAM_WIDTH,CAM_HEIGHT);

    grayImage_Player.allocate(CAM_WIDTH,CAM_HEIGHT);
    threshold = 80;
	
}

//--------------------------------------------------------------
void testApp::update(){
    
    /*Enviroment Camera*/
	int i;
	
    
	vidGrabber.update();
	if(vidGrabber.isFrameNew())
	{
		// get camera img into iplimage
		 colorImg.setFromPixels(vidGrabber.getPixels(), CAM_WIDTH,CAM_HEIGHT);;
		
		if (chosen_img) {
			grayImg = colorImg;
			detector.setImageSearch(grayImg.getCvImage());
			detector.update();
		}
		 
				
	}
    
    
    
    
    /*Eye Video*/
 
    vidPlayer.update();
    
    if (vidPlayer.isFrameNew()){
        /*Rotate Image*/
        unsigned char *p =  vidPlayer.getPixels();
        int n = width*height*3;
        for (int i=0; i<n/2; i+=3) {
            swap(p[i+0],p[n-i-3]);
            swap(p[i+1],p[n-i-2]);
            swap(p[i+2],p[n-i-1]);
        }
        
/*Detect Pupil*/
        colorImage_Player.setFromPixels(vidPlayer.getPixels(), width,height);
        
        colorImage_Player.resize(CAM_WIDTH,CAM_HEIGHT);
        grayImage_Player.resize(CAM_WIDTH,CAM_HEIGHT);
        
        grayImage_Player = colorImage_Player;
        
        grayImage_Player.threshold(threshold);
		//grayImage.adaptiveThreshold(threshold);
        
		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayImage_Player, 40, ((CAM_WIDTH+20)*CAM_HEIGHT)/2, 10, true);	/* find holes*/
        
        
    }
    
    /*Open CV Test*/
    
    
}




//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0,0,0);
	
	ofSetColor(255, 255, 255);
	/*Enviroment Camera*/
	// camera image
	vidGrabber.draw(0, 0,CAM_WIDTH,CAM_HEIGHT);
	
	// draw a rectanlge around the current selection
	if (choosing_img) {
		int x = mouseX;
		int y = mouseY;
		
		ofNoFill();
		ofRect(x_start < x ? x_start : x, 
			   y_start < y ? y_start : y, 
			   abs(x_start - x), 
			   abs(y_start - y));
		
	}
	
	if (chosen_img) {

		ofPushMatrix();
		ofTranslate(CAM_WIDTH, 0, 0);
		testImg.draw(0, 0);
		ofPopMatrix();
		
		ofSetColor(200, 20, 50);
		
		ofLine(detector.dst_corners[0].x, detector.dst_corners[0].y,
			   detector.dst_corners[1].x, detector.dst_corners[1].y);
		
		ofLine(detector.dst_corners[2].x, detector.dst_corners[2].y,
			   detector.dst_corners[1].x, detector.dst_corners[1].y);
		
		ofLine(detector.dst_corners[2].x, detector.dst_corners[2].y,
			   detector.dst_corners[3].x, detector.dst_corners[3].y);
		
		ofLine(detector.dst_corners[0].x, detector.dst_corners[0].y,
			   detector.dst_corners[3].x, detector.dst_corners[3].y);

	}
    
    
    /*Eye Video*/
    ofSetHexColor(0xffffff);
	
    colorImage_Player.draw(0,CAM_HEIGHT,CAM_WIDTH,CAM_HEIGHT);
    
    ofFill();
	ofSetHexColor(0x333333);
	ofRect(CAM_WIDTH,CAM_HEIGHT,CAM_WIDTH,CAM_HEIGHT);
	ofSetHexColor(0xffffff);
    
    for (int i = 0; i < contourFinder.nBlobs; i++){
        contourFinder.blobs[i].draw(0,0);
		contourFinder.blobs[i].draw(0,CAM_HEIGHT);
		// draw over the centroid if the blob is a hole
		ofSetColor(255);
		if(contourFinder.blobs[i].hole){
			ofDrawBitmapString("hole",
                               contourFinder.blobs[i].boundingRect.getCenter().x ,
                               contourFinder.blobs[i].boundingRect.getCenter().y + CAM_HEIGHT);
		}
        
        
        
        
    }

	
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){
	
	switch (key){			
		case 's':
			vidGrabber.videoSettings();
			break;
		case '1':
			break;
		case '2':
			break;
			
		case 'b':
			break;
			
	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
	// start a rectangle selection
	if(!choosing_img)
	{
		choosing_img = true;
		x_start = x;
		y_start = y;
	}
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
	// end the rectangle selection
	if (choosing_img) {
		choosing_img = false;
		x_end = x;
		y_end = y;
		
		if(x_start > x_end)
			std::swap(x_start, x_end);
		if(y_start > y_end)
			std::swap(y_start, y_end);
		
		int w = x_end - x_start;
		int h = y_end - y_start;
		
		cvSetImageROI(colorImg.getCvImage(), 
					  cvRect(x_start, 
							 y_start, 
							 w, h));
		
		if (testImg.bAllocated) {
			testImg.clear();
			testGrayImg.clear();
		}
		testImg.allocate(w, h);
		testGrayImg.allocate(w, h);
		testImg = colorImg;
		testGrayImg = testImg;
		cvResetImageROI(colorImg.getCvImage());

		detector.setImageTemplate(testGrayImg.getCvImage());
		chosen_img = true;
	}
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}
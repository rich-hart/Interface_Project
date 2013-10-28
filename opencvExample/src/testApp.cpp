#include "testApp.h"

/*
 Camera: ps3 eye
 640 x 480 at 60 frames/second
 320 x 240 at 120 frames/second
 */
extern const int width = 640;
extern const int height = 480;
extern const int window_width=320;
extern const int window_height=240;
//--------------------------------------------------------------
void testApp::setup(){

	#ifdef _USE_LIVE_VIDEO
        vidGrabber.setVerbose(true);
        vidGrabber.setDeviceID(1); //MacBook Pro Camera:0, USB Camera:1
        vidGrabber.setDesiredFrameRate(60);
        vidGrabber.initGrabber(width,height);
	#else
        vidPlayer.loadMovie("Eye_Med_Movement_Lights.mov");
        vidPlayer.play();
	#endif

    colorImg.allocate(width,height);
	grayImage.allocate(width,height);
	grayBg.allocate(width,height);
	grayDiff.allocate(width,height);

	bLearnBakground = true;
	threshold = 80;
}

//--------------------------------------------------------------
void testApp::update(){

    
	ofBackground(100,100,100);

    bool bNewFrame = false;

	#ifdef _USE_LIVE_VIDEO
       vidGrabber.update();
	   bNewFrame = vidGrabber.isFrameNew();
    #else
        vidPlayer.update();
        bNewFrame = vidPlayer.isFrameNew();
	#endif

	if (bNewFrame){

		#ifdef _USE_LIVE_VIDEO
            colorImg.setFromPixels(vidGrabber.getPixels(), width,height);
	    #else
            colorImg.setFromPixels(vidPlayer.getPixels(), width,height);
        #endif
        
        colorImg.resize(window_width,window_height);
        grayImage.resize(window_width,window_height);
        grayBg.resize(window_width,window_height);
        grayDiff.resize(window_width,window_height);
        
        
        grayImage = colorImg;
		if (bLearnBakground == true){
			grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
			bLearnBakground = false;
		}

		// take the abs value of the difference between background and incoming and then threshold:
		grayDiff.absDiff(grayBg, grayImage);
		grayDiff.threshold(threshold);

		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayDiff, 20, ((width+20)*height)/3, 10, true);	// find holes
        
        
	}

}

//--------------------------------------------------------------
void testApp::draw(){


    
	// draw the incoming, the grayscale, the bg and the thresholded difference
	ofSetHexColor(0xffffff);
    
	colorImg.draw(20,20);
	grayImage.draw(window_width+40,20);
	grayBg.draw(20,window_height+40);
	grayDiff.draw(window_width+40,window_height+40);

	// then draw the contours:

	ofFill();
	ofSetHexColor(0x333333);
	ofRect(window_width+40,window_height+300,window_width,window_height);
	ofSetHexColor(0xffffff);

	// we could draw the whole contour finder
	//contourFinder.draw(width+40,540);

	// or, instead we can draw each blob individually from the blobs vector,
	// this is how to get access to them:
    for (int i = 0; i < contourFinder.nBlobs; i++){
        contourFinder.blobs[i].draw(window_width+40,window_height+300);
		
		// draw over the centroid if the blob is a hole
		ofSetColor(255);
		if(contourFinder.blobs[i].hole){
			ofDrawBitmapString("hole",
				contourFinder.blobs[i].boundingRect.getCenter().x + window_width+40,
				contourFinder.blobs[i].boundingRect.getCenter().y + window_height+300);
		}
    }

	// finally, a report:
	ofSetHexColor(0xffffff);
	stringstream reportStr;
	reportStr << "bg subtraction and blob detection" << endl
			  << "press ' ' to capture bg" << endl
			  << "threshold " << threshold << " (press: +/-)" << endl
			  << "num blobs found " << contourFinder.nBlobs << ", fps: " << ofGetFrameRate();
	ofDrawBitmapString(reportStr.str(), 20, 600);

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	switch (key){
		case ' ':
			bLearnBakground = true;
			break;
        case '=':
		case '+':
			threshold ++;
			if (threshold > 255) threshold = 255;
			break;
		case '-':
			threshold --;
			if (threshold < 0) threshold = 0;
			break;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

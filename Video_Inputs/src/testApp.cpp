#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    camWidth 		= 320;	// try to grab at this size.
	camHeight 		= 240;

    vidGrabber.setVerbose(true);
    vidGrabber.setDeviceID(0); //MacBook Pro Camera:0, USB Camera:1
    vidGrabber.setDesiredFrameRate(30);
    vidGrabber.initGrabber(camWidth,camHeight);

    //videoInverted 	= new unsigned char[camWidth*camHeight*3];
	//videoTexture.allocate(camWidth,camHeight, GL_RGB);
	ofSetVerticalSync(true);
    
    
    
    
    
    vidPlayer.loadMovie("EyeCaptureSamples/Eye_Med_Movement_Lights.mov");
    vidPlayer.play();


}

//--------------------------------------------------------------
void testApp::update(){

    ofBackground(100,100,100);
	
	vidGrabber.update();
    vidPlayer.update();
	/*
	if (vidGrabber.isFrameNew()){
		int totalPixels = camWidth*camHeight*3;
		unsigned char * pixels = vidGrabber.getPixels();
		for (int i = 0; i < totalPixels; i++){
			videoInverted[i] = 255 - pixels[i];
		}
		videoTexture.loadData(videoInverted, camWidth,camHeight, GL_RGB);
	}
     */
    
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetHexColor(0xffffff);
	vidGrabber.draw(20,20);

	//videoTexture.draw(20+camWidth,20,camWidth,camHeight);
    vidPlayer.draw(20+camWidth,20,camWidth,camHeight);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

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
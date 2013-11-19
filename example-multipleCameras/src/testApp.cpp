#include "testApp.h"


unsigned long EYE_CAMERA_ID = 605224960; // Using port A (MacMini Mid 2011) for this example
unsigned long FRONT_CAMERA_ID = 605290496;
//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	
	ofSetLogLevel(OF_LOG_VERBOSE);	
	
	vector<ofxMacamPs3EyeDeviceInfo*> deviceList = ofxMacamPs3Eye::getDeviceInfoList();
	/*Enviroment Camera*/
	for (int i = 0; i < deviceList.size(); i++) {
       
		ofxMacamPs3Eye * camera = new ofxMacamPs3Eye();
		camera->setDeviceID(deviceList[i]->id);
		camera->setDesiredFrameRate(180);
		camera->initGrabber(CAM_WIDTH, CAM_HEIGHT);
        camera->setUseTexture(true);
            
		cameras.push_back(camera);
        
         if(deviceList[i]->locationID == FRONT_CAMERA_ID){
            // vidInput = camera;
             cout<< "Front Camera: " <<deviceList[i]->locationID<<endl;
         }
        
        if(deviceList[i]->locationID == EYE_CAMERA_ID){
          //  vidInput = camera;
            cout<< "Front Camera: " <<deviceList[i]->locationID<<endl;
        }
	}
    
    
    // allocate stuff
	colorImg.allocate(CAM_WIDTH, CAM_HEIGHT);
	grayImg.allocate(CAM_WIDTH, CAM_HEIGHT);
    
	choosing_img = false;
	chosen_img = false;

    
    
    /*Eye Camera*/
    /*
    for (int i = 0; i < deviceList.size(); i++) {
        if(deviceList[i]->locationID == EYE_CAMERA_ID){
            ofxMacamPs3Eye * camera = new ofxMacamPs3Eye();
            camera->setDeviceID(deviceList[i]->id);
            camera->setDesiredFrameRate(180);
            camera->initGrabber(CAM_WIDTH, CAM_HEIGHT);
            cameras.push_back(camera);
            cout<< "Eye Camera: " <<deviceList[i]->locationID<<endl;
        }
	}
     */
	
	if(cameras.size() > 0){
		ofSetWindowShape(CAM_WIDTH * cameras.size(), CAM_HEIGHT);
	}
}

//--------------------------------------------------------------
void testApp::update(){
	for (int i = 0; i < cameras.size(); i++) {
		cameras[i]->update();
	}
	
}

//--------------------------------------------------------------
void testApp::draw(){
	for (int i = 0; i < cameras.size(); i++) {
		cameras[i]->draw(i * cameras[i]->getWidth(),0);
		ofDrawBitmapString(ofToString(cameras[i]->getRealFrameRate()), i * cameras[i]->getWidth() + 20, 20);
	}
	
	if(cameras.size() == 0){
		ofDrawBitmapString("No PS3Eye found. :(", 20, 20);
	}
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
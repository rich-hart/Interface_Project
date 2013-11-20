#include "testApp.h"

unsigned long FRONT_CAMERA_ID = 605290496; // Using port A (MacMini Mid 2011) for this example
unsigned long EYE_CAMERA_ID = 605224960;

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	
	ofSetLogLevel(OF_LOG_VERBOSE);	
	
	vector<ofxMacamPs3EyeDeviceInfo*> deviceList = ofxMacamPs3Eye::getDeviceInfoList();
	for (int i = 0; i < deviceList.size(); i++) {
    cout<< deviceList[i]->locationID<<endl;
    }
    
	for (int i = 0; i < deviceList.size(); i++) {
		ofxMacamPs3Eye * camera = new ofxMacamPs3Eye();
		camera->setDeviceID(deviceList[i]->id);
		camera->setDesiredFrameRate(FRAME_RATE);
		camera->initGrabber(CAM_WIDTH, CAM_HEIGHT);
		cameras.push_back(camera);
        if(deviceList[i]->locationID == EYE_CAMERA_ID){
            vidInput=camera;
        }
        
        if(deviceList[i]->locationID == FRONT_CAMERA_ID){
            vidPlayer=camera;
        }

        
	}
	
    //Window Setup
	if(cameras.size() > 0){
		ofSetWindowShape(CAM_WIDTH * cameras.size(), CAM_HEIGHT * 2 );
	}
    
    
	// allocate stuff
	colorImg.allocate(CAM_WIDTH, CAM_HEIGHT);
	grayImg.allocate(CAM_WIDTH, CAM_HEIGHT);
    
	choosing_img = false;
	chosen_img = false;
    
    
    
    threshold = 80;
	eye_radius=25;
    
    
    
    colorImage_Player.allocate(CAM_WIDTH,CAM_HEIGHT);
    
    grayImage_Player.allocate(CAM_WIDTH,CAM_HEIGHT);
    
    /*
     Eye and enviroment interaction.
     
     
     */
    select_time = 3;
    start_selection_time = clock();
    
    
    /*Arduino*/
    
    
    // replace the string below with the serial port for your Arduino board
    // you can get this from the Arduino application or via command line
    // for OSX, in your terminal type "ls /dev/tty.*" to get a list of serial devices
	ard.connect("/dev/cu.usbmodem24121", 57600);
	
	// listen for EInitialized notification. this indicates that
	// the arduino is ready to receive commands and it is safe to
	// call setupArduino()
	ofAddListener(ard.EInitialized, this, &testApp::setupArduino);
	bSetupArduino	= false;	// flag so we setup arduino when its ready, you don't need to touch this :)
    pin_on = false;
    
    
}

//--------------------------------------------------------------
void testApp::update(){
	for (int i = 0; i < cameras.size(); i++) {
		cameras[i]->update();
	}
    
    if(vidInput->isFrameNew()){
        // get camera img into iplimage
		colorImg.setFromPixels(vidInput->getPixels(), CAM_WIDTH, CAM_HEIGHT);
		
		if (chosen_img) {
			grayImg = colorImg;
			detector.setImageSearch(grayImg.getCvImage());
			detector.update();
		}
    }
    
    
    
    
    if (vidPlayer->isFrameNew()){
        
        /*Rotate Image*/
        
        
        /*
        unsigned char *p =  vidPlayer->getPixels();
        int n = CAM_WIDTH*CAM_HEIGHT*3;
        for (int i=0; i<n/2; i+=3) {
            swap(p[i+0],p[n-i-3]);
            swap(p[i+1],p[n-i-2]);
            swap(p[i+2],p[n-i-1]);
        }
         */
        
        /*Detect Pupil*/
        colorImage_Player.setFromPixels(vidPlayer->getPixels(), CAM_WIDTH,CAM_HEIGHT);
        
        // colorImage_Player.resize(CAM_WIDTH,CAM_HEIGHT);
        //  grayImage_Player.resize(CAM_WIDTH,CAM_HEIGHT);
        
        grayImage_Player = colorImage_Player;
        
        grayImage_Player.threshold(threshold);
		//grayImage.adaptiveThreshold(threshold);
        
		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayImage_Player, 40, ((CAM_WIDTH+20)*CAM_HEIGHT)/2, 10, true);	/* find holes*/
        
        
    }
    
    /*Arduino*/
    updateArduino();

	
}
//--------------------------------------------------------------
void testApp::setupArduino(const int & version) {
	
	// remove listener because we don't need it anymore
	ofRemoveListener(ard.EInitialized, this, &testApp::setupArduino);
    
    // it is now safe to send commands to the Arduino
    bSetupArduino = true;
    
    
   
    // set pin D11 as PWM (analog output)
    ard.sendDigitalPinMode(11, ARD_PWM);
    
    
}

//--------------------------------------------------------------
void testApp::updateArduino(){
    
	// update the arduino, get any data or messages.
    // the call to ard.update() is required
	ard.update();
	
	// do not send anything until the arduino has been set up
	if (bSetupArduino) {
        // fade the led connected to pin D11
        if(pin_on){
		ard.sendPwm(11, (int)255);   // pwm...
        } else{
            ard.sendPwm(11, (int) 0 );   // pwm...
        }
	}
    
}


//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0,0,0);
	
	ofSetColor(255, 255, 255);
    
    
	for (int i = 0; i < cameras.size(); i++) {
		cameras[i]->draw(i * cameras[i]->getWidth(),0);
		ofDrawBitmapString(ofToString(cameras[i]->getRealFrameRate()), i * cameras[i]->getWidth() + 20, 20);
	}
	
	if(cameras.size() == 0){
		ofDrawBitmapString("No PS3Eye found. :(", 20, 20);
	}
    
    
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
    
    
     obj_detected= false;
    if (chosen_img) {
        
		ofPushMatrix();
		ofTranslate(0, CAM_HEIGHT, 0);
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
        
        
        obj_center_x = (detector.dst_corners[0].x +detector.dst_corners[1].x +detector.dst_corners[2].x +detector.dst_corners[3].x
                        )/4;
        obj_center_y = (detector.dst_corners[0].y +detector.dst_corners[1].y +detector.dst_corners[2].y +detector.dst_corners[3].y
                        )/4;

        
           obj_detected = true;
        
	}
    
    
    
    
    
    ofSetColor(255, 255, 255);
    
    /*Eye Video*/
    eye_detected=false;
    ofSetHexColor(0xffffff);
	
    //colorImage_Player.draw(CAM_WIDTH,0);
    
    /*
    ofFill();
	ofSetHexColor(0x333333);
	ofRect(CAM_WIDTH,CAM_HEIGHT,CAM_WIDTH,CAM_HEIGHT);
	ofSetHexColor(0xffffff);
    */
    
    //below is modified code to find the largest hole in the eye camera.
    max_blob_index = -1;
    float max_area = -1;
    for (int i = 0; i < contourFinder.nBlobs; i++){
        if(max_area<contourFinder.blobs[i].area && contourFinder.blobs[i].hole){
            max_area=contourFinder.blobs[i].area;
            max_blob_index=i;
        }
    }
    
    
    // for (int i = max_blob_index; i == max_blob_index; i++){ // this will find the largest whole
    
    for (int i = 0; i < contourFinder.nBlobs; i++){  // this will intereate over all the blobs
        
		if(contourFinder.blobs[i].hole && i==max_blob_index){
            //contourFinder.blobs[i].draw(0,0);
            contourFinder.blobs[i].draw(CAM_WIDTH,0);
            // draw over the centroid if the blob is a hole
            ofSetColor(255);
            eye_center_x=contourFinder.blobs[i].boundingRect.getCenter().x;
            eye_center_y=contourFinder.blobs[i].boundingRect.getCenter().y;
			ofDrawBitmapString("hole",
                               eye_center_x +CAM_WIDTH ,
                               eye_center_y );
            
            
            eye_detected=true;
            
        }
        
        
        
        
    }
    
    obj_highlight= false;
    if(obj_detected&&eye_detected){
        
        obj_highlight = (pow(obj_center_x-eye_center_x,2.0)+pow(obj_center_y-eye_center_y,2.0)<pow(eye_radius,2.0));
        if(obj_highlight){
            ofSetColor(255);
        }
        else{
            ofSetColor(200, 20, 50);
        }
        ofDrawBitmapString("object center", obj_center_x ,obj_center_y);
        ofFill();		// draw "filled shapes"
        ofCircle(obj_center_x,obj_center_y,10);
        
        if(obj_highlight){
            ofSetColor(255);
        }
        else{
            ofSetColor(20,200,200);
        }
        
        for (int i = 0; i < contourFinder.nBlobs; i++){  // this will intereate over all the blobs
            
            if(contourFinder.blobs[i].hole && i==max_blob_index){
                ofDrawBitmapString("eye",
                                   eye_center_x ,
                                   eye_center_y);
                // if(){
                ofNoFill();
                ofCircle(eye_center_x, eye_center_y,eye_radius);
                //  }
            }
        }
        
        
    }
    if(obj_highlight){
        if(clock()-start_selection_time > CLOCKS_PER_SEC * select_time){
            
            //INTERACT WITH OBJECT!!!
            pin_on = !pin_on;
            
            ofSetColor(200, 20, 50);
            ofFill();
            ofCircle(obj_center_x, obj_center_y,5);
            start_selection_time= clock();
        }
    } else{
        start_selection_time= clock();
        
    }
    
    // finally, a report:
	ofSetHexColor(0xffffff);
	stringstream reportStr;
	reportStr << "threshold " << threshold << " (press: =/-)" << endl;
	ofDrawBitmapString(reportStr.str(), CAM_WIDTH, CAM_HEIGHT);


    
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	switch (key){
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
void testApp::mouseMoved(int x, int y){

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

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
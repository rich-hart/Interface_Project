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

void testApp::setup(){
	

	svmImg.allocate(CAM_WIDTH, CAM_HEIGHT);
svmImg_calc.allocate(CAM_WIDTH, CAM_HEIGHT);
	
    // Data for visual representation
    int width = CAM_WIDTH, height = CAM_HEIGHT;
    image = Mat::zeros(height, width, CV_8UC3);

    
   // svmImg_calc.allocate(height, width);
    //svmImg_calc
    // Set up training data
    float labels[4] = {1.0, -1.0, -1.0, -1.0};
    Mat labelsMat(4, 1, CV_32FC1, labels);
    
    float trainingData[4][2] = { {501, 10}, {255, 10}, {501, 255}, {10, 501} };
    Mat trainingDataMat(4, 2, CV_32FC1, trainingData);
    
    // Set up SVM's parameters
    CvSVMParams params;
    params.svm_type    = CvSVM::C_SVC;
    params.kernel_type = CvSVM::LINEAR;
    params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);
    
    // Train the SVM
    CvSVM SVM;
    SVM.train(trainingDataMat, labelsMat, Mat(), Mat(), params);
    
    Vec3b green(0,255,0), blue(255,0,0);
    // Show the decision regions given by the SVM
    for (int i = 0; i < image.rows; ++i)
        for (int j = 0; j < image.cols; ++j)
        {
            Mat sampleMat = (Mat_<float>(1,2) << i,j);
            float response = SVM.predict(sampleMat);
            
            if (response == 1)
                image.at<Vec3b>(j, i)  = green;
            else if (response == -1)
                image.at<Vec3b>(j, i)  = blue;
        }
    
    // Show the training data
    int thickness = -1;
    int lineType = 8;
    circle( image, cvPoint(501,  10), 5, Scalar(  0,   0,   0), thickness, lineType);
    circle( image, cvPoint(255,  10), 5, Scalar(255, 255, 255), thickness, lineType);
    circle( image, cvPoint(501, 255), 5, Scalar(255, 255, 255), thickness, lineType);
    circle( image, cvPoint( 10, 501), 5, Scalar(255, 255, 255), thickness, lineType);
    
    // Show support vectors
    thickness = 2;
    lineType  = 8;
    int c     = SVM.get_support_vector_count();
    
    for (int i = 0; i < c; ++i)
    {
        const float* v = SVM.get_support_vector(i);
        circle( image,  cvPoint( (int) v[0], (int) v[1]),   6,  Scalar(128, 128, 128), thickness, lineType);
    }
    
    //drawMat(image,0,0);
       //svmImg_calc.setFromPixels(image.ptr(),CAM_WIDTH,CAM_HEIGHT);
   // svmImg_calc.draw(0, 0);
   // ofBackground(0,0,0);
	
	//ofSetColor(255, 255, 255);
	
	// camera image
	


}

//--------------------------------------------------------------
void testApp::update(){
	
    
}

//--------------------------------------------------------------
void testApp::draw(){
		
	
	
	
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){
	

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
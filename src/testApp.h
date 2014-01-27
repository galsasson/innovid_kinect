#pragma once

#include <vector>

#include "ofMain.h"
#include "ofxKinect.h"
#include "ofxOpenCv.h"
#include "ofxUI.h"
#include "OpticalFlow.h"
#include "ofxOsc.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
    ofxKinect kinect;
    float kinectAngle;
    
    ofxCvGrayscaleImage depthImg;
    ofxCvGrayscaleImage oldDepthImg;
    ofxCvGrayscaleImage diffImg;
    
    ofxCvColorImage colorImg;
    
    // Optical Flow
    OpticalFlow oFlow;
    vector<FlowVector> flowVectors;
    int kernelSize;
    float flowMagThresh;
    int flowBlur;
    
    
    // GUI
    ofxUICanvas *gui;
    void setupGui();
    
    int farThreshold;
    int erodeAmount;
    
    
    // OSC
    ofxOscSender oscSend;
    void oscSendForce(const ofVec2f& origin, const ofVec2f& force);
};

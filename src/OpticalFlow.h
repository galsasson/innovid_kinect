//
//  OpticalFlow.h
//  innovid_kinect
//
//  Created by Gal Sasson on 1/26/14.
//
//

#ifndef __innovid_kinect__OpticalFlow__
#define __innovid_kinect__OpticalFlow__

#include <iostream>
#include <vector>
#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"

class FlowVector;

class OpticalFlow
{
public:
    OpticalFlow();
    
    void setup(int w, int h);
    void update(ofxCvImage& img, float blurAmount, float lengthThresh, int kernelSize = 4);
    void draw(int x, int y, int w, int h);
    ofVec2f getFlowPositionAt(int x, int y);
    ofVec2f getFlowOffsetAt(int x, int y);

    vector<FlowVector>& getFlowVectors();
    ofVec2f sourceSize, resultSize;

private:
    // Optical Flow
    ofxCv::FlowFarneback farneback;
    vector<FlowVector> flowVectors;
//    ofxCvColorImage colorImg;
    ofxCvColorImage flowImg;
    

};

class FlowVector
{
public:
    FlowVector(float x, float y, float ox, float oy) {
        origin = ofVec2f(x, y);
        offset = ofVec2f(ox, oy);
    }
    
    ofVec2f origin;
    ofVec2f offset;
};


#endif /* defined(__innovid_kinect__OpticalFlow__) */

//
//  OpticalFlow.cpp
//  innovid_kinect
//
//  Created by Gal Sasson on 1/26/14.
//
//

#include "OpticalFlow.h"

OpticalFlow::OpticalFlow()
{

}

void OpticalFlow::setup(int w, int h)
{
    sourceSize = ofVec2f(w, h);
    resultSize = ofVec2f(w/4, h/4);
    
    flowImg.allocate(resultSize.x, resultSize.y);
    
//    curFlow = &farneback;
    
//    farneback.setPyramidScale(1);
//    farneback.setPolyN(1);
//    farneback.setNumIterations(1);
//    farneback.setNumLevels(1);
//    farneback.setPyramidScale(0.01);
}

void OpticalFlow::update(ofxCvImage& img, float blurAmount, float lengthThresh, int kernelSize)
{
//    colorImg.setFromPixels(pixels, colorImg.width, colorImg.height);
    img.blur(blurAmount);
    flowImg.scaleIntoMe(img);

    farneback.calcOpticalFlow(flowImg.getPixelsRef());
    
    // get all motion vectors
    int kSize = kernelSize;
    flowVectors.clear();
    for (int y=0; y<flowImg.height; y+=kSize)
    {
        for (int x=0; x<flowImg.width; x+=kSize)
        {
            ofVec2f f;
            for (int i=0; i<kSize; i++) {
                for (int j=0; j<kSize; j++)
                {
                    f += farneback.getFlowOffset(x+j, y+i);
                }
            }
            f /= kSize*kSize;
            if (f.length() > lengthThresh)
            {
                flowVectors.push_back(FlowVector(x+kSize/2, y+kSize/2, f.x, f.y));
            }
        }
    }
}

void OpticalFlow::draw(int x, int y, int w, int h)
{
    farneback.draw(x, y, w, h);
}

ofVec2f OpticalFlow::getFlowPositionAt(int x, int y)
{
    return farneback.getFlowPosition(x, y);
}

ofVec2f OpticalFlow::getFlowOffsetAt(int x, int y)
{
    return farneback.getFlowOffset(x, y);
}

vector<FlowVector>& OpticalFlow::getFlowVectors()
{
    return flowVectors;
}

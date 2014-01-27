#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
    // start OSC
    oscSend.setup("localhost", 10000);
    
    // enable depth->video image calibration
	kinect.setRegistration(true);

    // only depth, no RGB
    kinect.init();
    
	kinect.open();		// opens first available kinect

    // set all the way down tilt on startup
	kinectAngle = -20;
	kinect.setCameraTiltAngle(kinectAngle);
    
    colorImg.allocate(kinect.width, kinect.height);
    depthImg.allocate(kinect.width, kinect.height);
    oldDepthImg.allocate(kinect.width, kinect.height);
    diffImg.allocate(kinect.width, kinect.height);
    
    oFlow.setup(kinect.width, kinect.height);
    
    setupGui();
}

//--------------------------------------------------------------
void testApp::update(){
    kinect.update();
    if (kinect.isFrameNew()) {
        // TODO: remove background by ANDING the threshold depth with the color image
        
        colorImg.setFromPixels(kinect.getPixels(), kinect.width, kinect.height);
        colorImg.mirror(false, true);
        oFlow.update(colorImg, flowBlur, flowMagThresh, kernelSize);

        // send flow vectors through OSC
        vector<FlowVector> flowVectors = oFlow.getFlowVectors();
        for (int i=0; i<flowVectors.size(); i++)
        {
            ofVec2f origin = flowVectors[i].origin / oFlow.resultSize;
            ofVec2f force = flowVectors[i].offset / oFlow.resultSize;
            oscSendForce(origin, force);
        }

//        cout<<"start: "<<ofGetElapsedTimeMillis()<<endl;

//        flowFbo.begin();
//        kinect.draw(0, 0, kinect.getWidth()/16, kinect.getHeight()/16);
//        flowFbo.end();
        
        
        
        // get motion out of the kinect depth
//        depthImg.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
//        depthImg.threshold(farThreshold);
//
//        depthImg.blur(5);

//        flowImg.scaleIntoMe(depthImg);
        
        
        
        
        // calc diff image
//        diffImg.absDiff(depthImg, oldDepthImg);
//        for (int i=0; i<erodeAmount; i++) {
//            diffImg.erode_3x3();
//        }
//
        
//        oldDepthImg = depthImg;
//        cout<<"end: "<<ofGetElapsedTimeMillis()<<endl;
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    ofClear(0);
    
    
    // draw optical flow
    ofPushMatrix();
    ofPushStyle();
    ofTranslate(200, 0);
//    ofScale(0.5, 0.5);
    colorImg.draw(0, 0);
    ofSetColor(255);
    
    vector<FlowVector> flowVectors = oFlow.getFlowVectors();
    for (int i=0; i<flowVectors.size(); i++)
    {
        ofVec2f origin = flowVectors[i].origin * 4;
        ofVec2f target = (flowVectors[i].origin + flowVectors[i].offset) * 4;
        ofLine(origin, target);
    }
    
//    cout<<"flow vectors = "<<flowVectors.size()<<endl;
//    oFlow.draw(320, 0, 320, 240);
    ofPopStyle();
    ofPopMatrix();
    
//    depthImg.draw(0, 0, 320, 240);
//    diffImg.draw(0, 240);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == 's')
    {
        gui->saveSettings("GUI/guiSettings.xml");
    }
    else if (key == 'l')
    {
        gui->loadSettings("Gui/guiSettings.xml");
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

void testApp::setupGui()
{
    gui = new ofxUICanvas(0, 0, 200, ofGetHeight());
    
    gui->addWidgetDown(new ofxUILabel("Innovid Kinect", OFX_UI_FONT_MEDIUM));
    
    farThreshold = 0;
    gui->addIntSlider("Far Threshold", 0, 255, &farThreshold);
    erodeAmount = 2;
    gui->addIntSlider("Erode Diff", 0, 10, &erodeAmount);
    flowMagThresh = 2;
    gui->addSlider("Flow Mag Threshold", 0, 10, &flowMagThresh);
    flowBlur = 3;
    gui->addIntSlider("Flow Blur", 0, 50, &flowBlur);
    kernelSize = 4;
    gui->addIntSlider("Kernel Size", 1, 20, &kernelSize);
    
    gui->loadSettings("GUI/guiSettings.xml");

}

void testApp::oscSendForce(const ofVec2f& origin, const ofVec2f& force)
{
    ofxOscMessage msg;
    msg.setAddress("/iv/force");
    msg.addFloatArg(origin.x);
    msg.addFloatArg(origin.y);
    msg.addFloatArg(force.x);
    msg.addFloatArg(force.y);
    oscSend.sendMessage(msg);
}
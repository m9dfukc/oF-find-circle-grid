#include "ofApp.h"
#include "ofxPS3EyeGrabber.h"

//--------------------------------------------------------------
void ofApp::setup(){   
    cam.setGrabber(std::make_shared<ofxPS3EyeGrabber>());
    cam.setup(640, 480);
    cam.getGrabber<ofxPS3EyeGrabber>()->setAutoWhiteBalance(true);
    cam.getGrabber<ofxPS3EyeGrabber>()->setAutogain(true);
    processed.allocate(cam.getWidth(), cam.getHeight(), OF_IMAGE_GRAYSCALE);
    
    bPatternFound = false;
    patternSize = cv::Size(4, 5);
    
    gui.setup("", "settings.xml");
    gui.add(squareSize.set("Square Size", 30, 10, 70));
    gui.add(circleSize.set("Circle Size", 5, 2, 15));
    gui.add(threshold.set("Threshold", 120, 0, 255));
    gui.loadFromFile("settings.xml");
    
    squareSize.addListener(this, &ofApp::squareSizeParamChanged);
    
    setupCandidateImagePoints();
}

//--------------------------------------------------------------
void ofApp::setupCandidateImagePoints(){
    candidateImagePoints.clear();
    Point2f p;
    for(int i = 0; i < patternSize.height; i++) {
        for(int j = 0; j < patternSize.width; j++) {
            p.x = float(((2 * j) + (i % 2)) * (float)squareSize);
            p.y = float(i * (float)squareSize);
            candidateImagePoints.push_back(p);
        }
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    cam.update();
    if (cam.isFrameNew()) {
        cv::Mat camImg = toCv(cam);
        cv::Mat processedImg = toCv(processed);
        processImage(camImg, processedImg, threshold);
        processed.update();
        
        vector<cv::Point2f> circlesImgPts;
        bPatternFound = cv::findCirclesGrid(processedImg, patternSize, circlesImgPts, cv::CALIB_CB_ASYMMETRIC_GRID);
    }
}

void ofApp::processImage(cv::Mat in, cv::Mat & out, int threshold){
    cv::Mat processedImg;
    if(in.type() != CV_8UC1) {
        cvtColor(in, processedImg, CV_RGB2GRAY);
    } else {
        processedImg = in;
    }
    cv::threshold(processedImg, out, threshold, 255, cv::THRESH_BINARY_INV);
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    ofSetColor(255);
    cam.draw(0, 0);
    processed.draw(cam.getWidth(), 0);
    gui.draw();
    if (bPatternFound) {
        ofSetColor(0, 255, 0);
        ofDrawBitmapString("Pattern found", 10, gui.getHeight() + 30);
    }
}

//--------------------------------------------------------------
void ofApp::drawProjector(ofEventArgs & args) {
    ofBackground(0);
    
    ofSetColor(0, 255, 0);
    if (bPatternFound) ofDrawBitmapString("Pattern found", 20, 30);

    ofSetColor(255);
    float offsetX = (PROJECTOR_WIDTH / 2.0) - (patternSize.width * squareSize / 2.0);
    float offsetY = (PROJECTOR_HEIGHT / 2.0) - (patternSize.height * squareSize / 2.0);
    for(const auto & p : candidateImagePoints) {
        ofDrawCircle(p.x + offsetX, p.y + offsetY, circleSize);
    }
}

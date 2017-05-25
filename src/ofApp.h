#pragma once

#define PROJECTOR_WIDTH 800
#define PROJECTOR_HEIGHT 600

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"

using namespace std;
using namespace cv;
using namespace ofxCv;

class ofApp : public ofBaseApp{

public:
    void setup();
    void setupCandidateImagePoints();
    void update();
    void draw();
    void drawProjector(ofEventArgs & args);
    void processImage(cv::Mat in, cv::Mat & out, int threshold);
    void squareSizeParamChanged(int & value) { setupCandidateImagePoints(); };
    
    cv::Size patternSize;
    cv::Point2f patternPosition;
    vector<cv::Point2f> candidateImagePoints;
    
    ofVideoGrabber cam;
    ofImage processed;
    ofxPanel gui;
    ofParameter<int> threshold;
    ofParameter<int> squareSize;
    ofParameter<int> circleSize;
    bool bPatternFound;
    
};

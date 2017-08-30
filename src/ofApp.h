#pragma once

#include "ofMain.h"
#include "Agent.hpp"
#include "ofxGui.h"
#include "ofxOsc.h"

struct rect{
    ofRectangle rectangle;
    ofColor color;
    ofVec2f position;
  
};


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    
    void addAgents(int num);
    void addBranch();
    void addAllBranches();
    void removeAgents(int num);
    
    
    
    
    
    ofEasyCam camera;
    float camDist;
    
    int numAgents;
    int renderdAgents;
    
    int targetAgentsNum;
    
    int totalNumAgents;
    int minNumAgents;
    
    vector<shared_ptr<Agent> > agents;
    vector <ofVec3f > points;
    vector <ofVec3f > sizes;
    vector <int > colors;
    

    
    
    ofVbo vbo;
    ofShader shader;
    
    ofTexture texture;
    
    ofFbo fbo;
    

    int pointsize;
    bool bHide;

    ofxFloatSlider homeforce;
    ofxFloatSlider wanderforce;
    ofxFloatSlider rotationforce;
    ofxFloatSlider seekforce;

    
    
    ofxFloatSlider wanderR;
    ofxFloatSlider wanderD;
    ofxFloatSlider change;
    ofxFloatSlider maxspeed;
    
    ofxFloatSlider targetPointSize;


    
    
    ofxFloatSlider maxvol;
    
    ofxColorSlider color;
    ofxPanel gui;
    
    
    ofxFloatSlider fadespeed;
    
    vector<ofPolyline> polylines;
    ofPolyline polyline;
    
    int colorangle=0;
    
    ofVec2f gutter;
    void drawGutter();
    vector <rect> rects;
    void drawRects();
    
    
    void shrink();

    
    //OSC
    
    ofxOscSender sender;
    //you can set up a sender!
    //We are going to use this network connection to give us
    //some visual feedback of our current game values.
    
    ofxOscReceiver receiver;
    //this is the magic! This is the port on which your game gets incoming data.
    
    ofxOscMessage m;
    //this is the osc message your application gets from your device.
    
};

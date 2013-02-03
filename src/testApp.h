#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxOsc.h"
#include "ofxTimer.h"

#define NUM_MSG_STRINGS 15

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
        void timerCallback( ofEventArgs &e );
        void guiEvent( ofxUIEventArgs &e );
        void proceedLevel( int upDown , int _nextLevel );
        float timeDiffToNextLevel( int _current, int _next );
        void reset();
        void pompTakeRest();
		
    private:
        ofxUICanvas *gui;
        ofSerial serial;
        ofxOscReceiver reciver;
        ofxTimer timer;
        bool run;
        bool isReset;
        bool busy;
    
        int currentLevel;
    
    
        int current_msg_string;
        string msg_strings[NUM_MSG_STRINGS];
        float timers[NUM_MSG_STRINGS];
    
};

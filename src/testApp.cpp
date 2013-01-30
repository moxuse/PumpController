#include "testApp.h"

#define RECIVE_PORT 5000
#define BAUD_RATE 9600


bool togBusyBlink = false;
//--------------------------------------------------------------
void testApp::setup(){
    run = true;
    busy = false;
    serial.enumerateDevices();
    serial.setup( "/dev/tty.usbmodem1411", BAUD_RATE );
    
    reciver.setup( RECIVE_PORT );
    
    timer.setup(1000, false);
    
    gui = new ofxUICanvas();
    
    gui->addWidgetDown(new ofxUILabel("Pomp Controller for yebizo 2013", OFX_UI_FONT_LARGE));
    gui->addToggle("Recive osc and run", true, 30, 30);
    gui->addButton("Reset", isReset, 30, 30);
    gui->addLabel("CurrentLevel", "CurrentLevel : 0");
    ofAddListener(gui->newGUIEvent,this,&testApp::guiEvent);
    
    ofAddListener( timer.TIMER_REACHED, this, &testApp::timerCallback );
}

//--------------------------------------------------------------
void testApp::update(){
    
    while ( reciver.hasWaitingMessages() ) {
        ofxOscMessage m;
        reciver.getNextMessage( &m );
        
        if( "/recive" == m.getAddress() ) {
            if( false == busy ) {
                int nextLevel = m.getArgAsInt32(0);
                busy = true;
                proceedLevel( nextLevel );
                if( !run ){
                    cout << "recived OSC message but not running flag.." << endl;
                }
            }
        }
        
    }
    
    if( 0 == ofGetFrameNum()%300 ){
        if( togBusyBlink ) {
            togBusyBlink = false;
        } else {
            togBusyBlink = true;
        }
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    
    ofBackground(0);
    
    if( busy && togBusyBlink) {
        ofPushStyle();
        ofSetColor(0, 255, 255);
        ofRect(5, 150, 230, 30);
        ofPopStyle();
    }

}

//--------------------------------------------------------------
void testApp::timerCallback( ofEventArgs &e ) {
    int count;
    count = timer.count;
    
    pompTakeRest();
    
    cout << "timer recieved _________ " << &e << " ms : " << count << endl;
    cout << "now currentLevel is _________ " << currentLevel << endl;
    
    ofxUILabel *label = ( ofxUILabel *)( gui->getWidget("CurrentLevel") );
    char result[100];
    sprintf( result, "CurrentLevel : %d", currentLevel );
    label -> setLabel( result );
}

//--------------------------------------------------------------
void testApp::guiEvent( ofxUIEventArgs &e ){
    string name = e.widget->getName();
    int kind = e.widget->getKind();
    
    if( "Recive osc and run" == name ){
        ofxUIToggle *tog = (ofxUIToggle *) e.widget;
        run = tog->getValue();
        
//        if( !run ){
//            timer.pauseTimer();
//        } else {
//            timer.resumeTimer();
//        }
        
    } else if ( "Reset" == name ){
        reset();
    }
}

//--------------------------------------------------------------
void testApp::proceedLevel( int _nextLevel ) {
    if( run ){
        int next = _nextLevel;
        if( 9999 != next ){ //in case error 9999
            
            
            float timeDIff = timeDiffToNextLevel( currentLevel, next );
            timer.reset();
            timer.loop(false);
            
            if( timeDIff >= 1 ) {
                
                if( currentLevel > next ){
                    unsigned char buf[3] = "XB";
                    serial.writeBytes( buf , sizeof(buf) / sizeof(buf[0]) );
                    
                } else {
                    unsigned char buf[3] = "XF";
                    serial.writeBytes( buf , sizeof(buf) / sizeof(buf[0]) );

                }
            
                timer.setTimer( timeDIff );
                timer.startTimer();
            } else {
                busy = false;
            }
                    
            
        } else {
            pompTakeRest();
            
        }
        currentLevel = next;
    } else {
        busy = false;
    }
};

float testApp::timeDiffToNextLevel( int _current, int _next ) {
    float timeRev = 12;
    return ( abs( _next - _current ) * timeRev );
}

//--------------------------------------------------------------
void testApp::reset() {
    //timer.stopTimer();
    //busy = false;
    
};

//--------------------------------------------------------------
void testApp::pompTakeRest() {
    unsigned char buf[3] = "XR";
    serial.writeBytes( buf , sizeof(buf) / sizeof(buf[0]) );
    busy = false;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

//--------------------------------------------------------------
void testApp::keyPressed(int key){

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
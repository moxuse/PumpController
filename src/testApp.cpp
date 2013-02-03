#include "testApp.h"

#define RECIVE_PORT 7400
#define BAUD_RATE 9600


bool togBusyBlink = false;
//--------------------------------------------------------------
void testApp::setup(){
    current_msg_string = 0;
    
    run = true;
    busy = false;
    serial.enumerateDevices();
    serial.setup( "/dev/tty.usbmodem1411", BAUD_RATE );
    
    reciver.setup( RECIVE_PORT );
    
    timer.setup(1000, false);
    
    gui = new ofxUICanvas();
    
    gui->addWidgetDown(new ofxUILabel("PumpController for yebizo 2013", OFX_UI_FONT_LARGE));
    gui->addToggle("Recive osc and run", true, 30, 30);
    //gui->addButton("Reset", isReset, 30, 30);
    gui->addLabel("CurrentLevel", "Current Timer : 0");
    ofAddListener(gui->newGUIEvent,this,&testApp::guiEvent);
    
    ofAddListener( timer.TIMER_REACHED, this, &testApp::timerCallback );
    
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, 5 * NSEC_PER_SEC), dispatch_get_current_queue(), ^{//do reset after 5 seconds
        reset();
    });
}

//--------------------------------------------------------------
void testApp::update(){
    
    for(int i = 0; i < NUM_MSG_STRINGS; i++){
		if(timers[i] < ofGetElapsedTimef()){
			msg_strings[i] = "";
		}
	}
    
    while ( reciver.hasWaitingMessages() ) {
        string msg_string;
        ofxOscMessage m;
        reciver.getNextMessage( &m );
        
        if( "/up" == m.getAddress() ) {
            if( false == busy ) {
                int nextLevel = m.getArgAsInt32(0);
                busy = true;
                proceedLevel( 0, nextLevel );
                if( !run ){
                    cout << "recived OSC message but not running flag.." << endl;
                }
                
                msg_string += "Message Recived : /up :" ;
                msg_string += ofToString( nextLevel );
            }
        } else if( "/down" == m.getAddress() ){
            if( false == busy ) {
                int nextLevel = m.getArgAsInt32(0);
                busy = true;
                proceedLevel( 1, nextLevel );
                if( !run ){
                    cout << "recived OSC message but not running flag.." << endl;
                }
                
                msg_string += "Message Recived : /down :" ;
                msg_string += ofToString( nextLevel );
            }
        }
        
        // add to the list of strings to display
        msg_strings[current_msg_string] = msg_string;
        timers[current_msg_string] = ofGetElapsedTimef() + 5.0f;
        current_msg_string = (current_msg_string + 1) % NUM_MSG_STRINGS;
        // clear the next line
        msg_strings[current_msg_string] = "";
        
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
    
    
	// draw mouse state
    ofPushStyle();
    ofSetColor(255, 255, 255);
    
	for(int i = 0; i < NUM_MSG_STRINGS; i++){
		ofDrawBitmapString(msg_strings[i], 430, 15 + 15 * i);
	}
    
    ofPopStyle();

}

//--------------------------------------------------------------
void testApp::timerCallback( ofEventArgs &e ) {
    int count;
    count = timer.count;
    
    pompTakeRest();
    
    cout << "timer recieved _________ " << &e << " ms : " << count << endl;
    cout << "Current Timer is _________ " << currentLevel << endl;
    
    ofxUILabel *label = ( ofxUILabel *)( gui->getWidget("CurrentLevel") );
    char result[100];
    sprintf( result, "Current Timer : %d", currentLevel );
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
void testApp::proceedLevel( int upDown , int _nextLevel ) {
    if( run ){
        int next = _nextLevel;
        if( 9999 != next  && -1 < next ){ //in case error 9999
            
            
//            float timeDIff = timeDiffToNextLevel( currentLevel, next );
//            timer.reset();
//            timer.loop(false);
            
            if( next >= 0 ) {
                
                if( 0 == upDown ){
                    unsigned char buf[3] = "XF";
                    serial.writeBytes( buf , sizeof(buf) / sizeof(buf[0]) );
                    
                } else if ( 1 == upDown ) {
                    unsigned char buf[3] = "XB";
                    serial.writeBytes( buf , sizeof(buf) / sizeof(buf[0]) );

                }
            
                timer.setTimer( next );
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
    
    ///////////////////timeRev is value for callbration
    float timeRev = 12;
    //////////////////
    
    
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
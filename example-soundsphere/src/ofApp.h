#pragma once

#include "ofMain.h"
#include "ofxSkinnedMesh.h"
#include "ofxEasyFft.h"

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
private:
	ofxSkinnedMesh mesh_;
	const float radius_min_ = 200;
	const float radius_max_ = 600;
	vector<shared_ptr<ofNode>> nodes_;
	ofEasyCam camera_;
	ofVideoGrabber video_;
	
	ofxEasyFft fft_;
};

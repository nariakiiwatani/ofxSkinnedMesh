#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofVec2f wsize(ofGetWidth(),ofGetHeight());
	
	vector<shared_ptr<ofNode>> nodes;
	for(int i = 0; i < 5; ++i) {
		auto node = make_shared<ofNode>();
		nodes.emplace_back(node);
	}
	nodes.at(0)->setPosition(0,0,0);
	nodes.at(1)->setPosition(0,wsize.y,0);
	nodes.at(2)->setPosition(wsize.x,0,0);
	nodes.at(3)->setPosition(wsize.x,wsize.y,0);
	nodes.at(4)->setPosition(wsize.x/2.f,wsize.y/2.f,0);
	mesh_.setSkeleton(nodes);
	
	constexpr int div = 32;
	for(int y = 0; y <= div; ++y) {
		for(int x = 0; x <= div; ++x) {
			ofIndexType index = y*(div+1)+x;
			ofVec3f ratio(x/(float)(div), y/(float)(div),0);
			ofPoint point = wsize*ratio;
			mesh_.addVertex(point);
			mesh_.addColor(ofFloatColor(ratio.x,ratio.y,0));
			if(x < div && y < div) {
				mesh_.addIndex(index);
				mesh_.addIndex(index+div+1);
				mesh_.addIndex(index+1);
				mesh_.addIndex(index+1);
				mesh_.addIndex(index+div+1);
				mesh_.addIndex(index+1+div+1);
			}
		}
	}
	mesh_.refreshMesh();
	mesh_.refreshWeightAutomatic(wsize.length()/2.f, 2);
	
	ctrl_ = nodes[4];
}

//--------------------------------------------------------------
void ofApp::update(){
	ctrl_->setPosition(ofGetMouseX(), ofGetMouseY(), 0);
	mesh_.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	if(ofGetMousePressed()) {
		mesh_.getDeformedMesh().drawWireframe();
	}
	else {
		mesh_.getDeformedMesh().drawFaces();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

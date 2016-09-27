#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0);
	ofSetFrameRate(60);
	
	ofMesh &&sphere = ofMesh::sphere(radius_min_, 12, OF_PRIMITIVE_TRIANGLES);
	
	mesh_.append(sphere);
	mesh_.refreshMesh();
	
	constexpr int div = 16;
	const int node_num = sphere.getNumVertices()/div;
	nodes_.resize(node_num);
	for(int i = 0; i < node_num; ++i) {
		const int vertex_index = i*div;
		nodes_[i] = make_shared<ofNode>();
		nodes_[i]->setPosition(sphere.getVertex(vertex_index));
		nodes_[i]->lookAt(ofVec3f(0,0,0));
	}
	mesh_.setSkeleton(nodes_);
	mesh_.refreshWeightAutomatic(radius_min_,2,8);
	
	ofDisableArbTex();
	video_.setup(1920,1080);
	camera_.setVFlip(true);
	
	fft_.setup();
	fft_.setUseNormalization(true);
}

//--------------------------------------------------------------
void ofApp::update(){
	fft_.update();
	const vector<float> &bin = fft_.getBins();
	for(int i = 0, num = nodes_.size(); i < num; ++i) {
		const int bin_index = ofMap(i,0,num,0,bin.size()-1);
		if(isnan(bin[bin_index])) continue;
		const ofPoint &pos = nodes_[i]->getZAxis()*ofMap(bin[bin_index], 0, 1, radius_min_, radius_max_, true);
		float ratio = 0.3f;
		nodes_[i]->setPosition(nodes_[i]->getPosition()*(1-ratio) + pos*ratio);
	}
	
	mesh_.update();
	if(video_.isInitialized()) {
		video_.update();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	camera_.begin();
	ofEnableDepthTest();
	if(video_.isInitialized()) {
		video_.getTexture().bind();
		mesh_.getDeformedMesh().draw();
		video_.getTexture().unbind();
	}
	else {
		mesh_.getDeformedMesh().draw();
	}
	const vector<float> &bin = fft_.getBins();
	const float alpha = std::accumulate(bin.begin(), bin.end(), 0.f)/bin.size();
	ofDisableDepthTest();
	ofPushStyle();
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofSetColor(ofColor::white, alpha*255);
	mesh_.getDeformedMesh().draw();
	mesh_.getDeformedMesh().drawWireframe();
	ofPopStyle();
	for_each(nodes_.begin(), nodes_.end(), [](shared_ptr<ofNode> node) {node->draw();});
	camera_.end();
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

//
//  ofxSkinMesh.cpp
//
//  Created by Iwatani Nariaki on 2016/07/13.
//
//

#include "ofxSkinnedMesh.h"

using namespace ofx::skinnedmesh;
using namespace std;

void SkinnedMesh::setSkeleton(const vector<shared_ptr<ofNode>> &skeleton)
{
	skeleton_.clear();
	original_skeleton_.clear();
	for_each(skeleton.begin(), skeleton.end(), [this](const shared_ptr<ofNode> &s) {
		skeleton_.emplace_back(s);
		original_skeleton_.emplace_back(*s);
		original_skeleton_.back().clearParent();
	});
}

void SkinnedMesh::addVertexWeight(ofIndexType vertex_index, ofIndexType node_index, float weight)
{
	auto result = weight_.insert(make_pair(vertex_index, map<ofIndexType,float>()));
	if(result.second) {
		result.first->second.insert(make_pair(node_index, weight));
	}
	else {
		auto result2 = weight_[vertex_index].insert(make_pair(node_index, weight));
		if(!result2.second) {
			weight_[vertex_index][node_index] += weight;
		}
	}
}
void SkinnedMesh::clearVertexWeight(ofIndexType vertex_index)
{
	weight_.erase(vertex_index);
}
void SkinnedMesh::clearVertexWeight()
{
	weight_.clear();
}

void SkinnedMesh::refreshWeightAutomatic(float distance_max)
{
	for(int i = 0, num = getNumVertices(); i < num; ++i) {
		const ofPoint &point = getVertex(i);
		vector<float> distance;
		float sum=0;
		for_each(original_skeleton_.begin(), original_skeleton_.end(), [&distance,&point,&sum,&distance_max](const ofNode &node) {
			float d = max(0.f,distance_max-node.getGlobalPosition().distance(point));
			distance.emplace_back(d);
			sum += d;
		});
		if(sum == 0) continue;
		for(int j = 0, jnum = distance.size(); j < jnum; ++j) {
			if(distance[j] > 0) {
				addVertexWeight(i, j, distance[j]/sum);
			}
		}
	}
}

void SkinnedMesh::update()
{
	deformed_ = *this;
	for(int i = 0, num = getNumVertices(); i < num; ++i) {
		auto it = weight_.find(i);
		if(it != weight_.end()) {
			ofPoint vertex(0,0,0);
			ofVec3f normal(0,0,0);
			for_each(it->second.begin(), it->second.end(), [this,&vertex,&normal,&i](const pair<ofIndexType,float> w) {
				auto &deform = skeleton_[w.first];
				auto &original = original_skeleton_[w.first];
				auto &&transformMat = original.getGlobalTransformMatrix().getInverse() * deform->getGlobalTransformMatrix();
				vertex += transformMat.preMult(getVertex(i)) * w.second;
				if(hasNormals()) {
					normal += transformMat.getRotate() * getNormal(i) * w.second;
				}
			});
			deformed_.setVertex(i, vertex);
			if(deformed_.hasNormals()) {
				deformed_.setNormal(i, normal);
			}
		}
	}
}

ofMesh& SkinnedMesh::getDeformedMesh()
{
	return deformed_;
}

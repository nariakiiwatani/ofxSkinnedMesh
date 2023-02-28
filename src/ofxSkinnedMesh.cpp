//
//  ofxSkinMesh.cpp
//
//  Created by Iwatani Nariaki on 2016/07/13.
//
//

#include "ofxSkinnedMesh.h"
#include <numeric>

using namespace ofx::skinnedmesh;
using namespace std;

void SkinnedMesh::addNode(std::shared_ptr<ofNode> node)
{
	skeleton_.emplace_back(node);
	original_skeleton_.emplace_back(*node);
	original_skeleton_.back().clearParent(true);
}

void SkinnedMesh::setSkeleton(const vector<shared_ptr<ofNode>> &skeleton)
{
	skeleton_.clear();
	original_skeleton_.clear();
	for_each(skeleton.begin(), skeleton.end(), [this](const shared_ptr<ofNode> &s) {
		skeleton_.emplace_back(s);
		original_skeleton_.emplace_back(*s);
		original_skeleton_.back().clearParent(true);
	});
}

void SkinnedMesh::addWeight(ofIndexType vertex_index, ofIndexType node_index, float weight)
{
	auto result = weight_.insert(make_pair(vertex_index, unordered_map<ofIndexType,float>()));
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
void SkinnedMesh::clearWeight(ofIndexType vertex_index)
{
	weight_.erase(vertex_index);
}
void SkinnedMesh::clearWeight()
{
	weight_.clear();
}

void SkinnedMesh::refreshWeightAutomatic(float distance_max, float strength, int num_joint_max)
{
	refreshWeightAutomatic(vector<float>(skeleton_.size(), distance_max), vector<float>(skeleton_.size(), strength), num_joint_max);
}
void SkinnedMesh::refreshWeightAutomatic(vector<float> distance_max, float strength, int num_joint_max)
{
	refreshWeightAutomatic(distance_max, vector<float>(skeleton_.size(), strength), num_joint_max);
}
void SkinnedMesh::refreshWeightAutomatic(float distance_max, vector<float> strength, int num_joint_max)
{
	refreshWeightAutomatic(vector<float>(skeleton_.size(), distance_max), strength, num_joint_max);
}
void SkinnedMesh::refreshWeightAutomatic(vector<float> distance_max, vector<float> strength, int num_joint_max)
{
	if(distance_max.size() < skeleton_.size()) {
		distance_max.insert(end(distance_max), skeleton_.size()-distance_max.size(), distance_max.empty()?1:distance_max.back());
	}
	if(strength.size() < skeleton_.size()) {
		strength.insert(end(strength), skeleton_.size()-strength.size(), strength.empty()?1:strength.back());
	}
	clearWeight();
	for(int i = 0, num = getNumVertices(); i < num; ++i) {
		const ofPoint &point = getVertex(i);
		typedef pair<int,float> value_type;
		vector<value_type> score;
		for(int j = 0, jnum = original_skeleton_.size(); j < jnum; ++j) {
			const ofNode &node = original_skeleton_[j];
			float distance = distance_max[j]-glm::distance(node.getGlobalPosition(), glm::vec3(point));
			if(distance > 0) {
				distance = pow(distance, strength[j]);
				score.emplace_back(make_pair(j, distance));
			}
		};
		if(score.empty()) continue;
		sort(score.begin(), score.end(), [](const value_type &a, const value_type &b) {
			return a.second > b.second;
		});
		auto end_it = 0 < num_joint_max ? score.begin()+min<int>(num_joint_max,score.size()) : score.end();
		float sum = accumulate(score.begin(), end_it, 0.f, [](float sum, const value_type &v) { return sum+v.second; });
		for_each(score.begin(), end_it, [this,&i,&sum](const value_type &v) {
			addWeight(i, v.first, v.second/sum);
		});
	}
}
void SkinnedMesh::refreshMesh()
{
	deformed_ = *this;
}
void SkinnedMesh::update()
{
	for_each(weight_.begin(), weight_.end(), [this](const pair<ofIndexType, unordered_map<ofIndexType,float>> &it) {
		int vertex_index = it.first;
		ofPoint vertex(0,0,0);
		ofVec3f normal(0,0,0);
		for_each(it.second.begin(), it.second.end(), [this,&vertex,&normal,&vertex_index](const pair<ofIndexType,float> w) {
			auto &deform = skeleton_[w.first];
			auto &original = original_skeleton_[w.first];
			auto &&transformMat = deform->getGlobalTransformMatrix() * glm::inverse(original.getGlobalTransformMatrix());
			vertex += transformMat * glm::vec4(getVertex(vertex_index),1) * w.second;
			if(hasNormals()) {
				normal += glm::mat3(transformMat) * getNormal(vertex_index) * w.second;
			}
		});
		deformed_.setVertex(vertex_index, vertex);
		if(deformed_.hasNormals()) {
			deformed_.setNormal(vertex_index, normal);
		}
	});
}

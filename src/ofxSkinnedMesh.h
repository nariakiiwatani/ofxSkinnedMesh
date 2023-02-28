//
//  ofxSkinMesh.h
//
//  Created by Iwatani Nariaki on 2016/07/13.
//
//

#pragma once

#include "ofMesh.h"
#include "ofNode.h"
#include <unordered_map>

namespace ofx { namespace skinnedmesh {
class SkinnedMesh : public ofMesh
{
public:
	void addNode(std::shared_ptr<ofNode> node);
	void setSkeleton(const std::vector<std::shared_ptr<ofNode>> &skeleton);

	void addWeight(ofIndexType vertex_index, ofIndexType node_index, float weight);
	void clearWeight(ofIndexType vertex_index);
	void clearWeight();
	
	void refreshWeightAutomatic(float distance_max, float strength=1, int num_joint_max=0);
	void refreshWeightAutomatic(std::vector<float> distance_max, float strength=1, int num_joint_max=0);
	void refreshWeightAutomatic(float distance_max, std::vector<float> strength, int num_joint_max=0);
	void refreshWeightAutomatic(std::vector<float> distance_max, std::vector<float> strength, int num_joint_max=0);
	
	void refreshMesh();
	void update();
	
	ofMesh& getDeformedMesh() { return deformed_; }
	const ofMesh& getDeformedMesh() const { return deformed_; }
	
	std::vector<std::shared_ptr<ofNode>>& getSkeleton() { return skeleton_; }
	const std::vector<std::shared_ptr<ofNode>>& getSkeleton() const { return skeleton_; }
	
	std::vector<ofNode>& getOriginalSkeleton() { return original_skeleton_; }
	const std::vector<ofNode>& getOriginalSkeleton() const { return original_skeleton_; }
	
	std::unordered_map<ofIndexType, std::unordered_map<ofIndexType, float>>& getWeight() { return weight_; }
	const std::unordered_map<ofIndexType, std::unordered_map<ofIndexType, float>>& getWeight() const { return weight_; }
	
private:
	std::vector<ofNode> original_skeleton_;
	std::vector<std::shared_ptr<ofNode>> skeleton_;
	std::unordered_map<ofIndexType, std::unordered_map<ofIndexType, float>> weight_;
	ofMesh deformed_;
};
}}

using ofxSkinnedMesh = ofx::skinnedmesh::SkinnedMesh;

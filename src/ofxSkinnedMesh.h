//
//  ofxSkinMesh.h
//
//  Created by Iwatani Nariaki on 2016/07/13.
//
//

#pragma once

#include "ofMesh.h"
#include "ofNode.h"

namespace ofx { namespace skinnedmesh {
class SkinnedMesh : public ofMesh
{
public:
	void setSkeleton(const std::vector<std::shared_ptr<ofNode>> &skeleton);
	void refreshMesh();

	void addVertexWeight(ofIndexType vertex_index, ofIndexType node_index, float weight);
	void clearVertexWeight(ofIndexType vertex_index);
	void clearVertexWeight();
	
	void refreshWeightAutomatic(float distance_max, float strength=1, int num_joint_max=0);
	
	void update();
	
	ofMesh& getDeformedMesh();
	
private:
	std::vector<ofNode> original_skeleton_;
	std::vector<std::shared_ptr<ofNode>> skeleton_;
	std::map<ofIndexType, std::map<ofIndexType, float>> weight_;
	ofMesh deformed_;
};
}}

using ofxSkinnedMesh = ofx::skinnedmesh::SkinnedMesh;
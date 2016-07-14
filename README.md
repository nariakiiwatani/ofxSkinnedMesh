# ofxSkinnedMesh

## How to use
It will be good to read this document with [example/src/ofApp.cpp](example/src/ofApp.cpp).

### Prepare

1. Create mesh same as ofMesh  
`addVertex`, `addColor`, `addIndex`, bla,bla,bla,,,  
Don't forget to call `refreshMesh` when you finish editing mesh.

1. Set skeleton data  
Skeleton data is just a vector of ofNode.  

1. Set weight
Edit relationships between mesh vertices and skeleton nodes by `addWeight`.  
You can call `refreshWeightAutomatic` instead of doing by yourself.  

### Deform mesh

1. Move skeleton nodes  
You can get skeleton nodes from `getSkeleton` or keep them by yourself.  

1. Update mesh
Just call `update`.

### Get deformed mesh

Just call `getDeformedMesh`.  
The return value is a normal ofMesh.  

## Lisence

MIT Lisence.

#ifndef QUADTREE_H
#define QUADTREE_H

#include "QuadNode.h"

#include <osg/Node>

#include <vector>
#include <queue>
#include <assert.h>


#define MAXDEPTH 8 

const size_t LevelIndex[MAXDEPTH] ={0,1,5,21,85,341,1365,5461};


class QuadTree
{

public:

	QuadTree(const size_t depth, osg::Node* OriginRoot);
	void buildQuadTree(QuadNode* root);
	void print() const;
	void addItem( osg::ref_ptr<osg::Node> node);
	size_t fitSize( const osg::BoundingBox& boundingbox);



private:
	bool m_loose;
	size_t m_depth;
	std::vector<osg::ref_ptr<QuadNode> > m_ArrayQuadNodePointer;	

	

	
};






#endif
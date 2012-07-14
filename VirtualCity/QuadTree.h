#ifndef QUADTREE_H
#define QUADTREE_H

#include "QuadNode.h"

#include <osg/Node>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <vector>
#include <queue>
#include <string>
#include <sstream>
#include <assert.h>


#define MAXDEPTH 8 

const size_t LevelIndex[MAXDEPTH] ={0,1,5,21,85,341,1365,5461};


class QuadTree
{

public:

	QuadTree(const size_t depth, osg::Node* OriginRoot,bool loose = false);
	void buildQuadTree(QuadNode* root);
	void print(std::string s) const;
	void addItem( osg::Node* node);
	size_t fitSize( const osg::BoundingBox& boundingbox);
	void fillQuadTree();
	std::string constructBuildingName( size_t index);
	QuadNode* getRoot(){ return m_ArrayQuadNodePointer[0].get();}
	void setLoose( bool loose){ m_loose = loose;}
	bool getLoose() const{ return m_loose;}
	void writeToFile(const std::string& s);
	size_t getCount(){ return getRoot()->getCount();}
	void adjustSize();



private:
	bool m_loose;
	size_t m_depth;
	std::vector<osg::ref_ptr<QuadNode> > m_ArrayQuadNodePointer;	

	

	
};






#endif
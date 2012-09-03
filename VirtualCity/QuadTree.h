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
#include <osg/PagedLOD>
#include <osg/Billboard>
#include <osg/Texture2D>
#include "windows.h"


#define MAXDEPTH 8 
#define RAND(min,max) ((min)+(double)rand()/(RAND_MAX+1)*((max)-(min)))

const size_t LevelIndex[MAXDEPTH] ={0,1,5,21,85,341,1365,5461};


class QuadTree
{

public:

	QuadTree(const size_t depth, osg::Node* OriginRoot,bool loose = true);
	QuadTree(const size_t depth, const osg::BoundingBox bb,bool loose = true);
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
	void addAllBuildings();
	QuadNode* findParentNode(osg::Node* node);
	void addAllTrees();
	void addTrees();
	void addStaticItem(osg::Node* node);



private:
	bool m_loose;
	size_t m_depth;
	std::vector<osg::ref_ptr<QuadNode> > m_ArrayQuadNodePointer;	

	

	
};






#endif
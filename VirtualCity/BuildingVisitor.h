#ifndef BUILDINGVISITOR_H
#define BUILDINGVISITOR_H

#include <osg/Node>



class BuildingVisitor : public osg::NodeVisitor
{
public:
	BuildingVisitor() : osg::NodeVisitor(TRAVERSE_ALL_CHILDREN){}


	virtual void apply( osg::Geode& geode)
	{


		traverse(geode);
	}

};








#endif
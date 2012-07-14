#ifndef BUILDINGVISITOR_H
#define BUILDINGVISITOR_H

#include <osg/NodeVisitor>
#include <osg/MatrixTransform>
#include <osgDB/WriteFile>

#include <string>
#include <sstream>
#include "InfoVisitor.h"




class BuildingVisitor : public osg::NodeVisitor
{
public:
	BuildingVisitor() : osg::NodeVisitor(TRAVERSE_ALL_CHILDREN),m_index(0){ m_root = new osg::Group;}


	virtual void apply( osg::Geode& geode)
	{
		osg::ref_ptr<osg::Geode> GeodeClone = new osg::Geode(geode,osg::CopyOp::DEEP_COPY_DRAWABLES);
		
		osg::Group* group = geode.getParent(0);
		osg::MatrixTransform* mt = dynamic_cast<osg::MatrixTransform*>(group);
		
		osg::Matrix matA = mt->getMatrix();
		
		mt = dynamic_cast<osg::MatrixTransform*>(group->getParent(0)->getParent(0));

		osg::Matrix matB = mt->getMatrix();
		
		osg::Matrix mat = matB * matA;
		mt = new osg::MatrixTransform;
		mt->setMatrix( mat );

		mt->addChild(GeodeClone.get());

		osgDB::writeNodeFile(*mt,constructFileName());
		//m_root->addChild(mt);

		traverse(geode);
	}


	void print()
	{
		InfoVisitor visitor;
		m_root->accept(visitor);
	}

	void writeSumFile()
	{
		osgDB::writeNodeFile(*m_root,"data/OptimizeBuildings.osg");
	}

	std::string constructFileName()
	{
		std::string name = "data/building_";
		std::stringstream ss;
		std::string temp;
		ss << m_index;
		ss >> temp;
		name += temp;
		name += ".osg";

		m_index++;
		return name;
	}
	

private:
	osg::ref_ptr<osg::Group> m_root;
	size_t m_index;

};








#endif
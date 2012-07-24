#ifndef _TRANSPORT_H_
#define _TRANSPORT_H_

#include <osgDB/ReadFile>
#include <osg/MatrixTransform>
#include "Vehicle.h"
#include "Path.h"
#include "TransformNodeCallback.h"
#include "Signal.h"
#include "struct.h"
#include "QuadTree.h"





//path与transport及vehicle的关系要重构
class CTransport
{
private:
	unsigned m_number;
	std::vector<osg::Node*> m_model;
	CPath* m_path;
	CSignal m_signal;
	std::vector<stindex> m_indexArray;
	int m_speed;
	QuadTree* m_tree;
	bool m_isQuadTree;



public:
	CTransport():m_number(10){}
	CTransport(CPath* path,QuadTree* tree);
	CTransport(CPath* path);
	void setNumberOfVehicle(unsigned num){m_number = num;}
	osg::ref_ptr<osg::Group> outoutputScene(); 
	void readModel();
	void update();
	std::vector<stindex> calMap(const std::vector<unsigned int>* index,const std::vector<crossingstruct>* crossing,const osg::ref_ptr<osg::Vec3Array>& junctionArray); //计算经过的路口与交通灯的对应关系
	void setInterval(double d){ m_signal.setInterval(d);}
	void setSpeed( int i){ m_speed = i;}
	void updateScene();
};










#endif
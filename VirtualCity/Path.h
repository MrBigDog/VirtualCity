#ifndef _PATH_H_
#define _PATH_H_


#define RAND(min, max) ((min) + (double)rand()/((double)RAND_MAX+1) * ((max)-(min)))


#include <osg/LineWidth>
#include <vector>
#include <osg/ref_ptr>
#include <osg/Vec3>
#include <osg/Array>
#include <osg/Geode>
#include <osg/Geometry>
#include <string>
//#include <iostream>
#include <fstream>
#include <assert.h>
#include <sstream>

#define MAX_CAPACITY 121
#define INCREMENT 100
#define TOLERANCE 8.0

//m_junctionAdjacenvy的内存分配还要重新考虑
//resize的问题还没解决
//z方向上的问题还没解决
class CPath
{
private:
	osg::ref_ptr<osg::Vec3Array> m_junctionArray;
	std::vector< std::vector<unsigned int> > m_junctionAdjacency;
	osg::Vec3d m_size;
	unsigned int m_currentCapacity;
	unsigned int m_numRoad;
	double m_widthRoad;

public:
	CPath():m_currentCapacity(MAX_CAPACITY),m_numRoad(1),m_widthRoad(0.5)
	{
 		m_junctionArray = new osg::Vec3Array;
		setSize(100.0,100.0,0.0);
		for(unsigned int i = 0;i < MAX_CAPACITY;i++)
		{
			std::vector<unsigned int> vec(MAX_CAPACITY,0);
			m_junctionAdjacency.push_back(vec);
		}
	} 
	 ~CPath(){ }


	bool isExist( const osg::Vec3d& point);
	int getIndex( const osg::Vec3d& point);
	void reSize();
	bool addPath( const osg::Vec3d& pathBegin,const osg::Vec3d& pathEnd);
	void deletePath(osg::Vec3d& pathBegin,osg::Vec3d& pathEnd);
	unsigned int getNumber(){ return m_junctionArray->size();}
	void setSize(double x,double y,double z){m_size.set(x,y,z);}
	void createJunction(unsigned int number);//生成连接点
	void createNet(unsigned int number);//生成路径网络
	void setWidthRoad(double width){ m_widthRoad = width;}
	void setNumRoad( unsigned int num){ m_numRoad = num;}
	double getWidthRoad(){ return m_widthRoad;}
	unsigned int getNumRoad(){ return m_numRoad;}
	double getLength(osg::Vec3d a,osg::Vec3d b);
	void saveToFile();
	void loadFromFile();


	osg::ref_ptr<osg::Geode> outputScene();

	osg::ref_ptr<osg::Vec3Array> getJunctionArray(){return m_junctionArray;}
	std::vector< std::vector<unsigned int> > getNet(){return m_junctionAdjacency;}


};

#endif
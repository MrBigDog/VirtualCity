#ifndef _VEHICLE_H_
#define _VEHICLE_H_

#include <osg/Array>
#include <osg/AnimationPath>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "Path.h"

class CVehicle
{

private:
	osg::ref_ptr<osg::Vec3Array> m_path;
	std::vector<unsigned int> m_index;
	double m_speed;
	std::vector<double> m_angleList;
	static unsigned int s_seed;
	unsigned int m_loc;//位于的车道


public:
	CVehicle():m_loc(0)
	{ 
		srand(s_seed++);
		//srand((unsigned)time( NULL ));
		
	}
	~CVehicle(){}

	void createRandomPath( const osg::ref_ptr<osg::Vec3Array>& junctionArray,std::vector< std::vector<unsigned int> >& s_junctionAdjacency);
	void createSequncePath( const osg::ref_ptr<osg::Vec3Array>& junctionArray,std::vector<int>& index);
	osg::ref_ptr<osg::AnimationPath> outputScene(const osg::ref_ptr<osg::Vec3Array>& junctionArray , double width ,unsigned int num);
	void getNet();
	void setSpeed( int speed){ m_speed =  rand() %  speed + 1;}
	void calculateAngleList( const osg::ref_ptr<osg::Vec3Array>& junctionArray);
	double calculateAngle(osg::Vec3 vecA,osg::Vec3 vecB);
	void generatePath(osg::ref_ptr<osg::Vec3Array> driveway , double width);//生成行驶路径
	osg::Vec3 calIntersectionPoint(const osg::Vec3& pa1,const osg::Vec3& pa2,const osg::Vec3& pb1,const osg::Vec3& pb2);
	std::vector<unsigned int>* getIndex(){ return &m_index;}
	unsigned int getLoc(){ return m_loc;}





};



#endif

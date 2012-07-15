#ifndef _SIGNAL_H
#define _SIGNAL_H

#include <osg/Array>
#include <osg/Timer>
#include <vector>


enum modes{ RED=0,GREEN=1};

typedef struct  roadstruct
{
	osg::Vec3 direction;
	modes color;
}roadstruct;

typedef struct crossingstruct
{
	osg::Vec3 point;
	int index;
	std::vector<roadstruct> roadArray;
	double interval;
}crossingstruct;


class CSignal
{
private:

	osg::Timer m_timer;
	std::vector<crossingstruct> m_crossingArray;
	double m_interval;

public:
	CSignal();
	void initSignal(const osg::ref_ptr<osg::Vec3Array>& junctionArray,const std::vector< std::vector<unsigned int> >& junctionAdjacency);
	void setInterval(double interval){ m_interval = interval;}
	void setSignal( const std::vector<osg::Vec3>& pointArray,const int index);
	void updateSignal();
	std::vector<crossingstruct>* getCrossingArray(){return &m_crossingArray;}



	
};





#endif
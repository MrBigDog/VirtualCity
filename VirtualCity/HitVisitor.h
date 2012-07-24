#ifndef _HITVISITOR_H_
#define _HITVISITOR_H_

#include <osgUtil/IntersectionVisitor>
#include <osg/Vec3>
#include "MyTransform.h"


class CHitVisitor : public osg::NodeVisitor
{
public:
	CHitVisitor();
	


	void setCenter(osg::Vec3& center);
	virtual void apply(osg::MatrixTransform& transform);
	bool getIsCrush(){ return m_isCrush;}
	void setIsCrush( bool b){ m_isCrush = b;}
	static size_t getCount() { return s_count;}
	static void resetCount() { s_count = 0;}

private:

	osg::Vec3 m_center;
	osg::BoundingSphere m_bs;
	bool m_isCrush;
	static size_t s_count;
	
	


};









#endif
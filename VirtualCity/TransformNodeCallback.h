#ifndef _TRANSFORMNODECALLBACK_H_
#define _TRANSFORMNODECALLBACK_H_


#include <osgAnimation/AnimationUpdateCallback>
#include <osg/AnimationPath>
#include <osgUtil/LineSegmentIntersector>
#include <osgUtil/IntersectionVisitor>
#include "HitVisitor.h"
#include "Signal.h"
#include "struct.h"




class  CTransformNodeCallback : public osg::AnimationPathCallback
{
private:

	osg::BoundingSphere m_bs;
	CHitVisitor m_hitVisitor;
	std::vector<crossingstruct>* m_crossingArray;
	std::vector<stindex> m_indexArray;
	int m_count;
	double m_length;//æ‡¿ÎΩªÕ®µ∆Õ£ ªµƒæ‡¿Î
	osg::Vec3 m_dirction;





public:

	CTransformNodeCallback(osg::AnimationPath* ap, std::vector<crossingstruct>* crossingArray,std::vector<stindex> indexArray,unsigned int loc);

	void operator()(osg::Node* node, osg::NodeVisitor* nv);

	bool isStopByCar(osg::Node* node);

	bool isStopBySignal();




};






#endif
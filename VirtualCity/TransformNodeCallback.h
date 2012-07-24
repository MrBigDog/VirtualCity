#ifndef _TRANSFORMNODECALLBACK_H_
#define _TRANSFORMNODECALLBACK_H_


#include <osgAnimation/AnimationUpdateCallback>
#include <osg/AnimationPath>
#include <osgUtil/LineSegmentIntersector>
#include <osgUtil/IntersectionVisitor>
#include <vector>
#include "HitVisitor.h"
#include "Signal.h"
#include "struct.h"
#include "QuadTree.h"
#include "MyTransform.h"




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
	QuadTree* m_tree;
	

public:
	static std::vector< osg::ref_ptr<MyTransform> > s_DeleteList; 
	static size_t s_count;
	



public:

	CTransformNodeCallback( osg::AnimationPath* ap, 
							std::vector<crossingstruct>* crossingArray,
							std::vector<stindex> indexArray,
							unsigned int loc,
							QuadTree* tree);

	void operator()(osg::Node* node, osg::NodeVisitor* nv);

	bool isStopByCar(osg::Node* node);

	bool isStopBySignal();

	static void resetCount(){ CHitVisitor::resetCount();}




};






#endif
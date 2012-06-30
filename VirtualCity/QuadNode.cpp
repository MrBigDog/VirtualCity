#include "QuadNode.h"

 void QuadNode::setHalf(const QuadNode& parent,const size_t index)
 {
 	if ( index == 0 || index == 3)
 	{
 		m_BoundingBox.xMin() = parent.m_BoundingBox.xMin();	 
 		m_BoundingBox.xMax() = (parent.m_BoundingBox.xMax() + parent.m_BoundingBox.xMin()) / 2.0;	 
 	}
 	if ( index == 1 || index == 2)
 	{
 		m_BoundingBox.xMin() = (parent.m_BoundingBox.xMax() + parent.m_BoundingBox.xMin()) / 2.0;
 		m_BoundingBox._max.x() = parent.m_BoundingBox._max.x();
 	}
 
 	if ( index == 0 || index == 1)
 	{
 		m_BoundingBox.yMin() = parent.m_BoundingBox.yMin();
 		m_BoundingBox.yMax() = (parent.m_BoundingBox.yMin() + parent.m_BoundingBox.yMax()) / 2.0;
 	}
 	
 	if ( index == 2 || index == 3)
 	{
 		m_BoundingBox.yMax() = parent.m_BoundingBox.yMax();
 		m_BoundingBox.yMin() = (parent.m_BoundingBox.yMin() + parent.m_BoundingBox.yMax()) / 2.0;
 	}
 }




 bool QuadNode::contain( const osg::BoundingBox& boundingbox)
 {
	
	 if ( boundingbox.xMin() >= m_BoundingBox.xMin() &&
		  boundingbox.xMax() <= m_BoundingBox.xMax() &&
		  boundingbox.yMin() >= m_BoundingBox.yMin() &&
		  boundingbox.yMax() <= m_BoundingBox.yMax()
		)
		return true;
	 else
		 return false;
 }


#include "QuadNode.h"

size_t QuadNode::s_sum;

 void QuadNode::setChildSize(const QuadNode& parent,const size_t index,bool loose)
 {
 	if ( index == 0 || index == 3)
 	{
 		m_BoundingBox.xMin() = parent.m_BoundingBox.xMin();	 
 		m_BoundingBox.xMax() = (parent.m_BoundingBox.xMax() + parent.m_BoundingBox.xMin()) * 0.5;	 
 	}
 	if ( index == 1 || index == 2)
 	{
 		m_BoundingBox.xMin() = (parent.m_BoundingBox.xMax() + parent.m_BoundingBox.xMin()) * 0.5;
 		m_BoundingBox._max.x() = parent.m_BoundingBox._max.x();
 	}
 	if ( index == 0 || index == 1)
 	{
 		m_BoundingBox.yMin() = parent.m_BoundingBox.yMin();
 		m_BoundingBox.yMax() = (parent.m_BoundingBox.yMin() + parent.m_BoundingBox.yMax()) * 0.5;
 	}
 	
 	if ( index == 2 || index == 3)
 	{
 		m_BoundingBox.yMax() = parent.m_BoundingBox.yMax();
 		m_BoundingBox.yMin() = (parent.m_BoundingBox.yMin() + parent.m_BoundingBox.yMax()) * 0.5;
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


 bool QuadNode::contain( const osg::Node* node)
 {
	 osg::BoundingSphere bs = node->getBound();
	 if ( bs._center.x() < m_BoundingBox.xMin() ||
		  bs._center.x() > m_BoundingBox.xMax() ||
		  bs._center.y() < m_BoundingBox.yMin() ||
		  bs._center.y() > m_BoundingBox.yMax() ||
		  bs._radius > getWidth()				||
		  bs._radius > getLength()
		 )
		return false;
	 return true;
 }


 void QuadNode::addItem( osg::Node* node)
 {
	 osg::BoundingSphere bs = node->getBound();

	 QuadNode* q = this;

	 for(;;)
	 {
		if ( q->getMaxLevel() == q->getLevel())
			break;

		 osg::Vec2 v((m_BoundingBox.xMax()+m_BoundingBox.xMin())*0.5,(m_BoundingBox.yMax()+m_BoundingBox.yMin())*0.5);   //四叉树节点中点
		 size_t index = 0;
		 if ( bs._center.x() > v.x() && bs._center.y() < v.y())
			 index = 1;
		 if ( bs._center.x() > v.x() && bs._center.y() > v.y())
			 index = 2;
		 if ( bs._center.x() < v.x() && bs._center.y() > v.y())
			 index = 3;


		 q = dynamic_cast<QuadNode*>(q->getChild(index));
		 if (q && q->contain(node))
			continue;
		 else
			 break;

	 }

	 q->addChild(node);
	 q->setMaxHeight(bs._center.z()+bs._radius);
 }


 void QuadNode::expand()
 {
	float m = 1.0 / m_ratio; 	
	float width = getWidth() * m;
	float length = getLength() * m;
	m_BoundingBox.xMin() -= width;
	m_BoundingBox.xMax() += width;
	m_BoundingBox.yMin() -= length;
	m_BoundingBox.yMax() += length;
 }
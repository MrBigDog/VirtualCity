#ifndef QUADNODE_H
#define QUADNODE_H

#include <osg/Group>
#include <osg/ComputeBoundsVisitor>
#include <iostream>
#include <osg/NodeCallback>
#include <osgUtil/CullVisitor>
#include <assert.h>
#include <fstream>


class FrustumCullCallback;

class  QuadNode : public osg::Group
{
public :

	QuadNode():m_Level(0),m_ratio(2.0)
	{
		setCullCallback(new QuadNodeCullCallback);
		m_BoundingBox.set(0.0,0.0,0.0,0.0,0.0,0.0);
		this->setCullingActive(false);
	}


	bool contain( const osg::BoundingBox& boundingbox);
	
	void setSize(const osg::BoundingBox& bb)
	{
		m_BoundingBox.set(bb._min,bb._max);	

	}


	void setChildSize(const QuadNode& parent,const size_t index,bool loose);

	const size_t getLevel() const { return m_Level;}

	void setLevel( const size_t level){ m_Level = level;}

	void print( std::ofstream& outfile)
	{
		  outfile << m_BoundingBox.xMin() << "  "
				  << m_BoundingBox.xMax() << "  "	
				  << m_BoundingBox.yMin() << "  "	
				  << m_BoundingBox.yMax() << "  "
				  << m_BoundingBox.zMin() << "  "
				  << m_BoundingBox.zMax() << std::endl;
	}

	float getWidth(){ return m_BoundingBox.xMax() - m_BoundingBox.xMin();}
	float getLength(){ return m_BoundingBox.yMax() - m_BoundingBox.yMin();}

	void setMaxHeight( float height) 
	{ 
		if (m_BoundingBox.zMax() < height)
		{
			m_BoundingBox.zMax() = height;
		}
		osg::Group* group = static_cast<osg::Group*>(this);
		for ( int i = 1; i < m_Level; ++i)
		{
			group = group->getParent(0);
			QuadNode* quad = dynamic_cast<QuadNode*>(group);
			if ( quad->getBoundingBox().zMax() < height)
				quad->getBoundingBox().zMax() = height;
			
		}
		
		
	}

	osg::BoundingBox getBoundingBox(){ return m_BoundingBox;}
	void setBoundingBox(const osg::BoundingBox& bb){ m_BoundingBox = bb;}

	void expand();
	
	class QuadNodeCullCallback : public osg::NodeCallback
	{
		virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
		{ 
			
			QuadNode* quadnode = dynamic_cast<QuadNode*>(node);
			assert(quadnode != NULL);
			osgUtil::CullVisitor* visitor = dynamic_cast<osgUtil::CullVisitor*>(nv);
			assert( visitor != NULL);

			s_sum++;

			node->setCullingActive(true);
			bool b = visitor->isCulled(quadnode->getBoundingBox());
			node->setCullingActive(false);
			if (b)
				return;
			else
				traverse(node,nv);

		}
	};

	size_t static getCount(){ return s_sum;}
	


protected :
	virtual ~QuadNode() {}


private:

	osg::BoundingBox m_BoundingBox;
	size_t m_Level;
	float m_ratio;
	std::vector< osg::ref_ptr<osg::Node> > m_RenderList;
	static size_t s_sum;

};




#endif
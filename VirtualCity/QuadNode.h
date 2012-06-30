#ifndef QUADNODE_H
#define QUADNODE_H

#include <osg/Group>
#include <osg/ComputeBoundsVisitor>
#include <iostream>




 
class  QuadNode : public osg::Group
{
public :
	QuadNode(size_t depth):m_Level(depth){}


	bool contain( const osg::BoundingBox& boundingbox);
	
	void setSize(const osg::BoundingBox& bb)
	{
		m_BoundingBox.set(bb._min,bb._max);
	}


	void setHalf(const QuadNode& parent,const size_t index);
	const size_t getLevel() const { return m_Level;}
	void print()
	{
		std::cout << m_BoundingBox.xMin() << "  "
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
		
	}



protected :
	virtual ~QuadNode() {}


private:

	osg::BoundingBox m_BoundingBox;
	const size_t m_Level;
	std::vector< osg::ref_ptr<osg::Node> > m_RenderList;


	

};




#endif
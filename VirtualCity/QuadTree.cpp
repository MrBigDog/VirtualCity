#include "QuadTree.h"


QuadTree::QuadTree(const size_t depth, osg::Node* OriginRoot):m_depth(depth)
{
    assert( m_depth <= MAXDEPTH);
	
	osg::ref_ptr<QuadNode> node = new QuadNode(1);

	osg::ComputeBoundsVisitor boundvisitor;
	OriginRoot->accept(boundvisitor);
	osg::BoundingBox bb;
	bb = boundvisitor.getBoundingBox();
	node->setSize(bb);

	m_ArrayQuadNodePointer.push_back(node);
	buildQuadTree(node.get());


}


void QuadTree::buildQuadTree(QuadNode* root)
{
	if ( root->getLevel() != m_depth)
	{
		 
	
		for ( size_t i = 0; i < 4; i++)
		{
			
			osg::ref_ptr<QuadNode> node = new QuadNode( root->getLevel() + 1);
			node->setHalf( *root,i );
			m_ArrayQuadNodePointer.push_back(node);
			root->addChild(node.get());			
		}
		

		size_t j = 0;
		for ( j = 0; j != m_ArrayQuadNodePointer.size(); j++)
		{
			if ( m_ArrayQuadNodePointer[j]->getNumChildren() == 0)
				break;
		}
		buildQuadTree( m_ArrayQuadNodePointer[j]);
	}

}


void QuadTree::print() const
{
	for ( int i = 0; i != m_ArrayQuadNodePointer.size(); i++)
	{
		std::cout << m_ArrayQuadNodePointer[i]->getLevel() << std::endl;
		m_ArrayQuadNodePointer[i]->print();

	}

}

void QuadTree::addItem( osg::ref_ptr<osg::Node> node)
{
	osg::ComputeBoundsVisitor boundvisitor;
	node->accept(boundvisitor);
	osg::BoundingBox boundingbox;
	boundingbox = boundvisitor.getBoundingBox();

	size_t level = fitSize( boundingbox );
	size_t index = LevelIndex[level];
	for( ; ;)
	{
		if ( index == LevelIndex[level+1])
			index = LevelIndex[--level];
		
		if ( m_ArrayQuadNodePointer[index]->contain(boundingbox))
		{
			m_ArrayQuadNodePointer[index]->addChild(node.get());
			m_ArrayQuadNodePointer[index]->setMaxHeight(boundingbox.zMax());
			break;
		}
			
	}
}

//返回匹配尺寸大小的四叉树的层
size_t QuadTree::fitSize( const osg::BoundingBox& boundingbox)
{
	float width = boundingbox.xMax() - boundingbox.xMin();
	float length = boundingbox.yMax() - boundingbox.yMax();
	float widthWorld = m_ArrayQuadNodePointer[0]->getWidth();
	float lengthWorld = m_ArrayQuadNodePointer[0]->getLength();

	size_t level = 1;
	for( ; ;)
	{
		widthWorld /= 2.0;
		lengthWorld /=2.0;
		if ( width <= widthWorld && length <= lengthWorld)
			level++;
		else
			break;
	}
	return level;

}



#include "QuadTree.h"


QuadTree::QuadTree(const size_t depth, osg::Node* OriginRoot):m_depth(depth)
{
	osg::ref_ptr<QuadNode> node = new QuadNode(1);
	osg::ComputeBoundsVisitor boundvisitor;
	OriginRoot->accept(boundvisitor);
	osg::BoundingBox bb;
	bb = boundvisitor.getBoundingBox();
	m_ArrayQuadNodePointer.push_back(node);
	//m_ArrayQuadNode.push_back(*node);
	//m_level = m_ArrayQuadNodePointer[0]->getLevel();

}


// void QuadTree::buildQuadTree(QuadNode* root)
// {
// 	if ( root->getLevel() != m_depth)
// 	{
// 
// 
// 		for ( size_t i = 0; i < 4; i++)
// 		{
// 
// 			osg::ref_ptr<QuadNode> node = new QuadNode( root->getLevel() + 1);
// 			m_ArrayQuadNodePointer.push_back(node.get());
// 			root->addChild(node.get());			
// 		}
// 
// 
// 		size_t j = 0;
// 		for ( j = 0; j != m_ArrayQuadNodePointer.size(); j++)
// 		{
// 			if ( m_ArrayQuadNodePointer[j]->getNumChildren() == 0)
// 				break;
// 		}
// 		buildQuadTree( m_ArrayQuadNodePointer[j]);
// 	}
// 
// }


void QuadTree::print() const
{
// 	for ( int i = 0; i != m_ArrayQuadNodePointer.size(); i++)
// 	{
// 		std::cout << m_ArrayQuadNodePointer[i]->getLevel() << std::endl;
// 
// 	}
// 	std::cout << m_level << std::endl;
	size_t i = m_ArrayQuadNodePointer.size();
	//size_t j = m_ArrayQuadNode.size();

}

// void QuadTree::asdf() const
// {
// 	size_t d = m_ArrayQuadNodePointer[0]->getLevel();
// }

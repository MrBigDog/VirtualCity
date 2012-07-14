#include "QuadTree.h"


QuadTree::QuadTree(const size_t depth, osg::Node* OriginRoot,bool loose):m_depth(depth),m_loose(loose)
{
    assert( m_depth <= MAXDEPTH);
	
	osg::ref_ptr<QuadNode> node = new QuadNode();
	node->setLevel(1);

	osg::ComputeBoundsVisitor boundvisitor;
	OriginRoot->accept(boundvisitor);
	osg::BoundingBox bb;
	bb = boundvisitor.getBoundingBox();
	node->setSize(bb);

	m_ArrayQuadNodePointer.push_back(node);
	buildQuadTree(node.get());
	if( loose )
		adjustSize();
	fillQuadTree();


	


}

void QuadTree::writeToFile(const std::string& s)
{
	osgDB::writeNodeFile(*(m_ArrayQuadNodePointer[0].get()),s.c_str());
}

void QuadTree::buildQuadTree(QuadNode* root)
{
	if ( root->getLevel() != m_depth)
	{
		for ( size_t i = 0; i < 4; i++)
		{	
			osg::ref_ptr<QuadNode> node = new QuadNode();
			node->setLevel( root->getLevel() + 1);
			node->setChildSize( *root,i,m_loose );
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


void QuadTree::print(std::string s) const
{
	std::ofstream outfile(s.c_str());
	for ( int i = 0; i != m_ArrayQuadNodePointer.size(); i++)
	{
		outfile << m_ArrayQuadNodePointer[i]->getLevel() << std::endl;
		m_ArrayQuadNodePointer[i]->print(outfile);
	}

}

void QuadTree::addItem( osg::Node* node)
{
	osg::ComputeBoundsVisitor boundvisitor;
	node->accept(boundvisitor);
	osg::BoundingBox boundingbox;
	boundingbox = boundvisitor.getBoundingBox();
	osg::Vec2 midpoint((boundingbox.xMax()+boundingbox.xMin())*0.5,(boundingbox.yMax()+boundingbox.yMin())*0.5);

	QuadNode* p = m_ArrayQuadNodePointer[0];

	for(;;)
	{
		osg::BoundingBox bb = p->getBoundingBox();
		osg::Vec2 v((bb.xMax()+bb.xMin())*0.5,(bb.yMax()+bb.yMin())*0.5);   //四叉树节点中点
		size_t index = 0;
		if ( midpoint.x() > v.x() && midpoint.y() < v.y())
			index = 1;
		if ( midpoint.x() > v.x() && midpoint.y() > v.y())
			index = 2;
		if ( midpoint.x() < v.x() && midpoint.y() > v.y())
			index = 3;

		if( p->getLevel() < m_depth)
		{
			QuadNode* q = dynamic_cast<QuadNode*>(p->getChild(index));
			if (q && q->contain(boundingbox))
				p = q;	
			else
				break;
		}		
		else
			break;
	}
	p->addChild(node);
	p->setMaxHeight(boundingbox.zMax());

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
		widthWorld *= 0.5;
		lengthWorld *= 0.5;
		if ( width <= widthWorld && length <= lengthWorld)
			level++;
		else
			break;
	}
	return level;

}


void QuadTree::fillQuadTree()
{
	for ( size_t i = 0; i < 900; i++)
	{
		osg::Node* node = osgDB::readNodeFile(constructBuildingName(i));
		addItem(node);
	}

}

std::string QuadTree::constructBuildingName( size_t index)
{	
	std::string name;
	name = "../data/buildings/building_";
	std::stringstream ss;
	std::string temp;
	ss << index;
	ss >> temp;
	name += temp;
	name += ".osg";

	return name;

}

void  QuadTree::adjustSize()
{
	for ( size_t i = 1; i != m_ArrayQuadNodePointer.size(); ++i)
		m_ArrayQuadNodePointer[i]->expand();
}

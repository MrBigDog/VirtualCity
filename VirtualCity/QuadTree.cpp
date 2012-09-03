#include "QuadTree.h"


QuadTree::QuadTree(const size_t depth, osg::Node* OriginRoot,bool loose):m_depth(depth),m_loose(loose)
{
    assert( m_depth <= MAXDEPTH);
	
	osg::ref_ptr<QuadNode> node = new QuadNode(depth);
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
	//fillQuadTree();

}

QuadTree::QuadTree(const size_t depth, const osg::BoundingBox bb,bool loose):m_depth(depth),m_loose(loose)
{
	assert( m_depth <= MAXDEPTH);

	osg::ref_ptr<QuadNode> node = new QuadNode(m_depth);
	node->setLevel(1);

	node->setSize(bb);

	m_ArrayQuadNodePointer.push_back(node);
	buildQuadTree(node.get());
	if( loose )
		adjustSize();
	//fillQuadTree();

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
			osg::ref_ptr<QuadNode> node = new QuadNode(m_depth);
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
	osg::BoundingSphere bs = node->getBound();
	osg::Vec2 midpoint(bs._center.x(),bs._center.y());

// 	if( (int)bs._center.x() == 680 && (int)bs._center.y() == 320)
// 	{
// 		int i = 0;
// 		i++;
// 	}

// 	osg::ComputeBoundsVisitor visitor;
// 	node->accept(visitor);
// 	osg::BoundingBox boundingbox = visitor.getBoundingBox();
// 	osg::Vec2 midpoint((boundingbox.xMax()+boundingbox.xMin())*0.5,(boundingbox.yMax()+boundingbox.yMin())*0.5);

	

	QuadNode* p = m_ArrayQuadNodePointer[0].get();

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
			if (q && q->contain(node))
				p = q;	 
			else
				break;
		}		
		else
			break;
	}
	p->addChild(node);
	//p->setMaxHeight(bs._center.z()+bs._radius);

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


void QuadTree::addAllBuildings()
{
 	std::string dir1 = "../data/buildingmodels/lod/lod-1/";
 	std::string dir2 = "../data/buildingmodels/lod/lod-2/";
	std::string dir3 = "../data/buildingmodels/lod/lod-3/";


	WIN32_FIND_DATAA FindFileData1;
	WIN32_FIND_DATAA FindFileData2;
	WIN32_FIND_DATAA FindFileData3;
	HANDLE firstFile1 = 0;
	HANDLE firstFile2 = 0;
	HANDLE firstFile3 = 0;

	QuadNode* qnode = NULL;

	firstFile1 = ::FindFirstFileA( (dir1+"*.ive").c_str(),&FindFileData1);
	firstFile2 = ::FindFirstFileA( (dir2+"*.ive").c_str(),&FindFileData2);
	firstFile3 = ::FindFirstFileA( (dir3+"*.ive").c_str(),&FindFileData3);
	if( firstFile1 && firstFile2 && firstFile3 )
	{
		std::string file1 = dir1 + FindFileData1.cFileName; 
		std::string file2 = dir2 + FindFileData2.cFileName; 
		std::string file3 = dir3 + FindFileData3.cFileName; 

		osg::ref_ptr<osg::PagedLOD> lod = new osg::PagedLOD;
		
		osg::Node* node = osgDB::readNodeFile( file3.c_str() );
		lod->addChild(node,250,FLT_MAX);

		lod->setCenter(node->getBound()._center);
	
		lod->setFileName(1,file2.c_str());
		lod->setRange(1,70,250);

		lod->setFileName(2,file1.c_str());
		lod->setRange(2,0,70);

		

		

		if( m_loose)
			addStaticItem(lod.get());
		else
			addItem(lod.get());
		
	}
	else
		return;
	while (::FindNextFileA( firstFile1,&FindFileData1) && ::FindNextFileA( firstFile2,&FindFileData2) && ::FindNextFileA( firstFile3,&FindFileData3))
	{
		std::string file1 = dir1 + FindFileData1.cFileName;
		std::string file2 = dir2 + FindFileData2.cFileName;
		std::string file3 = dir3 + FindFileData3.cFileName; 

		osg::ref_ptr<osg::PagedLOD> lod = new osg::PagedLOD;

		osg::Node* node = osgDB::readNodeFile( file3.c_str() );
		lod->addChild(node,250,FLT_MAX);
		lod->setCenter(node->getBound()._center);

		lod->setFileName(1,file2.c_str());
		lod->setRange(1,100,250);

		lod->setFileName(2,file1.c_str());
		lod->setRange(2,0,100);



		if( m_loose )
			addStaticItem(lod.get());
		else
			addItem(lod.get());
		
	}
}

QuadNode* QuadTree::findParentNode( osg::Node* node)
{
	osg::BoundingSphere bs = node->getBound();
	osg::Vec2 midpoint(bs._center.x(),bs._center.y());

	QuadNode* p = m_ArrayQuadNodePointer[0].get();

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
			if (q && q->contain(node))
				p = q;	 
			else
				break;
		}		
		else
			break;
	}

	return p;
	
}

void QuadTree::addAllTrees()
{
	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D( osgDB::readImageFile("../data/tree/tree.rgba"));
	osg::ref_ptr<osg::Node> model = osgDB::readNodeFile("../data/tree/tree.ive");


	
	int n = 90;
	for ( int k = 0; k != 16; k++)
	{	
		int m = 20;

		for ( int j = 0; j != 9; j++)
		{
			for ( int i = 1; i != 6; i++)
			{
				m += 10;
				for ( int h = 0; h != 2; h++)
				{
					
					osg::ref_ptr<osg::Billboard> billboard = new osg::Billboard;
					billboard->setMode(osg::Billboard::POINT_ROT_EYE);
					billboard->getOrCreateStateSet()->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
					billboard->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
					osg::ref_ptr<osg::Drawable> tree = osg::createTexturedQuadGeometry( osg::Vec3(0.0,0.0,0.0),
						osg::Vec3(3,0,0),
						osg::Vec3(0,0,6));
					tree->getOrCreateStateSet()->setTextureAttributeAndModes(0,texture.get());

					osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
					mt->addChild(model.get());
					
					if ( h )
					{
						billboard->addDrawable(tree.get(),osg::Vec3(m,n,1));
						mt->setMatrix(osg::Matrix::translate(m,n,0));

					}
					else
					{
						billboard->addDrawable(tree.get(),osg::Vec3(n,m,1));
						mt->setMatrix(osg::Matrix::translate(n,m,0));

					}
					
					osg::ref_ptr<osg::LOD> lod = new osg::LOD;
					lod->addChild(billboard.get(),200,FLT_MAX);
					lod->addChild(mt.get(),0,200);

					addItem(lod.get());

				}
			
			}


			m += 50;
		}

		if( k % 2 )
			n += 80;
		else
			n += 20;

	}	

}

void QuadTree::addTrees()
{
	osg::ref_ptr<osg::Node> model = osgDB::readNodeFile("../data/tree/tree.ive");

	int n = 90;
	for ( int k = 0; k != 16; k++)
	{	
		int m = 20;

		for ( int j = 0; j != 9; j++)
		{
			for ( int i = 1; i != 6; i++)
			{
				m += 10;
			
				osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
				mt->setMatrix(osg::Matrix::translate(m,n,0));
				mt->addChild(model.get());
				addItem(mt.get());
			}


			m += 50;
		}

		if( k % 2 )
			n += 80;
		else
			n += 20;

	}	

}


void QuadTree::addStaticItem(osg::Node* node)
{
	osg::ComputeBoundsVisitor visitor;
	node->accept(visitor);
	osg::BoundingBox boundingbox = visitor.getBoundingBox();
	osg::Vec2 midpoint((boundingbox.xMax()+boundingbox.xMin())*0.5,(boundingbox.yMax()+boundingbox.yMin())*0.5);

	size_t level = fitSize(boundingbox);
	if( level > m_depth)
		level = m_depth;

	std::vector<QuadNode*> vec;
	
	size_t index = LevelIndex[level-1];
	for( ; index != m_ArrayQuadNodePointer.size(); index++)
	{
		if( m_ArrayQuadNodePointer[index]->contain(boundingbox))
			vec.push_back(m_ArrayQuadNodePointer[index].get());
	}

	double min = FLT_MAX;
	size_t j;
	for ( size_t i=0; i != vec.size(); i++)
	{
		osg::BoundingBox bb = vec[i]->getBoundingBox();
		osg::Vec2 mid((bb.xMax()+bb.xMin())*0.5,(bb.yMax()+bb.yMin())*0.5);
		double d = (mid.x() - midpoint.x()) * (mid.x() - midpoint.x()) + (mid.y() - midpoint.y()) * (mid.y() - midpoint.y());
		if( d < min)
			j = i;
	}
	vec[j]->addChild(node);

}


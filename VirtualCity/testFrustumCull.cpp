
#include "QuadNode.h"

#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgDB/WriteFile>
int main()
{
	osg::BoundingBox bb;
	bb.set(1.0,1.0,1.0,2.0,2.0,2.0);	
	osg::ref_ptr<QuadNode> qnode1 = new QuadNode;
	qnode1->setBoundingBox(bb);

	osg::ref_ptr<QuadNode> qnode2 = new QuadNode;
	bb.set(0.0,0.0,0.0,5.0,5.0,5.0);
	qnode2->setBoundingBox(bb);
	osg::ref_ptr<osg::Group> root = new osg::Group;
//	osg::Node* node= osgDB::readNodeFile("../data/glider.osg");
//	qnode1->addChild(node);
	root->addChild(qnode1.get());
	root->addChild(qnode2.get());

	osgDB::writeNodeFile(*root,"../data/testFrustunCull.osg");	
//	osg::Node* root = osgDB::readNodeFile("../data/testFrustunCull.osg");
	osgViewer::Viewer viewer;
	viewer.setSceneData(root);
	return viewer.run();
}

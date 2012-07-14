#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/StateSetManipulator>
#include <osgDB/WriteFile>

#include "InsertCallbacksVisitor.h"
#include "CityManipulator.h"
#include "QuadTree.h"



int main( int argc, char **argv )
{

	osgViewer::Viewer viewer;
	osg::Node* root = osgDB::readNodeFile("../data/Buildings.osg");
 	QuadTree tree(4,root,false);
 	//tree.print("../data/LooseNode.txt");
	InsertCallbacksVisitor visitor;
	tree.getRoot()->accept(visitor);

	CityManipulator* manipulator = new CityManipulator();
	//manipulator->setHomePosition(osg::Vec3(195.0,195.0,250.0),osg::Vec3(195.0,195.0,-100.0),osg::Vec3(0.0,0.0,1.0));
	//manipulator->setHomePosition(osg::Vec3(0.0,50.0,200.0),osg::Vec3(380,380.0,-200.0),osg::Vec3(0.0,0.0,1.0));
	manipulator->setHomePosition(osg::Vec3(195.0,20.0,15.0),osg::Vec3(195.0,100.0,15.0),osg::Vec3(0.0,0.0,1.0));
	viewer.setCameraManipulator(manipulator);
	
	viewer.addEventHandler( new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()) );
	viewer.addEventHandler(new osgViewer::StatsHandler);
	viewer.addEventHandler(new osgViewer::WindowSizeHandler);


	viewer.setSceneData(tree.getRoot());
/*	return viewer.run();*/
 	size_t count = 0;
	while( !viewer.done())
	{
		count = tree.getCount();
		viewer.frame();
	}
  	return 0;

}
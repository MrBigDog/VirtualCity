#include <osgViewer/ViewerEventHandlers>
#include <osgGA/StateSetManipulator>
#include <osgDB/WriteFile>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osg/PositionAttitudeTransform>
#include <osgViewer/ViewerEventHandlers>
#include <osg/LineWidth>
#include <osg/MatrixTransform>
#include <osg/AnimationPath>
#include <osg/ComputeBoundsVisitor>
#include <osg/BoundingBox>
#include <osgGA/TrackballManipulator>
#include <osg/MatrixTransform>
#include <osgDB/WriteFile>

#include "Path.h"
#include "Vehicle.h"
#include "Transport.h"
#include "QuadTree.h"
#include "City.h"
#include "CityManipulator.h"
#include "StatisticsVisitor.h"


#include "windows.h"



int main( int argc, char **argv )
{
	osg::ref_ptr<osg::Group> root = new osg::Group;
	osgViewer::Viewer viewer;


	City city;
	//city.createArea();

 	osg::ref_ptr<osg::Node> road = osgDB::readNodeFile("../data/model/road.osg");
 	root->addChild(road.get());

	CPath pathnet;
	int numRoad = 1;
	pathnet.setSize(1000.0,1000.0,0.0);
	pathnet.createNet(100);
	pathnet.setWidthRoad(8.0);
	pathnet.setNumRoad(numRoad);

	osg::BoundingBox bb(-10.0,-10.0,0.0,910.0,910.0,0.0);
	QuadTree qtree(4,bb,true);
	qtree.addAllBuildings();
	qtree.addAllTrees();

	StatisticsVisitor visitor;
	qtree.getRoot()->accept(visitor);
	visitor.writeToFile("../data/statistics.txt");


	CTransport transport( &pathnet,&qtree);
	int numVehicle = 100;
	transport.setNumberOfVehicle(numVehicle);

	int speed = 1;
	transport.setSpeed(speed);

	int interval = 10;
	transport.setInterval(interval);

	osg::ref_ptr<osg::Group> group = transport.outoutputScene();
	root->addChild(group.get());


	viewer.addEventHandler( new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()) );
	viewer.addEventHandler(new osgViewer::StatsHandler);
	viewer.addEventHandler(new osgViewer::WindowSizeHandler);
	viewer.setCameraManipulator( new CityManipulator );

	viewer.setSceneData(root.get());



	osgDB::DatabasePager* pager = viewer.getDatabasePager();
	pager->setDoPreCompile(true);
	pager->setTargetMaximumNumberOfPageLOD(10);
	//pager->setTargetFrameRate(20);
	//pager->setDeleteRemovedSubgraphsInDatabaseThread(true);
	while(!viewer.done())
	{
		//transport.update();
		viewer.frame();
	}
	return 0;



}
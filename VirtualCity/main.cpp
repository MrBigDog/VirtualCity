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

#include "Path.h"
#include "Vehicle.h"
#include "Transport.h"


int main( int argc, char **argv )
{
	osg::ref_ptr<osg::Group> root = new osg::Group;
	osgViewer::Viewer viewer;

	CPath pathnet;
	int numRoad = 1;
	//pathnet.loadFromFile();
	pathnet.setSize(1000.0,1000.0,0.0);
	pathnet.createNet(100);
	pathnet.setWidthRoad(10.0);
	pathnet.setNumRoad(numRoad);
 	osg::ref_ptr<osg::Geode> geode = pathnet.outputScene();
 	root->addChild(geode);
	CTransport transport( &pathnet);
	int numVehicle = 150;
	transport.setNumberOfVehicle(numVehicle);

	int speed = 4;
	transport.setSpeed(speed);

	int interval = 10;
	transport.setInterval(interval);

	osg::ref_ptr<osg::Group> group = transport.outoutputScene();
	root->addChild(group.get());

	viewer.addEventHandler( new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()) );
	viewer.addEventHandler(new osgViewer::StatsHandler);
	viewer.addEventHandler(new osgViewer::WindowSizeHandler);
	viewer.setCameraManipulator( new osgGA::TrackballManipulator );

	viewer.setSceneData(root.get());

	while(!viewer.done())
	{
		transport.update();
		viewer.frame();
	}
	return 0;



}
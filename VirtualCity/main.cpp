#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/StateSetManipulator>
#include <osgDB/WriteFile>

#include "City.h"
#include "CityManipulator.h"

int main( int argc, char **argv )
{

	osgViewer::Viewer viewer;
	osg::ref_ptr<osg::Group> root = new osg::Group;

	City city;
	city.createArea();
	city.createCityBuildings();
	root->addChild(city.getArea());
	root->addChild(city.getBuildings());

// 	CityManipulator* manipulator = new CityManipulator();
// 	viewer.setCameraManipulator(manipulator);
	
	viewer.addEventHandler( new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()) );
	viewer.addEventHandler(new osgViewer::StatsHandler);
	viewer.addEventHandler(new osgViewer::WindowSizeHandler);
	viewer.setSceneData(root.get());
	
	osgDB::writeNodeFile(*root,"cityBuildings.osg");
	return 0;//viewer.run();


}
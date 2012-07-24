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
#include "QuadTree.h"
#include "InfoVisitor.h"



osg::Drawable*  createPlane(osg::Vec3 a,osg::Vec3 b,osg::Vec3 c,osg::Vec3 d)
{
	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
	vertices->push_back(a);
	vertices->push_back(b);
	vertices->push_back(c);
	vertices->push_back(d);

	osg::Vec3 AB = b - a;
	osg::Vec3 AD = d - a;
	osg::Vec3 normal = AB^AD;
	normal.normalize();

	osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
	normals->push_back(normal);

	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(0.0,0.5,0.0,0.5));

	osg::ref_ptr<osg::Geometry> area = new osg::Geometry;
	area->setVertexArray(vertices.get());
	area->setNormalArray(normals.get());
	area->setNormalBinding(osg::Geometry::BIND_OVERALL);
	area->setColorArray(colors.get());
	area->setColorBinding(osg::Geometry::BIND_OVERALL);
	area->addPrimitiveSet(new osg::DrawArrays(osg::DrawArrays::QUADS,0,4));

	return area.release();
}



int main( int argc, char **argv )
{
	osg::ref_ptr<osg::Group> root = new osg::Group;
	osgViewer::Viewer viewer;

	osg::Drawable* drawable = createPlane( osg::Vec3(-10.0,-10.0,-1.0),osg::Vec3(910.0,-10.0,-1.0),osg::Vec3(910.0,910.0,-1.0),osg::Vec3(-10.0,910.0,-1.0));
	osg::Geode* geo = new osg::Geode;
	geo->addDrawable(drawable);
	root->addChild(geo);

	CPath pathnet;
	int numRoad = 1;
//	pathnet.loadFromFile();
	pathnet.setSize(1000.0,1000.0,0.0);
	pathnet.createNet(100);
//	pathnet.saveToFile();
	pathnet.setWidthRoad(8.0);
	pathnet.setNumRoad(numRoad);
 	osg::ref_ptr<osg::Geode> geode = pathnet.outputScene();
 	//root->addChild(geode);


	osg::BoundingBox bb(-10.0,-10.0,0.0,910.0,910.0,0.0);
	QuadTree qtree(4,bb,true);

	CTransport transport( &pathnet,&qtree);
	int numVehicle = 200;
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
	viewer.setCameraManipulator( new osgGA::TrackballManipulator );

	viewer.setSceneData(root.get());

	
	while(!viewer.done())
	{
		transport.update();
		//osg::Group* temp = root.get();
		viewer.frame();
	}
	return 0;



}
#include "City.h"
#include "QuadTree.h"
#include <iostream>

int main( int argc, char **argv )
{
	osg::ref_ptr<osg::Group> root = new osg::Group;

	City city;
	city.createArea();
	city.createCityBuildings();
	root->addChild(city.getArea());
	root->addChild(city.getBuildings());
	
	
	QuadTree tree(4,root.get());
	tree.print();
	return 0;
}
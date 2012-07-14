//#include <osgDB/ReadFile>
#include "QuadTree.h"
// #include <iostream>
// #include <osgViewer/Viewer>
 #include "StatisticsVisitor.h"


int main( int argc, char **argv )
{

 	osg::Node* root = osgDB::readNodeFile("../data/QuadTree(D6).osg");
  	StatisticsVisitor visitor;
  	root->accept(visitor);
  	visitor.writeToFile("../data/QuadTree(D6)_Statistics.txt");


// 	osg::Node* root = osgDB::readNodeFile("../data/Buildings.osg");
//  	QuadTree tree(6,root,false);
// 	tree.writeToFile("../data/QuadTree(D6).osg"
 
	return 0;
}
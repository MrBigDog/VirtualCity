/* OpenSceneGraph example, osgsimplifier.
*
*  Permission is hereby granted, free of charge, to any person obtaining a copy
*  of this software and associated documentation files (the "Software"), to deal
*  in the Software without restriction, including without limitation the rights
*  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*  copies of the Software, and to permit persons to whom the Software is
*  furnished to do so, subject to the following conditions:
*
*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
*  THE SOFTWARE.
*/

#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>
#include <osgUtil/Simplifier>
#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>
#include <iostream>
#include <osg/ComputeBoundsVisitor>
#include <osg/MatrixTransform>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/StateSetManipulator>
#include <iostream>
#include <osgDB/WriteFile>
#include "TriangleInfo.h"

using namespace std;


int main( int argc, char **argv )
{

	// construct the viewer.
	osgViewer::Viewer viewer;

	osg::ref_ptr<osg::Group> root = new osg::Group;

	osg::Node* Model = osgDB::readNodeFile("../data/buildingmodels/3ds/Bldng001.osg");
	root->addChild( Model );
	FindGeometryVisitor fgv;
	Model->accept( fgv );
	size_t sum = FindGeometryVisitor::sum;
	cout << sum << endl;
	fgv.reset();
	sum = 0;


	osgUtil::Simplifier simplifier_1(0.01, 0.5);
	simplifier_1.setDoTriStrip(false);
	osg::ref_ptr<osg::Node> lodModel_1 = (osg::Node*)Model->clone(osg::CopyOp::DEEP_COPY_ALL);
	lodModel_1->accept(simplifier_1);
	osg::ref_ptr<osg::MatrixTransform> mt_1 = new osg::MatrixTransform;
	mt_1->setMatrix( osg::Matrix::translate(5.0,0.0,0.0));
	mt_1->addChild(lodModel_1.get());
	root->addChild(mt_1.get());
	lodModel_1->accept( fgv );
	sum = FindGeometryVisitor::sum;
	cout << sum << endl;
	fgv.reset();
	sum = 0;
	osgDB::writeNodeFile(*lodModel_1,"../data/buildingmodels/lod/Bldng001/Bldng001_1.ive");

	osgUtil::Simplifier simplifier_2(0.01, 0.5);
	simplifier_2.setDoTriStrip(false);
	osg::ref_ptr<osg::Node> lodModel_2 = (osg::Node*)lodModel_1->clone(osg::CopyOp::DEEP_COPY_ALL);
	lodModel_2->accept(simplifier_2);
	osg::ref_ptr<osg::MatrixTransform> mt_2 = new osg::MatrixTransform;
	mt_2->setMatrix( osg::Matrix::translate(10.0,0.0,0.0));
	mt_2->addChild(lodModel_2.get());
	root->addChild(mt_2.get());
	lodModel_2->accept( fgv );
	sum = FindGeometryVisitor::sum;
	cout << sum << endl;
	fgv.reset();
	sum = 0;
	osgDB::writeNodeFile(*lodModel_2,"../data/buildingmodels/lod/Bldng001/Bldng001_2.ive");

	osgUtil::Simplifier simplifier_3(0.01, 0.5);
	simplifier_3.setDoTriStrip(false);
	osg::ref_ptr<osg::Node> lodModel_3 = (osg::Node*)lodModel_2->clone(osg::CopyOp::DEEP_COPY_ALL);
	lodModel_3->accept(simplifier_3);
	osg::ref_ptr<osg::MatrixTransform> mt_3 = new osg::MatrixTransform;
	mt_3->setMatrix( osg::Matrix::translate(15.0,0.0,0.0));
	mt_3->addChild(lodModel_3.get());
	root->addChild(mt_3.get());
	lodModel_3->accept( fgv );
	sum = FindGeometryVisitor::sum;
	cout << sum << endl;
	fgv.reset();
	sum = 0;
	osgDB::writeNodeFile(*lodModel_3,"../data/buildingmodels/lod/Bldng001/Bldng001_3.ive");


	viewer.setSceneData(root.get());

	viewer.addEventHandler( new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()) );
	viewer.addEventHandler(new osgViewer::StatsHandler);
	viewer.addEventHandler(new osgViewer::WindowSizeHandler);
	viewer.setCameraManipulator( new osgGA::TrackballManipulator );
	

	return viewer.run();
}


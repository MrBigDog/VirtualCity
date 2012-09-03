#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osg/MatrixTransform>
#include <osgViewer/Viewer>
#include <osg/BoundingBox>
#include <osg/ComputeBoundsVisitor>
#include <fstream>
#include <sstream>
#include <osgDB//ReadFile>


using namespace std;


void setSize()
{
	fstream outfile("../data/Statistics/buildingsSize.txt");

	for ( size_t i = 1 ; i < 183 ; i++)
	{
		stringstream ss;
		string base = "../data/buildingmodels/3ds/";
		string str;
		string fullname;

		if( i < 10)
			str = "Bldng00";

		else
			if ( i < 100)
				str = "Bldng0";

			else
				str = "Bldng";

		ss << i;
		str += ss.str();
		str += ".3ds";
		fullname = base + str;

		osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(fullname);
// 		osg::ComputeBoundsVisitor visitor;
// 		node->accept(visitor);
// 		osg::BoundingBox bb = visitor.getBoundingBox();
//		outfile << bb.xMax()-bb.xMin() << "  " << bb.yMax()-bb.yMin() << "  " << bb.zMax()-bb.zMin() << endl;
		osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
		mt->setMatrix(osg::Matrix::scale(6.0,6.0,6.0));
		mt->addChild(node.get());

		base = "../data/buildingmodels/normalize3ds/";
		fullname = base + str;
		osgDB::writeNodeFile(*mt,fullname);

	}
}



int main()
{
	//setSize();
	ifstream infile;
	infile.open("../data/Statistics/buildingsSize.txt",ifstream::in);
	
	vector<osg::Vec3> vec;
	osg::Vec3 v3;
	string line;
	
	while ( getline(infile,line))
	{
		stringstream ss(line);
		ss >> v3.x() >> v3.y() >> v3.z();
		vec.push_back(v3);
	}

	for ( size_t i = 0; i != vec.size(); i++)
		vec[i] *= 6.0;
		
	vector<int> s1;//1*1
	vector<int> s2;//1*2
	vector<int> s3;//2*2
	vector<int> s4;


	double rang1 = 80.0 / 3.0;
	double rang2 = 80.0 / 2.0;
	
	for ( size_t i = 0; i != vec.size(); i++)
	{
		if ( vec[i].x() < rang1 && vec[i].y() < rang1 )
			s1.push_back(i+1);
		else
			if ( vec[i].x() < rang1 || vec[i].y() < rang1 )
				s2.push_back(i+1);
			else
				if( vec[i].x() < rang2 || vec[i].y() < rang2 )
					s3.push_back(i+1);
				else
					s4.push_back(i+1);
	}
	
	return 0;
}
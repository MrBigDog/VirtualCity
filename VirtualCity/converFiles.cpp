#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <string>
#include <sstream>
#include <vector>

using namespace  std;



int main()
{
	vector<string> vs;
	vector<string> osg;
	vector<string> ive;
	
	for ( size_t i = 1 ; i < 183 ; i++)
	{
		
		if( i < 10)
		{
			string s1 = "../data/buildingmodels/3ds/Bldng00";
			string s2 = "../data/buildingmodels/osg/Bldng00";
			string s3 = "../data/buildingmodels/ive/Bldng00";
			stringstream ss;
			ss << i;
			s1 += ss.str();
			s1 += ".3ds";
			s2 += ss.str();
			s2 += ".osg";
			s3 += ss.str();
			s3 += ".ive";
			vs.push_back(s1);
			osg.push_back(s2);
			ive.push_back(s3);
		}
		else
			if ( i < 100)
			{
				string s1 = "../data/buildingmodels/3ds/Bldng0";
				string s2 = "../data/buildingmodels/osg/Bldng0";
				string s3 = "../data/buildingmodels/ive/Bldng0";
				stringstream ss;
				ss << i;
				s1 += ss.str();
				s1 += ".3ds";
				s2 += ss.str();
				s2 += ".osg";
				s3 += ss.str();
				s3 += ".ive";
				vs.push_back(s1);
				osg.push_back(s2);
				ive.push_back(s3);
			}
			else
			{
				string s1 = "../data/buildingmodels/3ds/Bldng";
				string s2 = "../data/buildingmodels/osg/Bldng";
				string s3 = "../data/buildingmodels/ive/Bldng";
				stringstream ss;
				ss << i;
				s1 += ss.str();
				s1 += ".3ds";
				s2 += ss.str();
				s2 += ".osg";
				s3 += ss.str();
				s3 += ".ive";
				vs.push_back(s1);
				osg.push_back(s2);
				ive.push_back(s3);
			}
			
	}

	for ( size_t i = 0; i < 5; i++)
	{
		osg::Node* node = osgDB::readNodeFile( vs[i]);
		osgDB::writeNodeFile( *node,osg[i]);
		osgDB::writeNodeFile( *node,ive[i]);
	}
}
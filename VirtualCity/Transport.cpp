#include "Transport.h"
#include "MyTransform.h"

CTransport::CTransport(CPath* path):m_number(1),m_path(path)
{ 

}



void CTransport::readModel()
{
	osg::Node* model = osgDB::readNodeFile("../data/vehicle/tank.osg");
	m_model.push_back(model);
// 	model = osgDB::readNodeFile("data/release_2.ive");
// 	m_model.push_back(model);
// 	model = osgDB::readNodeFile("../data/vehicle/vehicle_3.ive");
// 	m_model.push_back(model);
// 	model = osgDB::readNodeFile("../data/vehicle/vehicle_4.ive");
// 	m_model.push_back(model);
// 	model = osgDB::readNodeFile("../data/vehicle/vehicle_5.ive");
// 	m_model.push_back(model);
// 	model = osgDB::readNodeFile("../data/vehicle/vehicle_6.ive");
// 	m_model.push_back(model);

}



osg::ref_ptr<osg::Group> CTransport::outoutputScene()
{
	readModel();

	osg::ref_ptr<osg::Vec3Array> junctionArray = m_path->getJunctionArray();
	std::vector< std::vector<unsigned int> > junctionAdjacency = m_path->getNet();
	
	m_signal.initSignal(junctionArray,junctionAdjacency);
	osg::ref_ptr<osg::Group> group = new osg::Group;

// 	std::vector<int> vec1;
// 	vec1.push_back(5);
// 	vec1.push_back(15);
// 	vec1.push_back(25);
// 	vec1.push_back(35);
// 	vec1.push_back(45);
// 	vec1.push_back(55);
// 	vec1.push_back(65);
// 	vec1.push_back(75);

// 	std::vector<int> vec2;
// 	vec2.push_back(7);
// 	vec2.push_back(6);
// 	vec2.push_back(5);
// 	vec2.push_back(4);
// 	vec2.push_back(3);
// 	vec2.push_back(2);
// 	vec2.push_back(1);
// 	vec2.push_back(0);
// 
// 	std::vector<int> vec3;
// 	vec3.push_back(2);
// 	vec3.push_back(3);
// 	vec3.push_back(4);
// 	vec3.push_back(5);
// 	vec3.push_back(6);
// 	vec3.push_back(7);
// 	vec3.push_back(8);
// 	vec3.push_back(9);




	for (int i = 0;i!=m_number;i++)
	{
		CVehicle car;
		car.setSpeed(m_speed);
		car.createRandomPath(junctionArray,junctionAdjacency);

		std::vector<unsigned int>* vec= car.getIndex();
		if ( vec->size() < 20)
		{
			--i;
			continue;
		}
		std::vector<stindex> indexArray = calMap(vec,m_signal.getCrossingArray(),junctionArray);


		osg::ref_ptr<osg::AnimationPath> animationPath = car.outputScene(junctionArray,m_path->getWidthRoad(),m_path->getNumRoad());
		animationPath->setLoopMode( osg::AnimationPath::LOOP );

		osg::ref_ptr<osg::MyTransform> transform = new osg::MyTransform;
		transform->addChild(m_model[i%m_model.size()]);
		
		transform->setUpdateCallback( new CTransformNodeCallback(animationPath,m_signal.getCrossingArray(),indexArray,car.getLoc()));

	
		group->addChild( transform );
		
		
	}

	return group.release();
}

void CTransport::update()
{
	m_signal.updateSignal();
}


std::vector<stindex>  CTransport::calMap(const std::vector<unsigned int>* index,const std::vector<crossingstruct>* crossing,const osg::ref_ptr<osg::Vec3Array>& junctionArray)
{
	std::vector<stindex> indexArray;
	for ( int i = 1; i != index->size(); i++)
	{
		stindex indexmap;
		for ( int j = 0; j !=crossing->size(); j++)
		{
			if ( (*index)[i] == (*crossing)[j].index)
			{
				indexmap.crossing = j;

				osg::Vec3 vec = (*junctionArray)[(*index)[i]] - (*junctionArray)[(*index)[i-1]];
				vec.normalize();

				std::vector<roadstruct> roadArray = (*crossing)[j].roadArray;
				double max = -1.0,temp;
				int r;
				for ( int k = 0; k != roadArray.size(); k++)
				{
					temp = vec * roadArray[k].direction;
					if ( temp > max)
					{
						max = temp;
						r = k;
					}

				}

				indexmap.direction = r;

				indexArray.push_back(indexmap);

			}
		}
	}

	return indexArray;

}
#include "Vehicle.h"



unsigned int CVehicle::s_seed = (unsigned)time(0);

void CVehicle::getNet()
{

}


void CVehicle::createRandomPath(const osg::ref_ptr<osg::Vec3Array>& junctionArray,std::vector< std::vector<unsigned int> >& junctionAdjacency)
{

	std::vector<unsigned int> temp1,temp2;

	unsigned int length = 50;  //定义路径长度
	unsigned int begin=0,end=0,temp=0;

	begin = rand() % junctionArray->size();
	m_index.push_back(begin);

	temp1 = junctionAdjacency[begin]; 
	for (int i = 0;i != temp1.size();i++)
	{
		if (temp1[i])
		{
			temp2.push_back(i);
		}
	}


	temp = rand() % temp2.size();
	end = temp2[temp];


	m_index.push_back(end);

	temp1.clear();
	temp2.clear();
	

	osg::Vec3 vecA,vecB;
	unsigned int mid;
	double angle;
	
	while (m_index.size() < length )
	{
		temp1 = junctionAdjacency[end];
		mid = end;
		vecA = (*junctionArray)[mid] - (*junctionArray)[begin];
		vecA.normalize();

		for (int i = 0;i != temp1.size();i++)
		{
			if (temp1[i])
			{
				temp2.push_back(i);
			}
		}

		if( temp2.size() == 1)
			break;
		else

			if (temp2.size() == 2)
			{
				end = temp2[0];
				if( begin == end)
					end = temp2[1];
				m_index.push_back(end);


			}
			else
			{
				do 	
				{
					temp = rand() % temp2.size();
					end = temp2[temp];
					vecB = (*junctionArray)[end] - (*junctionArray)[mid];
					vecB.normalize();
					angle = calculateAngle( vecA,vecB );
					srand(s_seed++);

				}while(  angle > (osg::PI / 3.0) || begin == end );

				m_index.push_back(end);

			}
			
	
		
		temp1.clear();
		temp2.clear();

		begin = m_index[m_index.size()-2];
		end = m_index[m_index.size()-1];
	}

// 	m_index.push_back(5);
// 	m_index.push_back(15);
// 	m_index.push_back(25);
// 	m_index.push_back(35);
// 	m_index.push_back(45);
// 	m_index.push_back(55);
// 	m_index.push_back(65);
// 	m_index.push_back(75);
// 	m_index.push_back(85);

	calculateAngleList(junctionArray);
	
}

void CVehicle::createSequncePath(const osg::ref_ptr<osg::Vec3Array>& junctionArray,std::vector<int>& index )
{

	for(int i = 0; i != index.size(); i++)
		m_index.push_back(index[i]);

	calculateAngleList(junctionArray);
}

double CVehicle::calculateAngle(osg::Vec3 vecA,osg::Vec3 vecB)
{
	double angle,angleA,angleB;

	angle = acos(vecA * vecB / (vecA.length() * vecB.length()));
	if( angle < 0.0001)
	{
		angle = 0.0;
		return angle;
	}

	//求得vecA vecB分别与x轴正向的夹角
	angleA =  acos( vecA.x() / vecA.length() );   
	if( vecA.y() >= 0 )
		angleA = angleA;
	if( vecA.y() < 0 )
		angleA = 2.0 * osg::PI - angleA;


	angleB =  acos( vecB.x() / vecB.length() );   
	if( vecB.y() >= 0 )
		angleB = angleB;
	if( vecB.y() < 0 )
		angleB = 2.0 * osg::PI - angleB;


	//得到两向量间的夹角，通过angleA angleB的关系来判断旋转的方向

	if( angleA > angleB && (angleA - angleB) <= osg::PI )
		angle = -1.0 * acos(vecA * vecB / (vecA.length() * vecB.length()));
	if( angleA > angleB && (angleA - angleB) > osg::PI)
		angle = acos(vecA * vecB / (vecA.length() * vecB.length())) ;


	if( angleA < angleB && (angleB - angleA) <= osg::PI )
		angle = acos(vecA * vecB / (vecA.length() * vecB.length()));
	if( angleA < angleB && (angleB - angleA) > osg::PI)
		angle = -1.0 *  acos(vecA * vecB / (vecA.length() * vecB.length()))  ;

	return angle;
}





void  CVehicle::calculateAngleList( const osg::ref_ptr<osg::Vec3Array>& junctionArray)
{

	osg::Vec3 vecA,vecB;

	for (int i = 0;i != m_index.size()-1;i++)
	{
		if(i == 0)
			vecA.set(0.0,1.0,0.0);
		else
			vecA = (*junctionArray)[m_index[i]] - (*junctionArray)[m_index[i-1]];
		vecA.normalize();

		vecB = (*junctionArray)[m_index[i+1]] - (*junctionArray)[m_index[i]];
		vecB.normalize();

		m_angleList.push_back(calculateAngle(vecA,vecB));	

	}


}


osg::ref_ptr<osg::AnimationPath> CVehicle::outputScene(const osg::ref_ptr<osg::Vec3Array>& junctionArray , double width, unsigned int num)
{
	osg::ref_ptr<osg::Vec3Array> driveway = new osg::Vec3Array;
	for (int i = 0; i != m_index.size(); i++ )
	{
		driveway->push_back((*junctionArray)[m_index[i]]);
	}

	m_loc = rand() % num ;
	double w = width / 2.0 + width * m_loc;
	generatePath(driveway,w);


	
	typedef osg::AnimationPath::ControlPoint ControlPoint;
	osg::Vec3 yoz(1.0, 0.0, 0.0), xoz(0.0, 1.0, 0.0), xoy(0.0, 0.0, 1.0);
	osg::ref_ptr<osg::AnimationPath> path = new osg::AnimationPath;




	double m = 0;

	for (int i = 0; i != m_index.size(); i++ )
	{
		if( i == 0)
		{
			path->insert( 0,
				ControlPoint((*driveway)[i],osg::Quat( 0.0,yoz,0.0,xoz, 0.0,xoy)) );
			
		}

		else
		{
			m += m_angleList[i-1];
				
			path->insert( (i-1) * m_speed + 0.1,
				ControlPoint((*driveway)[i-1],osg::Quat( 0.0,yoz,0.0,xoz,m,xoy)) );
			path->insert( i * m_speed , 
				ControlPoint((*driveway)[i],osg::Quat( 0.0,yoz,0.0,xoz,m,xoy)) );
		
		}

	}

	return path.release();

}

void  CVehicle::generatePath(osg::ref_ptr<osg::Vec3Array> driveway , double width)
{
	//求出每条线段方向向量的垂直向量
	std::vector<osg::Vec3> vecArray ;
	osg::Vec3 vec,axis;
	axis.set(0.0,0.0,-1.0);
	for ( int i = 0; i != driveway->size()-1; i++)
	{
		//vec.set(((*driveway)[i+1].x() - (*driveway)[i].x()),((*driveway)[i+1].y() - (*driveway)[i].y()),((*driveway)[i+1].z() - (*driveway)[i].z()));
		vec = (*driveway)[i+1] - (*driveway)[i];
		vec.normalize();
		vec = osg::Quat( osg::PI_2,axis) * vec;//旋转90度
		vecArray.push_back(vec);
	}
	

	osg::Vec3 v;
	v = ((*driveway)[0] + (*driveway)[1]) / 2.0;//(double)(rand() % 5 + 2);
	(*driveway)[0] = v + vecArray[0] * width;//出生点的位置

	v = ((*driveway)[ driveway->size()-1] + (*driveway)[ driveway->size()-2]) / 2.0;//(double)(rand() % 5 + 2);
	(*driveway)[ driveway->size()-1] = v + vecArray[vecArray.size()-1] * width ;//消亡点位置


	
	std::vector<osg::Vec3> temp;
	vec = (*driveway)[0] + vecArray[0] * width;
	temp.push_back(vec);
	(*driveway)[0] = vec ;
	//交叉点对应两个垂直点
	for (int i = 0; i != vecArray.size()-1; i++)
	{
		vec = (*driveway)[i+1] + vecArray[i] * width;
		temp.push_back(vec);
		vec = (*driveway)[i+1] + vecArray[i+1] * width;
		temp.push_back(vec);
	}
	vec = (*driveway)[driveway->size()-1] + vecArray[vecArray.size()-1] * width;
	temp.push_back(vec);
	(*driveway)[driveway->size()-1] = vec;

	
	//求出两条直线的交点
	int j = 1;
	for ( int i = 2;i < temp.size();i+=2)
	{	
		(*driveway)[j] = calIntersectionPoint(temp[i-2],temp[i-1],temp[i],temp[i+1]);
		j++;

	}



}


osg::Vec3  CVehicle::calIntersectionPoint(const osg::Vec3& pa1,const osg::Vec3& pa2,const osg::Vec3& pb1,const osg::Vec3& pb2)
{
	osg::Vec3 intersectionPoint,pa12,pb12;
	double t;
	pa12 = pa2 - pa1;
	pb12 = pb2 - pb1;

	double temp =  (pa12.y() * pb12.x()) - (pb12.y() * pa12.x());
	if(temp < 0.001)
		return pa2;
	else
		t =( (pb1.y() - pa1.y()) * pb12.x() + (pa1.x() - pb1.x()) * pb12.y() ) / temp;

	double x,y,z;
	x = pa1.x() + pa12.x() * t;
	y = pa1.y() + pa12.y() * t;
	z = pa1.z() + pa12.z() * t;
	intersectionPoint.set(x,y,z);

	return intersectionPoint;



		



}
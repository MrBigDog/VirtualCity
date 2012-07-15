#include "Path.h"




bool CPath::addPath(const osg::Vec3d& pathBegin,const osg::Vec3d& pathEnd)
{
	int begin = getIndex( pathBegin );
	int end = getIndex( pathEnd );
// 	if ( begin != -1 && end != -1 )	
// 	{
// 		m_junctionAdjacency[begin][end] = m_junctionAdjacency[end][begin] = 0;
// 		return false;
// 	}
// 		

	if( begin == -1)
	{
		m_junctionArray->push_back(pathBegin);
		begin = m_junctionArray->size() - 1;
	}
	
	if( end == -1)
	{
		m_junctionArray->push_back(pathEnd);
		end = m_junctionArray->size() - 1;	
	}
	m_junctionAdjacency[begin][end] = m_junctionAdjacency[end][begin] = 1;
	return true;
	
}

void CPath::deletePath(osg::Vec3d& pathBegin,osg::Vec3d& pathEnd)
{
	int begin = getIndex(pathBegin);
	int end = getIndex(pathEnd);
	if ( begin != -1 && end != -1 )	
		m_junctionAdjacency[begin][end] = m_junctionAdjacency[end][begin] = 0;

}

bool CPath::isExist( const osg::Vec3d& point)
{
	if(m_junctionArray->empty())
		return false;
	else 
	{
		double length;
		for( int i = 0; i!= m_junctionArray->size(); i++ )
		{
			length = getLength( point , (*m_junctionArray)[i] );
			if( length < TOLERANCE)
				return true;
		}
		return false;
	}

}

int CPath::getIndex( const osg::Vec3d& point)
{
	if(m_junctionArray->empty())
		return -1;
	else 
	{
		double length;
		for( int i = 0; i!= m_junctionArray->size(); i++ )
		{
			length = getLength( point , (*m_junctionArray)[i] );
			if( length < TOLERANCE)
				return i;
		}
		return -1;
	}


}

double CPath::getLength(osg::Vec3d a,osg::Vec3d b)
{
	osg::Vec3d temp;
	temp = a - b;
	return temp.length();
}

void CPath::reSize()
{
// 	m_currentCapacity  += INCREMENT;
// 	for(unsigned int i = 0;i < m_currentCapacity;i++)
// 	{
// 		std::vector<unsigned int> vec(m_currentCapacity,0);
// 		m_junctionAdjacency.push_back(vec);
// 	}

}

void CPath::createJunction(unsigned int number)
{
	osg::Vec3d temp;
	double size = sqrt(double(number));
	double tileX = m_size.x() / size;
	double tileY = m_size.y() / size;

	for (unsigned int i=0;i<size;i++)
	{
		for(unsigned int j=0;j<size;j++)
		{
			temp.set( j*tileX ,i*tileY ,0 );
			//temp.set(j*tileX + rand() % (int)( tileX / 5.0),i*tileY + rand() % (int)( tileY / 5.0),0);
			//temp.set( RAND(j*tileX, (j+1)*tileX),RAND( i*tileY, (i+1)*tileY ),0);
			m_junctionArray->push_back(osg::Vec3(temp));
			
		}
	}
	


}

void CPath::createNet(unsigned int number)
{
	
	
	double tile = m_size.x() / sqrt(double(number)) * 1.2;

	createJunction(number);

	for (unsigned int i=0;i<m_junctionArray->size();i++)
	{
		for (unsigned int j=i+1;j<m_junctionArray->size();j++)
		{
			double length,x,y,z;
			x = (*m_junctionArray)[i].x()-(*m_junctionArray)[j].x();
			y = (*m_junctionArray)[i].y()-(*m_junctionArray)[j].y();
			z = (*m_junctionArray)[i].z()-(*m_junctionArray)[j].z();
			length = sqrt( x * x + y * y + z * z );
			if (length < tile )
				m_junctionAdjacency[i][j] = m_junctionAdjacency[j][i] = 1;
			
		}
	}

}


osg::ref_ptr<osg::Geode> CPath::outputScene()
{
	
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;

	for (unsigned i=0;i<m_junctionArray->size();i++)
	{
		osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;

		for (unsigned j=i+1;j<m_junctionArray->size();j++)
		{
			if ( m_junctionAdjacency[i][j] == 1 )
			{
				vertices->push_back((*m_junctionArray)[i]);
				vertices->push_back((*m_junctionArray)[j]);
			}
		}
		osg::ref_ptr<osg::Geometry> road = new osg::Geometry;
		road->setVertexArray(vertices.get());
		road->addPrimitiveSet(new osg::DrawArrays(osg::DrawArrays::LINES,0,vertices->size()));
		geode->addDrawable(road.get());
	}

	geode->getOrCreateStateSet()->setAttribute(new osg::LineWidth(2.0));
	return geode.release();


}


void CPath::saveToFile()
{
	std::ofstream outfile("data/junction.txt");
	assert(outfile != NULL);

	std::stringstream ss;
	std::string str;
	for ( int i = 0; i != m_junctionArray->size(); ++i)
	{
		ss << (*m_junctionArray)[i].x();
		ss >> str;
		outfile << str ;
		ss.clear();
		str.clear();

		ss << (*m_junctionArray)[i].y();
		ss >> str;
		outfile << "   " << str;
		ss.clear();
		str.clear();

		ss << (*m_junctionArray)[i].z();
		ss >> str;
		outfile << "   " << str << std::endl;
		ss.clear();
		str.clear();
		
	}
	outfile.close();
	

	outfile.open("data/adjacency.txt");
	for ( int i = 0; i != m_junctionArray->size(); ++i)
	{
		for ( int j = 0; j != m_junctionArray->size(); ++j)
		{
			ss << m_junctionAdjacency[i][j];
			ss >> str;
			outfile << str << " ";
			ss.clear();
			str.clear();
		}
		outfile << std::endl;
		

	}




}


void CPath::loadFromFile()
{
 	std::ifstream infile("../data/PathPoints.txt");
	assert( infile != NULL);

	std::stringstream ss;
	std::string line,word;
	double x,y,z;
	
	while (getline(infile,line))
	{
		osg::Vec3d v;
		ss << line;
		ss >> x;
		ss >> y;
		ss >> z;
		v.set(x,y,z);
		m_junctionArray->push_back(v);
		ss.clear();
	}

	infile.close();
	
	//用同一个infile的时候有错误，待修正

	std::ifstream  file; 
	file.open("../data/Adjacency.txt");
	assert( file != NULL);

	unsigned int temp;
	int i = 0,j = 0;
	while (getline( file,line))
	{
		ss << line;
		while ( ss >> temp)
		{
			if( temp == 1)
				m_junctionAdjacency[i][j] = 1;
			j++;
		}
		j = 0;
		ss.clear();
		i++;
	}




}
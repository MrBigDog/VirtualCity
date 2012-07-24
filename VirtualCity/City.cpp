#include "City.h"


osg::Drawable* City::createPlane(osg::Vec3 a,osg::Vec3 b,osg::Vec3 c,osg::Vec3 d)
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


	osg::ref_ptr<osg::Geometry> area = new osg::Geometry;
	area->setVertexArray(vertices.get());
	area->setNormalArray(normals.get());
	area->setNormalBinding(osg::Geometry::BIND_OVERALL);
	area->addPrimitiveSet(new osg::DrawArrays(osg::DrawArrays::QUADS,0,4));

	return area.release();
}

void City::createArea()
{
	osg::Vec3 a,b,c,d;
	a.set(0.0,0.0,0.0);
	b.set(m_lengthArea,0.0,0.0);
	c.set(m_lengthArea,m_lengthArea,0.0);
	d.set(0.0,m_lengthArea,0.0);

	int increment = m_lengthArea + m_widthRoad;

	for ( int i = 0; i < ROW; i++)
	{
		for ( int j = 0; j < COLUMN; j++)
		{
			initArea(a,c);
	
			m_geodeArea->addDrawable(createPlane(a,b,c,d));
			a += osg::Vec3(increment,0.0,0.0);
			b += osg::Vec3(increment,0.0,0.0);
			c += osg::Vec3(increment,0.0,0.0);
			d += osg::Vec3(increment,0.0,0.0);
		}
		
		a += osg::Vec3(0.0,increment,0.0);
		b += osg::Vec3(0.0,increment,0.0);
		c += osg::Vec3(0.0,increment,0.0);
		d += osg::Vec3(0.0,increment,0.0);

		a.x() = 0.0;
		b.x() = m_lengthArea;
		c.x() = m_lengthArea;
		d.x() = 0.0;
		
	}

}

void City::initArea(const osg::Vec3& min, const osg::Vec3& max)
{
	Area area;
	area.minX = min.x();
	area.minY = min.y();
	area.maxX = max.x();
	area.maxY = max.y();
	for (size_t i = 0; i < m_divide; i++)
	{
		for (size_t j = 0; j < m_divide; j++)
			area.occupy[i][j] = 0;
	}
	area.full = false;
	m_arrayArea.push_back(area);

}

void City::setArea( Area& area)
{
	for (size_t i = 0; i < m_divide; i++)
	{
		for (size_t j = 0; j < m_divide; j++)
		{
			if( area.occupy[i][j] == 0)
				return;
		}
	}
	area.full = true;
}

osg::Geode* City::createBuilding()
{
	float gridWidth = m_lengthArea / m_divide;
	
	
	float length = RAND(MinSize,gridWidth);	
	float width = RAND(MinSize,gridWidth);
	float height = RAND(gridWidth,HEIGHT);

	float startX = ( gridWidth - width ) / 2.0;
	float startY = ( gridWidth - length ) / 2.0;

	osg::Vec3 a,b,c,d;
	osg::Vec3 A,B,C,D;
	
	a.set(startX,startY,OFFSET);
	b.set(startX+width,startY,OFFSET);
	c.set(startX+width,startY+length,OFFSET);
	d.set(startX,startY+length,OFFSET);

	A.set(startX,startY,height);
	B.set(startX+width,startY,height);
	C.set(startX+width,startY+length,height);
	D.set(startX,startY+length,height);

	osg::ref_ptr<osg::Geode> building = new osg::Geode;
	building->addDrawable(createPlane(a,b,c,d));//底面
	building->addDrawable(createPlane(A,B,C,D));//上面
	building->addDrawable(createPlane(a,b,B,A));//前面
	building->addDrawable(createPlane(c,d,D,C));//后面
	building->addDrawable(createPlane(d,a,A,D));//左面
	building->addDrawable(createPlane(b,c,C,B));//右面

	return building.release();
	
}

osg::Group* City::createAreaBuildings( size_t index)
{
	float gridWidth = m_lengthArea / m_divide;
	osg::ref_ptr<osg::Group> group = new osg::Group;

	for( size_t i = 0; i < m_divide; i++)
		for ( size_t j = 0; j < m_divide; j++)
		{
			if( m_arrayArea[index].occupy[i][j] == 1)
				continue;
			else
			{
				osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
				mt->setMatrix( osg::Matrix::translate( j*gridWidth,i*gridWidth,0.0));
				mt->addChild(createBuilding());
				group->addChild(mt.get());

				m_arrayArea[0].occupy[i][j] = 1;
			}
		}

	return group.release();


}

void City::createCityBuildings()
{
	
	for( size_t k = 0; k != m_arrayArea.size(); k++)
	{
		size_t m = k / ROW;
		size_t n = k % COLUMN;

		osg::ref_ptr<osg::Group> group = new osg::Group;
		osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
		mt->setMatrix( osg::Matrix::translate( n * (m_lengthArea+m_widthRoad), m * (m_lengthArea+m_widthRoad),0.0));
		group->addChild(mt.get());
		mt->addChild(createAreaBuildings(k));

		m_buildings->addChild(group.get());		
	}
	
}


void City::writePathToFile( const std::string str)
{
	std::ofstream outfile(str.c_str());
	float x = m_widthRoad * -0.5;
	float y = m_widthRoad * -0.5;
	float offside = m_lengthArea + m_widthRoad;
	for ( size_t i = 0; i < ROW + 1; ++i)
	{
		for ( size_t j = 0; j < COLUMN + 1; ++j )
		{
			outfile << x + j * offside << " "
				    << y + i * offside << " "
					<< 0.1 << std::endl;
		}

	}
		
}


void City::writeAdjacencyToFile(const std::string str)
{
	size_t num = ROW + 1;
	
	std::vector< std::vector<size_t> > arr;
	for ( size_t i = 0; i != num*num; ++i)
	{
		std::vector<size_t> vec(num*num,0);
		arr.push_back(vec);
	}
		
	for ( size_t i = 0; i != num*num; ++i)
	{
		size_t j  = 0;
		j = i + 1;
		if ( j != num * num && i / num == j / num)
		{
			arr[i][j] = 1;
			arr[j][i] = 1;
		}
		j = i + num;
		if ( j != num * num)
		{
			arr[i][j] = 1;
			arr[j][i] = 1;
		}

		
		arr[i][i] = 1;

	}

	std::ofstream outfile(str.c_str());
	for ( size_t i = 0; i != num * num; ++i)
	{
		for ( size_t j = 0; j != num * num; ++j)
		{
			outfile << arr[i][j] << " ";
		}

		outfile << std::endl;
	}
	
}

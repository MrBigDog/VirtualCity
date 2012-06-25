#include "City.h"


osg::Drawable* City::createArea(osg::Vec3 a,osg::Vec3 b,osg::Vec3 c,osg::Vec3 d)
{
	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
	vertices->push_back(a);
	vertices->push_back(b);
	vertices->push_back(c);
	vertices->push_back(d);

	osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array(4);
	(*normals)[0].set(0.0,0.0,1.0);
	(*normals)[1].set(0.0,0.0,1.0);
	(*normals)[2].set(0.0,0.0,1.0);
	(*normals)[3].set(0.0,0.0,1.0);

	osg::ref_ptr<osg::Geometry> area = new osg::Geometry;
	area->setVertexArray(vertices.get());
	area->setNormalArray(normals.get());
	area->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
	area->addPrimitiveSet(new osg::DrawArrays(osg::DrawArrays::QUADS,0,4));

	return area.release();
}

void City::createCity()
{
	osg::Vec3 a,b,c,d;
	a.set(0.0,0.0,0.0);
	b.set(m_lengthArea,0.0,0.0);
	c.set(m_lengthArea,m_lengthArea,0.0);
	d.set(0.0,m_lengthArea,0.0);

	int increment = m_lengthArea + m_widthRoad;

	for ( int i = 0; i < 10; i++)
	{
		for ( int j = 0; j < 10; j++)
		{
			initArea(a,c);
	
			m_geode->addDrawable(createArea(a,b,c,d));
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
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			area.occupy[i][j] = 0;
	}
	area.full = false;
	m_arrayArea.push_back(area);

}

void setArea( Area& area)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if( area.occupy[i][j] == 0)
				return;
		}
	}
	area.full = true;
}
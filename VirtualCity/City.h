#ifndef CITY_H
#define CITY_H

#include <vector>
#include <osg/Drawable>
#include <osg/Geode>
#include <osg/Geometry>

struct Area
{
	int maxX;
	int minX;
	int maxY;
	int minY;
	int occupy[3][3];
	bool full;
};

class City
{
public:
	City(int x = 30,int y = 10):m_lengthArea(x),m_widthRoad(y)
	{
		m_geode = new osg::Geode;

	}

	osg::Drawable* createArea(osg::Vec3 a,osg::Vec3 b,osg::Vec3 c,osg::Vec3 d);
	void createCity();
	osg::ref_ptr<osg::Geode> getCity(){ return m_geode;}
	void initArea(const osg::Vec3& min, const osg::Vec3& max);
	void setArea( Area& area);

private:
	int m_lengthArea;
	int m_widthRoad;
	osg::ref_ptr<osg::Geode> m_geode;
	std::vector<Area> m_arrayArea;
};

#endif
#ifndef CITY_H
#define CITY_H

#include <vector>
#include <osg/Drawable>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/MatrixTransform>
#include <time.h>


#define OFFSET 0.1
#define RAND(min, max) ((min) + (int)rand()%((int)(max)-(int)(min)))

struct Area
{
	size_t maxX;
	size_t minX;
	size_t maxY;
	size_t minY;
	size_t occupy[3][3];
	bool full;
};

class City
{
public:
	//默认area长宽为30，划分为3×3，马路宽度为10
	City(size_t x = 30,size_t y = 10,size_t m = 3):m_lengthArea(x),m_widthRoad(y),m_divide(m),m_seed(0)
	{
		m_geodeArea = new osg::Geode;
		m_buildings = new osg::Group;
		m_seed = (unsigned)time( NULL );
	}

	osg::Drawable* createPlane(osg::Vec3 a,osg::Vec3 b,osg::Vec3 c,osg::Vec3 d);
	void createArea();
	osg::ref_ptr<osg::Geode> getArea(){ return m_geodeArea;}
	osg::ref_ptr<osg::Group> getBuildings(){ return m_buildings;}
	void initArea(const osg::Vec3& min, const osg::Vec3& max);
	void setArea( Area& area);
	osg::Geode* createBuilding();
	void createCityBuildings();
	osg::Group* createAreaBuildings( size_t index);


private:
	size_t m_lengthArea;
	size_t m_widthRoad;
	size_t m_divide;
	osg::ref_ptr<osg::Geode> m_geodeArea;
	std::vector<Area> m_arrayArea;
	osg::ref_ptr<osg::Group> m_buildings;
	size_t m_seed;
};

#endif
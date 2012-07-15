#ifndef CITY_H
#define CITY_H

#include <vector>
#include <osg/Drawable>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/MatrixTransform>
#include <time.h>
#include <fstream>
#include <iostream>




#define OFFSET 0.1
#define RAND(min, max)  (min) + ( (double)rand() / ((double)RAND_MAX+1) * ((max) - (min)) )   
#define ROW 10
#define COLUMN 10
#define HEIGHT 30.0
#define MinSize 3.0


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
	City(size_t x = 30,size_t y = 10,size_t m = 3):m_lengthArea(x),m_widthRoad(y),m_divide(m)
	{
		m_geodeArea = new osg::Geode;
		m_buildings = new osg::Group;
		srand((unsigned)time( NULL ));
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
	void writePathToFile(const std::string str);
	void writeAdjacencyToFile(const std::string str);


private:
	size_t m_lengthArea;
	size_t m_widthRoad;
	size_t m_divide;
	osg::ref_ptr<osg::Geode> m_geodeArea;
	std::vector<Area> m_arrayArea;
	osg::ref_ptr<osg::Group> m_buildings;
	 
};

#endif
#ifndef BOUNDINGBOXBUILDING_H
#define BOUNDINGBOXBUILDING_H


#include <osg/BoundingBox>
#include <osg/Material>
#include <osgDB/WriteFile>
#include <osg/Geode>



#define OFFSET 0.1


class BoundingBoxBuilding
{
public:
	BoundingBoxBuilding( const osg::BoundingBox& bb,osg::Material* material):m_bb(bb),m_material(material)
	{
		m_geode = new osg::Geode;
	}

	osg::Drawable* createPlane(osg::Vec3 a,osg::Vec3 b,osg::Vec3 c,osg::Vec3 d);
	void createBuilding();
	void Save( const std::string& filename)
	{
		osgDB::writeNodeFile(*m_geode,filename);
	}


private:
	osg::BoundingBox m_bb;
	osg::Material* m_material;
	osg::ref_ptr<osg::Geode> m_geode;
};




#endif
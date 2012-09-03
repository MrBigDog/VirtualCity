#include "BoundingBoxBuilding.h"



osg::Drawable* BoundingBoxBuilding::createPlane(osg::Vec3 a,osg::Vec3 b,osg::Vec3 c,osg::Vec3 d)
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

void BoundingBoxBuilding::createBuilding()
{



	float length = m_bb.yMax() - m_bb.yMin();	
	float width = m_bb.xMax() - m_bb.xMin();
	float height = m_bb.zMax() - m_bb.zMin();

	float startX = m_bb.xMin();
	float startY = m_bb.yMin();
	float startZ = m_bb.zMin();

	osg::Vec3 a,b,c,d;
	osg::Vec3 A,B,C,D;

	a.set(startX,startY,startZ);
	b.set(startX+width,startY,startZ);
	c.set(startX+width,startY+length,startZ);
	d.set(startX,startY+length,startZ);

	A.set(startX,startY,startZ+height);
	B.set(startX+width,startY,startZ+height);
	C.set(startX+width,startY+length,startZ+height);
	D.set(startX,startY+length,startZ+height);

	m_geode->addDrawable(createPlane(a,b,c,d));//底面
	m_geode->addDrawable(createPlane(A,B,C,D));//上面
	m_geode->addDrawable(createPlane(a,b,B,A));//前面
	m_geode->addDrawable(createPlane(c,d,D,C));//后面
	m_geode->addDrawable(createPlane(d,a,A,D));//左面
	m_geode->addDrawable(createPlane(b,c,C,B));//右面

	osg::ref_ptr<osg::StateSet> state = new osg::StateSet;
	state->setAttribute(m_material);
	
	m_geode->setStateSet(state.get());
 
	
	

}
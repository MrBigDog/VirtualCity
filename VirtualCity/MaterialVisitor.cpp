#include <osg/NodeVisitor>
#include <osg/StateSet>
#include <osg/Material>
#include <osgDB/ReadFile>
#include <osg/BoundingBox>
#include <osg/ComputeBoundsVisitor>
#include <string>
#include <sstream>

#include "BoundingBoxBuilding.h"



class MaterialVisitor : public osg::NodeVisitor
{
public:
	MaterialVisitor():osg::NodeVisitor(TRAVERSE_ALL_CHILDREN),count(0),m_material(NULL){}
	
	virtual void apply( osg::Node& node)
	{
		traverse(node);
	}

	virtual void apply( osg::Geode& node)
	{
		
		for ( size_t i = 0; i != node.getNumDrawables(); ++i)
		{
			osg::Drawable* drawable = node.getDrawable(i);
			if( !drawable)
				continue;
			if( count == 0)
			{
				osg::StateSet* state = drawable->getStateSet();
				osg::StateAttribute* attribute= state->getAttribute(osg::StateAttribute::MATERIAL);
				osg::Material* material = dynamic_cast<osg::Material*>(attribute);
				m_material = static_cast<osg::Material*>(material->clone(osg::CopyOp::DEEP_COPY_ALL));


			}


			
			count++;

		}

		traverse(node);
	}


	const osg::Material* getMaterial() const { return m_material;}
	osg::Material* getMaterial() { return m_material;}

private:
	int count;
	osg::Material* m_material;
};



int main()
{
	

	
	for ( int i = 1; i != 155 ; i++)
	{
		std::string s1 = "../data/buildingmodels/3ds/s1/";
		std::stringstream ss1;
		ss1 << i;
		s1 += ss1.str();
		s1 += ".3ds";

		osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(s1);
		MaterialVisitor visitor;
		node->accept(visitor);
		osg::Material* material = visitor.getMaterial();

		osg::ComputeBoundsVisitor cbv;
		node->accept(cbv);
		osg::BoundingBox bb = cbv.getBoundingBox();

		BoundingBoxBuilding building(bb,material);
		building.createBuilding();

		std::string s2 = "../data/buildingmodels/lod/";
		std::stringstream ss2;
		ss2 << i;
		s2 += ss2.str();
		s2 += ".ive";
		building.Save(s2);
	}
	



}
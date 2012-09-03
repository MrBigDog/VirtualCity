#include <osg/Node>
#include <osg/Geometry>
#include <iostream>


class XZSwapVistitor : public osg::NodeVisitor
{
public:
	XZSwapVistitor():osg::NodeVisitor(TRAVERSE_ALL_CHILDREN){}

	virtual void apply(osg::Node& node)
	{
		traverse(node);
	}

	virtual void apply( osg::Geode& node)
	{
		for ( size_t i = 0; i != node.getNumDrawables(); ++i)
		{
			osg::Drawable* drawable = node.getDrawable(i);
			osg::Geometry* geom = dynamic_cast<osg::Geometry*>(drawable);
			if( !geom )
				continue;

			osg::Vec3Array* vertices = dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
			
			if ( vertices)
			{
				for ( osg::Vec3Array::iterator iter = vertices->begin();iter != vertices->end()-1; ++iter)
				{
					double x = iter->x();
					double y = iter->y();
					double z = iter->z();
					//std::cout << x << " " << y <<" "<< z << std::endl;
					iter->set(osg::Vec3(x,z,-y));
					//std::cout << iter->x() << " " << iter->y() <<" "<< iter->z() << std::endl;

				}
			}
		}
	}
};
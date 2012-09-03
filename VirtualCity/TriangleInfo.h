/* -*-c++-*- Copyright (C) 2009 Wang Rui <wangray84 at gmail dot com>
 * OpenSceneGraph Engine Book - Design and Implementation
 * How to create a functor to collect geometry information
*/

#include <osg/io_utils>
#include <osg/TriangleFunctor>
#include <osg/Drawable>
#include <osgDB/ReadFile>
#include <iostream>
#include <osgViewer/Viewer>

class AttributePrinter : public osg::Drawable::AttributeFunctor
{
public:
    typedef osg::Drawable::AttributeType AttributeType;
    inline const char* getTypeName( AttributeType type )
    {
        static const char* typeNames[] =
        {
            "Vertices", "Weights", "Normals", "Colors", "Secondary Colors",
            "Fog Coords", "Attribute 6", "Attribute 7",
            "Texture Coords 0", "Texture Coords 1", "Texture Coords 2",
            "Texture Coordinates 3", "Texture Coords 4", "Texture Coords 5",
            "Texture Coords 6", "Texture Coords 7"
        };
        return typeNames[type];
    }
    
    template<typename T>
    void printInfo( AttributeType type, unsigned int size, T* front )
    {
        std::cout << "*** " << getTypeName(type) << ": " << size << std::endl;
        for ( unsigned int i=0; i<size; ++i )
            std::cout << "(" << *(front+i) << "); ";
        std::cout << std::endl << std::endl;
    }
    
    virtual void apply( AttributeType type, unsigned int size, float* front )
    { printInfo( type, size, front ); }
    
    virtual void apply( AttributeType type, unsigned int size, osg::Vec2* front )
    { printInfo( type, size, front ); }
    
    virtual void apply( AttributeType type, unsigned int size, osg::Vec3* front )
    { printInfo( type, size, front ); }
    
    virtual void apply( AttributeType type, unsigned int size, osg::Vec4* front )
    { printInfo( type, size, front ); }
};

struct TrianglePrinter
{
    TrianglePrinter()
    { }
    
    void operator()( const osg::Vec3& v1, const osg::Vec3& v2, const osg::Vec3& v3, bool ) const
    { ++ count;}

	static size_t count;
};

size_t TrianglePrinter::count =  0;

class FindGeometryVisitor : public osg::NodeVisitor
{
public:
    FindGeometryVisitor() : osg::NodeVisitor(TRAVERSE_ALL_CHILDREN) {}
    
    virtual void apply( osg::Node& node )
    { traverse( node ); }
    
    virtual void apply( osg::Geode& node )
    {
        for ( unsigned int i=0; i<node.getNumDrawables(); ++i )
        {
            osg::Drawable* drawable = node.getDrawable( i );
            if ( !drawable ) continue;
            
            osg::TriangleFunctor<TrianglePrinter> triPrinter;
            drawable->accept( triPrinter );
            
			sum = TrianglePrinter::count;
        }
        traverse( node );
    }

	void reset()
	{ 
		sum = 0;
		TrianglePrinter::count = 0;
	}

	static size_t sum;
};
size_t FindGeometryVisitor::sum = 0;

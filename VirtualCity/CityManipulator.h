#ifndef CITYMANIPULATOR_H
#define CITYMANIPULATOR_H



#include <osgGA/OrbitManipulator>



class  CityManipulator : public osgGA::OrbitManipulator
{
	typedef osgGA::OrbitManipulator inherited;

public:

	CityManipulator( int flags = DEFAULT_SETTINGS );
	CityManipulator( const CityManipulator& tm,
					const osg::CopyOp& copyOp = osg::CopyOp::SHALLOW_COPY 
					);

	META_Object( osgGA, CityManipulator );


protected:

	//virtual bool performMovementMiddleMouseButton( const double eventTimeDelta, const double dx, const double dy );
	virtual bool performMovementRightMouseButton( const double eventTimeDelta, const double dx, const double dy );
	virtual bool performMovementLeftMouseButton(const double eventTimeDelta, const double dx, const double dy );
	virtual void rotateTrackball( const float px0, const float py0,const float px1, const float py1, const float scale );
	virtual void panModel( const float dx, const float dy, const float dz = 0.0 );
	virtual void computeHomePosition(const osg::Camera *camera = NULL, bool useBoundingBox = false);
};





#endif
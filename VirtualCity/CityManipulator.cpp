
#include "CityManipulator.h"
#include <osg/io_utils>
#include <osg/ComputeBoundsVisitor>

using namespace osg;
using namespace osgGA;



/// Constructor.
CityManipulator::CityManipulator( int flags )
: inherited( flags )
{
	 setVerticalAxisFixed( false );
}


/// Constructor.
CityManipulator::CityManipulator( const CityManipulator& tm, const CopyOp& copyOp )
: inherited( tm, copyOp )

{
}


// doc in parent
bool CityManipulator::performMovementLeftMouseButton( const double eventTimeDelta, const double dx, const double dy )
{
	float scale = -0.3f * _distance * getThrowScale( eventTimeDelta );
	panModel( dx*scale, dy*scale , 0.0);
	return true;
}




// doc in parent
bool CityManipulator::performMovementRightMouseButton( const double eventTimeDelta, const double dx, const double dy )
{
	// rotate camera
	if( getVerticalAxisFixed() )
		rotateWithFixedVertical( dx, dy );
	else
		rotateTrackball( _ga_t0->getXnormalized(), _ga_t0->getYnormalized(),
		_ga_t1->getXnormalized(), _ga_t1->getYnormalized(),
		getThrowScale( eventTimeDelta ) );
	return true;
}

void CityManipulator::rotateTrackball( const float px0, const float py0,
									   const float px1, const float py1, const float scale )
{
	osg::Vec3d axis;
	float angle;

	trackball( axis, angle, px1, py1, px0, py0 );

	Quat new_rotate;
	axis.x() = 0.0;
	axis.y() = 0.0;
	new_rotate.makeRotate( angle, axis );

	_rotation = _rotation * new_rotate;
}


void CityManipulator::panModel( const float dx, const float dy, const float dz )
{
	Matrix rotation_matrix;
	rotation_matrix.makeRotate( _rotation );

	Vec3d dv( dx, dy, dz );
	 

	_center += dv * rotation_matrix;
}

void CityManipulator::computeHomePosition(const osg::Camera *camera, bool useBoundingBox)
{
	if (getNode())
	{
		osg::BoundingSphere boundingSphere;

		OSG_INFO<<" CameraManipulator::computeHomePosition("<<camera<<", "<<useBoundingBox<<")"<<std::endl;

		if (useBoundingBox)
		{
			// compute bounding box
			// (bounding box computes model center more precisely than bounding sphere)
			osg::ComputeBoundsVisitor cbVisitor;
			getNode()->accept(cbVisitor);
			osg::BoundingBox &bb = cbVisitor.getBoundingBox();

			if (bb.valid()) boundingSphere.expandBy(bb);
			else boundingSphere = getNode()->getBound();
		}
		else
		{
			// compute bounding sphere
			boundingSphere = getNode()->getBound();
		}

		

		// set home position
		setHomePosition(boundingSphere.center() + osg::Vec3d(0.0,-300,400.0f),
			boundingSphere.center(),
			osg::Vec3d(0.0f,0.0f,1.0f),
			_autoComputeHomePosition);
	}
}
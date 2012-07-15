#include "HitVisitor.h"

CHitVisitor::CHitVisitor():osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN),m_isCrush(false)
{
	m_center.set(0.0,0.0,0.0);
	m_bs.init();

}

void CHitVisitor::setCenter(osg::Vec3& end)
{
	m_center = end;
}



void CHitVisitor::apply(osg::MatrixTransform& mt)
{
	osg::MyTransform* transform = dynamic_cast<osg::MyTransform*>(&mt);
	if(transform)
	{
		osg::Vec3 tmp;
		if (!m_isCrush)
		{
			m_bs = transform->getBound();
			tmp = m_center - m_bs._center;
			float length = tmp.length();

			if ( length < 3  && length > 0.001)
				m_isCrush = true;

		}

	}


	
}

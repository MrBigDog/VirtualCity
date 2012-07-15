#include "Signal.h"

CSignal::CSignal():m_interval(5.0)
{
	 m_timer.instance();
	 m_timer.setStartTick();
}

void CSignal::initSignal(const osg::ref_ptr<osg::Vec3Array>& junctionArray,const std::vector< std::vector<unsigned int> >& junctionAdjacency)
{
	std::vector<unsigned int> temp;
	std::vector<osg::Vec3> pointArray;
	for (int i = 0; i != junctionArray->size(); i++ )
	{	
		temp = junctionAdjacency[i];
		pointArray.push_back((*junctionArray)[i]);
		for (int j = 0; j != junctionArray->size(); j++)
		{
			if (temp[j])
			{
				pointArray.push_back((*junctionArray)[j]);
			}
		}
		
		//只有十字路口设置交通灯
		if( pointArray.size() == 5 )
			setSignal(pointArray,i);
		
		pointArray.clear();
	
	}

}


void CSignal::setSignal( const std::vector<osg::Vec3>& pointArray,const int index)
{
	crossingstruct crossing;
	crossing.index = index;
	crossing.interval = m_interval;
	crossing.point = pointArray[0];

	for (int i = 1;i < 5; i++)
	{
		roadstruct road;
		road.direction = pointArray[0] - pointArray[i];
		road.direction.normalize();
		road.color = GREEN;
		crossing.roadArray.push_back(road);
	}


	osg::Vec3 temp = crossing.roadArray[0].direction;
	osg::Vec3 compare;
	double min = 1.0;
	int j;
	for (int i = 1; i < 4; i++)
	{
		compare = crossing.roadArray[i].direction;
		double  d = temp * compare;
		if ( d < min )
		{
			min = d;
			j = i;
		}

	}
	
	 crossing.roadArray[0].color = RED;
	 crossing.roadArray[j].color = RED;


	 m_crossingArray.push_back(crossing);


}

void CSignal::updateSignal()
{
	double elapse = m_timer.time_s();
	for ( int i = 0; i != m_crossingArray.size(); i++ )
	{
		if ( (int)elapse % (int)(m_crossingArray[i].interval) == 0)
		{
			for (int j = 0; j != m_crossingArray[i].roadArray.size(); j++)
			{
				 if (m_crossingArray[i].roadArray[j].color == GREEN)
					m_crossingArray[i].roadArray[j].color = RED;
				 else
					m_crossingArray[i].roadArray[j].color = GREEN;
			}
		}
	}


}
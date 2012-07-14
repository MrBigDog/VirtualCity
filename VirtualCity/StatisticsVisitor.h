#include <osg/Node>
#include <osgDB/ReadFile>
#include <queue>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <osg/MatrixTransform>

/*统计物体在四叉树各层中的分布情况*/

struct St
{
	size_t level;
	size_t numOfChild;
};

struct Cmp
{
	bool operator()(const St s1, const St s2)
	{
		return s1.level > s2.level;
	}
};

class StatisticsVisitor : public osg::NodeVisitor
{
public:
	StatisticsVisitor()
		: osg::NodeVisitor(TRAVERSE_ALL_CHILDREN), _indent(0) {}

	virtual void apply( osg::Group& group )
	{
		St s;
		s.numOfChild = group.getNumChildren();
		s.level = _indent;

		_queue.push(s);


		_indent++;
		traverse( group );
		_indent--;
	}

	virtual void apply( osg::MatrixTransform& mt )
	{

		traverse( mt );

	}

	

	void writeToFile( std::string s)
	{
		std::ofstream outfile(s.c_str());		
		
		size_t count = 0;
		size_t index;



 		while ( _queue.size())
 		{
 			index = _queue.top().level;
 			count += _queue.top().numOfChild;
 			_queue.pop();
 
 			if ( _queue.size() && _queue.top().level == index)
 				continue;
 			else
 			{
				std::stringstream ss;
				ss << "level: " << index << "   " << "children: " << count << std::endl;
				outfile << ss.str();
				count = 0;
				
				if ( _queue.empty())
					break;
 			}		
 			
 		}



	}

protected:
	int _indent;
	std::priority_queue<St,std::vector<St>,Cmp> _queue;

};
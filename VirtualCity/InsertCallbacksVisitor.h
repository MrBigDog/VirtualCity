#include <osg/NodeVisitor>
#include <osg/MatrixTransform>
#include <osgUtil/CullVisitor>
#include <assert.h>
#include <fstream>

class CullCallback : public osg::NodeCallback
{
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{ 
		osg::MatrixTransform* mt = dynamic_cast<osg::MatrixTransform*> (node);
		assert( mt != NULL );
		osgUtil::CullVisitor* visitor = dynamic_cast<osgUtil::CullVisitor*>(nv);
		assert( visitor != NULL);

		s_count++;

		node->setCullingActive(true);
		bool b = visitor->isCulled(*node);
		node->setCullingActive(false);

		if (b)
			return;
		else
			traverse(node,nv);


	}

private:
	static size_t s_count;
};

size_t CullCallback::s_count;


class InsertCallbacksVisitor : public osg::NodeVisitor
{

public:

	InsertCallbacksVisitor():osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN),outfile("../data/bound.txt")
	{
	}

	virtual void apply(osg::MatrixTransform& node)
	{
		node.setCullCallback(new CullCallback());
		node.setCullingActive(false);

		traverse(node);
	}
private:
	std::fstream outfile;
};
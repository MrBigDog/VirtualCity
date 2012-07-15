
#ifndef MYTRANSFORM_H
#define MYTRANSFORM_H 

#include <osg/MatrixTransform>

namespace osg {

 
	class   MyTransform : public MatrixTransform
	{
	public :


		MyTransform():MatrixTransform(),m_dirction(0.0,1.0,0.0){}
		MyTransform(const MyTransform& transform,const CopyOp& copyop=CopyOp::SHALLOW_COPY):MatrixTransform(transform,copyop){}

		META_Node(osg, MyTransform);
		virtual MyTransform* asMyTransform() { return this; }
		virtual const MyTransform* asMyTransform() const { return this; }

	private:
		osg::Vec3 m_dirction;

	public:
		osg::Vec3 getDirction(){ return m_dirction;}
		void setDirection( osg::Vec3 d) { m_dirction = d;}
 
	
	};

}

#endif


 

 

 
 
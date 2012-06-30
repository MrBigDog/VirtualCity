#ifndef MYMATRIXTRANSFORM_H
#define MYMATRIXTRANSFORM_H 

#include <osg/MatrixTransform>


class  MyMatrixTransform : public osg::MatrixTransform
{
public :


	MyMatrixTransform();

	/** Copy constructor using CopyOp to manage deep vs shallow copy.*/
	MyMatrixTransform(const MyMatrixTransform& mt,const CopyOp& copyop=CopyOp::SHALLOW_COPY):osg::MatrixTransform(mt,copyop){}

	MyMatrixTransform(const Matrix& matix):osg::MatrixTransform(matix){}

	META_Node(osg, MyMatrixTransform);

	virtual MyMatrixTransform* asMyMatrixTransform() { return this; }
	virtual const MyMatrixTransform* asMyMatrixTransform() const { return this; }



protected :

	virtual ~MatrixTransform(){}

};



#endif

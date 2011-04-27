/*======================================================================

  This file is part of the elastix software.

  Copyright (c) University Medical Center Utrecht. All rights reserved.
  See src/CopyrightElastix.txt or http://elastix.isi.uu.nl/legal.php for
  details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE. See the above copyright notices for more information.

======================================================================*/

#ifndef __elxRayCastResampleInterpolator_hxx
#define __elxRayCastResampleInterpolator_hxx

#include "elxRayCastResampleInterpolator.h"
#include "itkBSplineInterpolateImageFunction.h"
#include "itkImageFileWriter.h"

namespace elastix
{
  using namespace itk;

/*
 * ***************** BeforeAll *****************
 */


template <class TElastix>
int
RayCastResampleInterpolator<TElastix>
::BeforeAll( void )
{
  // Check if 2D-3D
  if ( this->m_Elastix->GetFixedImage()->GetImageDimension() != 3 )
  {
    itkExceptionMacro( << "The RayCastInterpolator expects the fixed image to be 3D." );
    return 1;
  }
  if ( this->m_Elastix->GetMovingImage()->GetImageDimension() != 3 )
  {
    itkExceptionMacro( << "The RayCastInterpolator expects the moving image to be 3D." );
    return 1;
  }

 return 0;
}


/*
 * ***************** BeforeRegistration *****************
 */

template <class TElastix>
void
RayCastResampleInterpolator<TElastix>
::BeforeRegistration( void )
{

 	m_CombinationTransform = CombinationTransformType::New();
	m_CombinationTransform->SetUseComposition( true );

	typedef typename elastix::OptimizerBase<TElastix>::ITKBaseType::ParametersType ParametersType;
   	unsigned int numberofparameters = this->m_Elastix->GetElxTransformBase()->GetAsITKBaseType()->GetNumberOfParameters();
   	TransformParametersType preParameters( numberofparameters );
	preParameters.Fill(0.);

   		for( unsigned int i = 0; i < numberofparameters ; i++ )
		{

   		  bool ret = this->GetConfiguration()->ReadParameter( preParameters[i],
              	  "PreParameters", this->GetComponentLabel(), i, 0 );
   			if (!ret)
   			{
			  std::cerr<<" Error, not enough PreParameters are given"<<std::endl;
   			}

		}

	EulerTransformType::InputPointType centerofrotation;
	centerofrotation.Fill(0.);

		for( unsigned int i = 0; i < this->m_Elastix->GetMovingImage()->GetImageDimension() ; i++ )
		{

   		  bool ret = this->GetConfiguration()->ReadParameter( centerofrotation[i],
              	  "CenterOfRotationPoint", this->GetComponentLabel(), i, 0 );

		}

	m_PreTransform = EulerTransformType::New();
	m_PreTransform->SetParameters( preParameters );
	m_PreTransform->SetCenter( centerofrotation );
	m_CombinationTransform->SetInitialTransform( m_PreTransform );
	m_CombinationTransform->SetCurrentTransform( this->m_Elastix->GetElxTransformBase()->GetAsITKBaseType() );

	this->SetTransform( m_CombinationTransform );
	this->SetInputImage( this->m_Elastix->GetMovingImage() );

	PointType focalPoint;
	focalPoint.Fill( 0. );

		for( unsigned int i = 0; i < this->m_Elastix->GetFixedImage()->GetImageDimension()  ; i++ )
		{

   		  bool ret = this->GetConfiguration()->ReadParameter( focalPoint[i],
              	  "FocalPoint", this->GetComponentLabel(), i, 0 );
   			if (!ret)
   			{
			  std::cerr<<" Error, FocalPoint not assigned"<<std::endl;
   			}

		}

	this->SetFocalPoint( focalPoint );

	this->m_Elastix->GetElxResamplerBase()->GetAsITKBaseType()->SetTransform( this->m_CombinationTransform );

	double threshold = 0.;
	this->GetConfiguration()->ReadParameter( threshold, "Threshold", 0);
	this->SetThreshold( threshold );

}


/*
 * ***************** AfterEachResolution *****************
 */

template <class TElastix>
void
RayCastResampleInterpolator<TElastix>
::AfterEachResolution( void )
{


	m_CombinationTransform = CombinationTransformType::New();
	m_CombinationTransform->SetUseComposition( true );

	typedef typename elastix::OptimizerBase<TElastix>::ITKBaseType::ParametersType ParametersType;
   	unsigned int numberofparameters = this->m_Elastix->GetElxTransformBase()->GetAsITKBaseType()->GetNumberOfParameters();
   	TransformParametersType preParameters( numberofparameters );
	TransformParametersType finalParameters( numberofparameters );
	finalParameters.Fill(0.);
	preParameters.Fill(0.);

   		for( unsigned int i = 0; i < numberofparameters ; i++ )
		{

   		  bool ret = this->GetConfiguration()->ReadParameter( preParameters[i],
              	  "PreParameters", this->GetComponentLabel(), i, 0 );
   			if (!ret)
   			{
			  std::cerr<<" Error, not enough PreParameters are given"<<std::endl;
   			}

		}

	m_PreTransform = EulerTransformType::New();
	m_PreTransform->SetParameters( preParameters );
	m_CombinationTransform->SetInitialTransform( m_PreTransform );
	finalParameters = this->m_Elastix->GetElxTransformBase()->GetAsITKBaseType()->GetParameters();

	m_CombinationTransform->SetCurrentTransform( this->m_Elastix->GetElxTransformBase()->GetAsITKBaseType() );

	this->SetTransform( m_CombinationTransform );
	this->SetInputImage( this->m_Elastix->GetMovingImage() );

	PointType focalPoint;
	focalPoint.Fill( 0. );

		for( unsigned int i = 0; i < this->m_Elastix->GetFixedImage()->GetImageDimension()  ; i++ )
		{

   		  bool ret = this->GetConfiguration()->ReadParameter( focalPoint[i],
              	  "FocalPoint", this->GetComponentLabel(), i, 0 );
   			if (!ret)
   			{
			  std::cerr<<" Error, FocalPoint not assigned"<<std::endl;
   			}

		}

	this->SetFocalPoint( focalPoint );

	double threshold = 0.;
	this->GetConfiguration()->ReadParameter( threshold, "Threshold", 0);
	this->SetThreshold( threshold );


}


/*
 * ***************** ReadFromFile *****************
 */

template <class TElastix>
void
RayCastResampleInterpolator<TElastix>
::ReadFromFile( void )
{

	/** Call ReadFromFile of the ResamplerBase. */
  	this->Superclass2::ReadFromFile();

 	m_CombinationTransform = CombinationTransformType::New();
	m_CombinationTransform->SetUseComposition( true );

	typedef typename elastix::OptimizerBase<TElastix>::ITKBaseType::ParametersType ParametersType;
   	unsigned int numberofparameters = this->m_Elastix->GetElxTransformBase()->GetAsITKBaseType()->GetNumberOfParameters();
   	TransformParametersType preParameters( numberofparameters );

   		for( unsigned int i = 0; i < numberofparameters ; i++ )
		{

   		  bool ret = this->GetConfiguration()->ReadParameter( preParameters[i],
              	  "PreParameters", this->GetComponentLabel(), i, 0 );
   			if (!ret)
   			{
			  std::cerr<<" Error, not enough PreParameters are given"<<std::endl;
   			}

		}

	m_PreTransform = EulerTransformType::New();
	m_PreTransform->SetParameters( preParameters );
	m_CombinationTransform->SetInitialTransform( m_PreTransform );
	m_CombinationTransform->SetCurrentTransform( this->m_Elastix->GetElxTransformBase()->GetAsITKBaseType() );

	this->SetTransform( m_CombinationTransform );
	this->SetInputImage( this->m_Elastix->GetMovingImage() );

	PointType imageorigin;
	SpacingType imagespacing;
	SizeType imagesize;
	unsigned int dim = 0;

	for( dim = 0; dim < ImageDimension; dim++ )
	{
	  this->m_Configuration->ReadParameter( imageorigin[ dim ], "Origin", dim );
	  this->m_Configuration->ReadParameter( imagesize[ dim ], "Size", dim );
	  this->m_Configuration->ReadParameter( imagespacing[ dim ], "Spacing", dim );
	}

	PointType focalPoint;
	focalPoint.Fill( 0. );

		for( unsigned int i = 0; i < this->m_Elastix->GetFixedImage()->GetImageDimension()  ; i++ )
		{

   		  bool ret = this->GetConfiguration()->ReadParameter( focalPoint[i],
              	  "FocalPoint", this->GetComponentLabel(), i, 0 );
   			if (!ret)
   			{
			  std::cerr<<" Error, FocalPoint not assigned"<<std::endl;
   			}

		}

	this->SetFocalPoint( focalPoint );

	double threshold = 0.;
	this->GetConfiguration()->ReadParameter( threshold, "Threshold", 0);
	this->SetThreshold( threshold );

}

/**
 * ******************* WriteToFile ******************************
 */

template <class TElastix>
void
RayCastResampleInterpolator<TElastix>
::WriteToFile( void ) const
{
   /** Call WriteToFile of the ResamplerBase. */
  this->Superclass2::WriteToFile();

  PointType imageorigin;
  imageorigin.Fill(0);
  unsigned int dim = 0;

  for( dim = 0; dim < ImageDimension; dim++ )
  {
    this->m_Configuration->ReadParameter( imageorigin[ dim ], "Origin", dim );
  }

  PointType focalpoint = this->GetFocalPoint();

  xout["transpar"] << "(FocalPoint "
    << focalpoint << ")" << std::endl;

  double threshold = this->GetThreshold();
  xout["transpar"] << "(Threshold "
    << threshold << ")" << std::endl;

} // end WriteToFile()

} // end namespace elastix

#endif // end #ifndef __elxBSplineResampleInterpolator_hxx


/*=========================================================================
 *
 *  Copyright UMC Utrecht and contributors
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef elxTransformixFilter_h
#define elxTransformixFilter_h

#include "itkImageSource.h"

#include "elxTransformixMain.h"
#include "elxParameterObject.h"
#include "elxPixelTypeName.h"

/**
 * Transformix library exposed as an ITK filter.
 */

namespace elastix {

template< typename TInputImage >
class TransformixFilter : public itk::ImageSource< TInputImage >
{
public:

  typedef TransformixFilter               Self;
  typedef itk::SmartPointer< Self >       Pointer;
  typedef itk::SmartPointer< const Self > ConstPointer;
  itkNewMacro( Self );
  itkTypeMacro( Self, itk::ImageSource );

  typedef elastix::TransformixMain                          TransformixMainType;
  typedef TransformixMainType::Pointer                      TransformixMainPointer;
  typedef TransformixMainType::ArgumentMapType              ArgumentMapType;
  typedef ArgumentMapType::value_type                       ArgumentMapEntryType;

  typedef itk::ProcessObject::DataObjectIdentifierType      DataObjectIdentifierType;
  typedef TransformixMainType::DataObjectContainerType      DataObjectContainerType;
  typedef TransformixMainType::DataObjectContainerPointer   DataObjectContainerPointer;

  typedef ParameterObject::ParameterMapVectorType           ParameterMapVectorType;
  typedef ParameterObject::ParameterMapType                 ParameterMapType;
  typedef ParameterObject::ParameterValueVectorType         ParameterValueVectorType;
  typedef typename ParameterObject::Pointer                 ParameterObjectPointer;
  typedef typename ParameterObject::ConstPointer            ParameterObjectConstPointer;

  typedef typename TInputImage::Pointer                     InputImagePointer;

  itkStaticConstMacro( InputImageDimension, unsigned int, TInputImage::ImageDimension );

  void SetInputImage( InputImagePointer inputImage );

  itkSetMacro( InputPointSetFileName, std::string );
  itkGetMacro( InputPointSetFileName, std::string );
  void RemoveInputPointSetFileName() { this->m_InputPointSetFileName = std::string(); };

  itkSetMacro( ComputeSpatialJacobian, bool );
  itkGetConstMacro( ComputeSpatialJacobian, bool );
  itkBooleanMacro( ComputeSpatialJacobian );

  itkSetMacro( ComputeDeterminantOfSpatialJacobian, bool );
  itkGetConstMacro( ComputeDeterminantOfSpatialJacobian, bool );
  itkBooleanMacro( ComputeDeterminantOfSpatialJacobian );

  itkSetMacro( ComputeDeformationField, bool );
  itkGetConstMacro( ComputeDeformationField, bool );
  itkBooleanMacro( ComputeDeformationField );

  void SetTransformParameterObject( ParameterObjectPointer parameterObject );
  ParameterObjectPointer GetTransformParameterObject( void );

  itkSetMacro( OutputDirectory, std::string );
  itkGetConstMacro( OutputDirectory, std::string );
  void RemoveOutputDirectory() { this->m_OutputDirectory = std::string(); };

  void SetLogFileName( std::string logFileName )
  {
    this->m_LogFileName = logFileName;
    this->LogToFileOn();
    this->Modified();
  }

  itkGetConstMacro( LogFileName, std::string );

  void RemoveLogFileName( void ) {
    this->m_LogFileName = std::string();
    this->LogToFileOff();
  };

  itkSetMacro( LogToConsole, bool );
  itkGetConstMacro( LogToConsole, bool );
  itkBooleanMacro( LogToConsole );

  itkSetMacro( LogToFile, bool );
  itkGetConstMacro( LogToFile, bool );
  itkBooleanMacro( LogToFile );

protected:

  void GenerateData( void ) ITK_OVERRIDE;

private:

  TransformixFilter();

  std::string   m_InputPointSetFileName;
  bool          m_ComputeSpatialJacobian;
  bool          m_ComputeDeterminantOfSpatialJacobian;
  bool          m_ComputeDeformationField;

  std::string   m_OutputDirectory;
  std::string   m_LogFileName;

  bool          m_LogToConsole;
  bool          m_LogToFile;


};

} // namespace elx

#ifndef ITK_MANUAL_INSTANTIATION
#include "elxTransformixFilter.hxx"
#endif

#endif // elxTransformixFilter_h

/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkImageLinearConstIteratorWithIndex.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkImageLinearConstIteratorWithIndex_h
#define __itkImageLinearConstIteratorWithIndex_h

#include "itkImageConstIteratorWithIndex.h"

namespace itk
{

/** \class ImageLinearConstIteratorWithIndex
 * \brief A multi-dimensional image iterator that visits image pixels within a
 * region in a "scan-line" order.
 *
 * ImageLinearConstIteratorWithIndex is templated over image type and is
 * constrained to walk within a specified image region. It is designed for
 * line-by-line processing of images.  This iterator walks a linear path along
 * a selected image direction that is parallel to one of the coordinate axes
 * of the image.  The iterator conceptually breaks the image into a set of
 * parallel lines that span the selected image dimension.
 *
 * ImageLinearConstIteratorWithIndex assumes a particular layout of the image
 * data. The is arranged in a 1D array as if it were [][][][slice][row][col]
 * with Index[0] = col, Index[1] = row, Index[2] = slice, etc.
 *
 * operator++ provides a simple syntax for walking around a region of a
 * multidimensional image. operator++ iterates across a preselected direction
 * constraining the movement to within a region of image. The user can verify
 * when the iterator reaches the boundary of the region along this direction,
 * by calling the IsAtEndOfLine() method. Then it is possible to pass to the
 * next line starting at the first pixel in the row that is part of the region
 * by calling the NextLine() method.
 *
 * This is the typical use of this iterator in a loop:
 *
 * \code
 *
 * ImageLinearConstIteratorWithIndex<ImageType> it( image, image->GetRequestedRegion() );
 *
 * it.SetDirection(2);
 * it.GoToBegin();
 * while( !it.IsAtEnd() )
 * {
 *   while( !it.IsAtEndOfLine() )
 *   {
 *      value = it.Get();  // it.Set() doesn't exist in the Const Iterator
 *      ++it;
 *   }
 *   it.NextLine();
 *  }
 *
 *  \endcode
 *
 * \example  Examples/Iterators/ImageLinearIteratorWithIndex.cxx
 *
 * \par MORE INFORMATION
 * For a complete description of the ITK Image Iterators and their API, please
 * see the Iterators chapter in the ITK Software Guide.  The ITK Software Guide
 * is available in print and as a free .pdf download from http://www.itk.org.
 *
 * \ingroup ImageIterators
 *
 * \sa ImageConstIterator \sa ConditionalConstIterator
 * \sa ConstNeighborhoodIterator \sa ConstShapedNeighborhoodIterator
 * \sa ConstSliceIterator  \sa CorrespondenceDataStructureIterator
 * \sa FloodFilledFunctionConditionalConstIterator
 * \sa FloodFilledImageFunctionConditionalConstIterator
 * \sa FloodFilledImageFunctionConditionalIterator
 * \sa FloodFilledSpatialFunctionConditionalConstIterator
 * \sa FloodFilledSpatialFunctionConditionalIterator
 * \sa ImageConstIterator \sa ImageConstIteratorWithIndex
 * \sa ImageIterator \sa ImageIteratorWithIndex
 * \sa ImageLinearConstIteratorWithIndex  \sa ImageLinearIteratorWithIndex
 * \sa ImageRandomConstIteratorWithIndex  \sa ImageRandomIteratorWithIndex
 * \sa ImageRegionConstIterator \sa ImageRegionConstIteratorWithIndex
 * \sa ImageRegionExclusionConstIteratorWithIndex
 * \sa ImageRegionExclusionIteratorWithIndex
 * \sa ImageRegionIterator  \sa ImageRegionIteratorWithIndex
 * \sa ImageRegionReverseConstIterator  \sa ImageRegionReverseIterator
 * \sa ImageReverseConstIterator  \sa ImageReverseIterator
 * \sa ImageSliceConstIteratorWithIndex  \sa ImageSliceIteratorWithIndex
 * \sa NeighborhoodIterator \sa PathConstIterator  \sa PathIterator
 * \sa ShapedNeighborhoodIterator  \sa SliceIterator
 * \sa ImageConstIteratorWithIndex
 *
 */
template<typename TImage>
class ITK_EXPORT ImageLinearConstIteratorWithIndex : public ImageConstIteratorWithIndex<TImage>
{
public:
  /** Standard class typedefs. */
  typedef ImageLinearConstIteratorWithIndex       Self;
  typedef ImageConstIteratorWithIndex<TImage>     Superclass;

  /** Index typedef support. While this was already typdef'ed in the superclass
   * it needs to be redone here for this subclass to compile properly with gcc.
   * Note that we have to rescope Index back to itk::Index to that is it not
   * confused with ImageIterator::Index. */
  typedef typename TImage::IndexType   IndexType;

  /** Region typedef support. While this was already typdef'ed in the superclass
   * it needs to be redone here for this subclass to compile properly with gcc.
   * Note that we have to rescope Region back to itk::ImageRegion so that is
   * it not confused with ImageIterator::Index. */
  typedef typename TImage::RegionType RegionType;

  /** Image typedef support. While this was already typdef'ed in the superclass
   * it needs to be redone here for this subclass to compile properly with gcc.
   * Note that we have to rescope Index back to itk::Index to that is it not
   * confused with ImageIterator::Index. */
  typedef TImage ImageType;

  /** PixelContainer typedef support. Used to refer to the container for
   * the pixel data. While this was already typdef'ed in the superclass
   * it needs to be redone here for this subclass to compile properly with gcc. */
  typedef typename TImage::PixelContainer     PixelContainer;
  typedef typename PixelContainer::Pointer    PixelContainerPointer;

  /** Default constructor. Needed since we provide a cast constructor. */
  ImageLinearConstIteratorWithIndex() : ImageConstIteratorWithIndex<TImage>() {}

  /** Constructor establishes an iterator to walk a particular image and a
   * particular region of that image. */
  ImageLinearConstIteratorWithIndex(const ImageType *ptr, const RegionType& region);

  /** Constructor that can be used to cast from an ImageIterator to an
   * ImageLinearConstIteratorWithIndex. Many routines return an ImageIterator but for a
   * particular task, you may want an ImageLinearConstIteratorWithIndex.  Rather than
   * provide overloaded APIs that return different types of Iterators, itk
   * returns ImageIterators and uses constructors to cast from an
   * ImageIterator to a ImageLinearConstIteratorWithIndex. */
  ImageLinearConstIteratorWithIndex( const ImageConstIteratorWithIndex<TImage> &it)
    { this->ImageConstIteratorWithIndex<TImage>::operator=(it); }

  /** Go to the next line.
   * \sa operator++  \sa operator-- \sa IsAtEndOfLine \sa PreviousLine \sa End */
  inline void NextLine(void);

  /** Go to the previous line.
   * \sa operator++ \sa operator-- \sa IsAtEndOfLine \sa NextLine \sa End */
  inline void PreviousLine(void);

  /** Go to the beginning pixel of the current line.
   * \sa GoToReverseBeginOfLine \sa operator++ \sa operator-- \sa NextLine \sa IsAtEndOfLine */
  void GoToBeginOfLine(void);

  /** Go to the beginning pixel of the current line.
   * \sa GoToBeginOfLine \sa operator++ \sa operator-- \sa NextLine \sa IsAtEndOfLine */
  void GoToReverseBeginOfLine(void);

  /** Go to the past end pixel of the current line.
   * \sa GoToBeginOfLine \sa operator++ \sa operator-- \sa NextLine \sa IsAtEndOfLine */
  void GoToEndOfLine(void);

  /** Test if the index is at the end of line */
  inline bool IsAtEndOfLine(void)
    {
    return this->m_PositionIndex[m_Direction] >= this->m_EndIndex[m_Direction];
    }

  /** Test if the index is at the begin of line */
  inline bool IsAtReverseEndOfLine(void)
    {
    return this->m_PositionIndex[m_Direction] < this->m_BeginIndex[m_Direction];
    }

  /** Set the direction of movement */
  inline void SetDirection(unsigned int direction)
    {
    if( direction >= TImage::ImageDimension )
      {
      itkGenericExceptionMacro(<<"In image of dimension " << TImage::ImageDimension <<" Direction " << direction << " sas selected");
      }
    m_Direction = direction;
    m_Jump = this->m_OffsetTable[ m_Direction ];
    }

  /** Increment (prefix) the selected dimension.
   * No bounds checking is performed. \sa GetIndex \sa operator-- */
  inline Self & operator++()
    {
    this->m_PositionIndex[m_Direction]++;
    this->m_Position += m_Jump;
    return *this;
    }

  /** Decrement (prefix) the selected dimension.
   * No bounds checking is performed.  \sa GetIndex \sa operator++ */
  inline Self & operator--()
    {
    this->m_PositionIndex[m_Direction]--;
    this->m_Position -= m_Jump;
    return *this;
    }

private:
    unsigned long  m_Jump;
    unsigned int   m_Direction;
};

//----------------------------------------------------------------------
//  Go to next line
//----------------------------------------------------------------------
template<class TImage>
inline
void
ImageLinearConstIteratorWithIndex<TImage>
::NextLine(void)
{

  this->m_Position -= this->m_OffsetTable[ m_Direction ] *
    ( this->m_PositionIndex[ m_Direction ] - this->m_BeginIndex[ m_Direction ] );

  this->m_PositionIndex[m_Direction] = this->m_BeginIndex[m_Direction];

  for( unsigned int n=0; n<TImage::ImageDimension; n++ )
    {

    this->m_Remaining = false;

    if( n == m_Direction )
      {
      continue;
      }

    this->m_PositionIndex[ n  ]++;
    if( this->m_PositionIndex[n] <  this->m_EndIndex[n] )
      {
      this->m_Position += this->m_OffsetTable[ n ];
      this->m_Remaining = true;
      break;
      }
    else
      {
      this->m_Position -= this->m_OffsetTable[ n ] * ( this->m_Region.GetSize()[n]-1 );
      this->m_PositionIndex[ n ] = this->m_BeginIndex[ n ];
      }
    }
}


//----------------------------------------------------------------------
//  Pass to the last pixel on the previous line
//----------------------------------------------------------------------
template<class TImage>
inline
void
ImageLinearConstIteratorWithIndex<TImage>
::PreviousLine(void)
{

  this->m_Position += this->m_OffsetTable[ m_Direction ] *
    ( this->m_EndIndex[ m_Direction ] - 1 - this->m_PositionIndex[ m_Direction ] );

  this->m_PositionIndex[m_Direction] = this->m_EndIndex[m_Direction]-1;


  for( unsigned int n=0; n<TImage::ImageDimension; n++ )
    {

    this->m_Remaining = false;

    if( n == m_Direction )
      {
      continue;
      }

    this->m_PositionIndex[ n  ]--;
    if( this->m_PositionIndex[ n ] >=  this->m_BeginIndex[ n ] )
      {
      this->m_Position -= this->m_OffsetTable[ n ];
      this->m_Remaining = true;
      break;
      }
    else
      {
      this->m_Position += this->m_OffsetTable[ n ] * ( this->m_Region.GetSize()[n]-1 );
      this->m_PositionIndex[ n ] = this->m_EndIndex[ n ] - 1;
      }
    }
}
} // end namespace itk

// Define instantiation macro for this template.
#define ITK_TEMPLATE_ImageLinearConstIteratorWithIndex(_, EXPORT, x, y) namespace itk { \
  _(1(class EXPORT ImageLinearConstIteratorWithIndex< ITK_TEMPLATE_1 x >)) \
  namespace Templates { typedef ImageLinearConstIteratorWithIndex< ITK_TEMPLATE_1 x > \
                        ImageLinearConstIteratorWithIndex##y; } \
  }

#if ITK_TEMPLATE_EXPLICIT
# include "Templates/itkImageLinearConstIteratorWithIndex+-.h"
#endif

#if ITK_TEMPLATE_TXX
# include "itkImageLinearConstIteratorWithIndex.txx"
#endif

#endif

#
#  Example on the use of the CannyEdgeDetectionImageFilter
#

from InsightToolkit import *

from sys import argv


reader = itkImageFileReaderF2_New()
writer = itkImageFileWriterUC2_New()

outputCast = itkRescaleIntensityImageFilterF2UC2_New()

filter  = itkCannyEdgeDetectionImageFilterF2F2_New()

filter.SetInput(      reader.GetOutput()      )
outputCast.SetInput(  filter.GetOutput()      )
writer.SetInput(      outputCast.GetOutput()  )

reader.SetFileName( argv[1] )
writer.SetFileName( argv[2] )

outputCast.SetOutputMinimum(    0  )
outputCast.SetOutputMaximum(  255  )

variance  = eval( argv[3] )
lowerThreshold = eval( argv[4] )
upperThreshold = eval( argv[5] )

filter.SetVariance(  variance  )
filter.SetLowerThreshold( lowerThreshold )
filter.SetUpperThreshold( upperThreshold )

writer.Update()



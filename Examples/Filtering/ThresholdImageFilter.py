#
#  Example on the use of the ThresholdImageFilter
#

from InsightToolkit import *

from sys import argv


reader = itkImageFileReaderUS2_New()
writer = itkImageFileWriterUS2_New()


filter  = itkThresholdImageFilterUS2_New()

filter.SetInput( reader.GetOutput() )
writer.SetInput( filter.GetOutput() )

reader.SetFileName( argv[1] )
writer.SetFileName( argv[2] )

filter.SetOutsideValue( eval( argv[3] )  )
filter.ThresholdAbove(  eval( argv[4] )  )


writer.Update()




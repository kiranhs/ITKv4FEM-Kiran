/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#ifndef __itkFEMMacro_h
#define __itkFEMMacro_h

/**
 * \file itkFEMMacro.h
 * \brief Definitions of macros used in FEM code.
 *
 * itkFEMMacro.h defines macros that allow simple and consistent FEM code
 * creation. Use these macros whenever posible (always)!
 */

/**
 * \brief If defined, FEM classes will use smart pointers.
 *
 * Define this macro if you want to compile the FEM classes so that
 * they use itk's SmartPointer object instead of standard c++ pointers.
 */
#define FEM_USE_SMART_POINTERS

// Include this first, to avoid duplicate linking problems on some platforms
// (MSVC6).
#include <fstream>

#include "itkFEMObjectFactory.h"

/* Required includes for itk's SmartPointer compatibility */
#ifdef FEM_USE_SMART_POINTERS
#include "itkMacro.h"
#include "itkSmartPointer.h"
#include "itkObjectFactory.h"
#include "itkLightObject.h"
#endif


#endif // #ifndef __itkFEMMacro_h

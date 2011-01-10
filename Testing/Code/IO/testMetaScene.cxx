/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    testMetaScene.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <stdio.h>
#include <ctype.h>
#include <cstdlib>
#include <metaScene.h>
#include <metaGroup.h>
#include <metaEllipse.h>
#include <itksys/SystemTools.hxx>

int testMetaScene(int argc, char * argv[])
{
  if (argc > 1)
    {
    itksys::SystemTools::ChangeDirectory(argv[1]);
    }

  std::cout << "Creating test scene ..." << std::endl;
  MetaScene * s = new MetaScene(3);

  MetaEllipse * e1 = new MetaEllipse(3);
  e1->ID(0);
  e1->Radius(3);

  MetaEllipse * e2 = new MetaEllipse(3);
  e2->ID(1);
  e2->Radius(4);

  MetaGroup g0;
  MetaGroup * g1 = new MetaGroup(3);
  g1->FileName("MyFilename");
  g1->ID(2);

//  MetaGroup g2(g1);
//  g2.PrintInfo();

  e1->ParentID(2);
  e2->ParentID(2);

  s->AddObject(g1);
  s->AddObject(e1);
  s->AddObject(e2);

  std::cout << "...[ok]" << std::endl;

  std::cout << "Writing test file ..." << std::endl;

  s->Write("scene.scn");

  std::cout << "...[ok]" << std::endl;

  std::cout << "Clearing the scene..." << std::endl;
  s->Clear();
  std::cout << "...[ok]" << std::endl;

  std::cout << "Reading test file ..." << std::endl;

  s->Read("scene.scn");

  if(s->NObjects() != 3)
    {
    std::cout << "Number of obejcts: " << s->NObjects()
              << " != 3...[FAILED]" << std::endl;
    return EXIT_FAILURE;
    }

  std::cout << "...[ok]" << std::endl;

  s->Clear();

  std::cout << "Writing single object..." << std::endl;

  e1 = new MetaEllipse(3);
  e1->ID(0);
  e1->Radius(3);
  e1->Write("ellipse.elp");
  delete e1;

  std::cout << "[OK]" << std::endl;

  s->Clear();

  std::cout << "Reading test file ..." << std::endl;

  s->Read("ellipse.elp");

  if(s->NObjects() != 1)
    {
    std::cout << "Number of obejcts: " << s->NObjects()
              << " != 1...[FAILED]" << std::endl;
    delete s;
    return EXIT_FAILURE;
    }

  delete s;
  std::cout << "[OK]" << std::endl;
  return EXIT_SUCCESS;
}

/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkPVPolyData.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

Copyright (c) 1998-2000 Kitware Inc. 469 Clifton Corporate Parkway,
Clifton Park, NY, 12065, USA.

All rights reserved. No part of this software may be reproduced, distributed,
or modified, in any form or by any means, without permission in writing from
Kitware Inc.

IN NO EVENT SHALL THE AUTHORS OR DISTRIBUTORS BE LIABLE TO ANY PARTY FOR
DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
OF THE USE OF THIS SOFTWARE, ITS DOCUMENTATION, OR ANY DERIVATIVES THEREOF,
EVEN IF THE AUTHORS HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE AUTHORS AND DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES, INCLUDING,
BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE, AND NON-INFRINGEMENT.  THIS SOFTWARE IS PROVIDED ON AN
"AS IS" BASIS, AND THE AUTHORS AND DISTRIBUTORS HAVE NO OBLIGATION TO PROVIDE
MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

=========================================================================*/
#include "vtkPVData.h"
#include "vtkPVPolyData.h"
#include "vtkPVShrinkPolyData.h"
#include "vtkPVElevationFilter.h"
#include "vtkPVConeSource.h"
#include "vtkPVGlyph3D.h"
#include "vtkKWView.h"

#include "vtkKWScale.h"
#include "vtkKWPushButton.h"
#include "vtkKWEntry.h"
#include "vtkPVWindow.h"
#include "vtkPVAssignment.h"

#include "vtkConeSource.h"
#include "vtkPVApplication.h"

int vtkPVPolyDataCommand(ClientData cd, Tcl_Interp *interp,
		                     int argc, char *argv[]);


//----------------------------------------------------------------------------
vtkPVPolyData::vtkPVPolyData()
{
  this->CommandFunction = vtkPVPolyDataCommand;
}

//----------------------------------------------------------------------------
vtkPVPolyData::~vtkPVPolyData()
{
}

//----------------------------------------------------------------------------
vtkPVPolyData* vtkPVPolyData::New()
{
  return new vtkPVPolyData();
}

//----------------------------------------------------------------------------
void vtkPVPolyData::Shrink()
{
  vtkPVApplication *pvApp = (vtkPVApplication *)this->Application;
  //shrink factor defaults to 0.5, which seems reasonable
  vtkPVShrinkPolyData *shrink;
  vtkPVPolyData *pvd;
  vtkPVAssignment *a;
  vtkPVWindow *window = this->GetPVSource()->GetWindow();
  
  shrink = vtkPVShrinkPolyData::New();
  shrink->Clone(pvApp);
  pvd = vtkPVPolyData::New();
  pvd->Clone(pvApp);
  
  shrink->SetInput(this);
  shrink->SetOutput(pvd);
  a = this->GetAssignment();
  shrink->SetAssignment(a);
  
  shrink->SetName("shrink");

  this->GetPVSource()->GetView()->AddComposite(shrink);
  // Turn this data object off so the next will will not be ocluded.
  this->GetPVSource()->VisibilityOff();

  // The window here should probably be replaced with the view.
  window->SetCurrentSource(shrink);
  window->GetSourceList()->Update();
  
  this->GetPVSource()->GetView()->Render();
  
  shrink->Delete();
}

//----------------------------------------------------------------------------
void vtkPVPolyData::Glyph()
{
  vtkPVApplication *pvApp = this->GetPVApplication();
  vtkPVConeSource *glyphCone;
  vtkPVGlyph3D *glyph;
  vtkPVPolyData *pvd;
  vtkPVPolyData *glyphOut;
  vtkPVAssignment *a;
  vtkPVWindow *window = this->GetPVSource()->GetWindow();

  glyph = vtkPVGlyph3D::New();
  glyph->Clone(pvApp);
  pvd = vtkPVPolyData::New();
  pvd->Clone(pvApp);

  glyphCone = vtkPVConeSource::New();
  glyphCone->Clone(pvApp);
  glyphOut = vtkPVPolyData::New();
  glyphOut->Clone(pvApp);
  
  glyphCone->SetOutput(glyphOut);
  window->GetMainView()->AddComposite(glyphCone);
  glyphCone->SetName("glyph source");
  
  glyph->SetInput(this);
  glyph->SetOutput(pvd);
  glyph->SetSource(glyphOut);
  glyph->SetGlyphSource(glyphCone);
  glyph->SetScaleModeToDataScalingOff();
  a = this->GetAssignment();
  glyph->SetAssignment(a);
  
  glyphCone->VisibilityOff();
    
  glyph->SetName("glyph");
 
  this->GetPVSource()->GetView()->AddComposite(glyph);
  this->GetPVSource()->VisibilityOff();
  
  window->SetCurrentSource(glyph);
  window->GetSourceList()->Update();
  
  this->GetPVSource()->GetView()->Render();
  
  glyphCone->Delete();
}


  
//----------------------------------------------------------------------------
void vtkPVPolyData::Elevation()
{
  vtkPVApplication *pvApp = (vtkPVApplication *)this->Application;
  vtkPVElevationFilter *elevation;
  vtkPVPolyData *newData;
  vtkPVAssignment *a;
  float *bounds;

  // This should go through the PVData who will collect the info.
  bounds = this->GetPolyData()->GetBounds();
  
  elevation = vtkPVElevationFilter::New();
  elevation->Clone(pvApp);
  newData = vtkPVPolyData::New();
  newData->Clone(pvApp);
  
  elevation->SetInput(this);
  elevation->SetOutput(newData);
  a = this->GetAssignment();
  elevation->SetAssignment(a);
  
  this->GetPVSource()->GetView()->AddComposite(elevation);
  elevation->SetName("elevation");
  
  vtkPVWindow *window = this->GetPVSource()->GetWindow();
  this->GetPVSource()->VisibilityOff();
  
  window->SetCurrentSource(elevation);

  elevation->SetLowPoint(bounds[0], 0.0, 0.0);
  elevation->SetHighPoint(bounds[1], 0.0, 0.0);
  
  this->GetPVSource()->GetView()->Render();
  elevation->Delete();
}

//----------------------------------------------------------------------------
int vtkPVPolyData::Create(char *args)
{
  if (this->vtkPVData::Create(args) == 0)
    {
    return 0;
    }
  
  this->FiltersMenuButton->AddCommand("vtkShrinkPolyData", this,
				      "Shrink");
  this->FiltersMenuButton->AddCommand("vtkElevationFilter", this,
				      "Elevation");
  this->FiltersMenuButton->AddCommand("vtkGlyph3D", this,
				      "Glyph");

  return 1;
}

//----------------------------------------------------------------------------
void vtkPVPolyData::SetPolyData(vtkPolyData *data)
{
  this->SetData(data);
  this->Mapper->SetInput(data);
  this->Actor->SetMapper(this->Mapper);
}

//----------------------------------------------------------------------------
vtkPolyData *vtkPVPolyData::GetPolyData()
{
  return (vtkPolyData*)this->Data;
}


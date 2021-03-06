/*
   ____    _ __           ____               __    ____
  / __/___(_) /  ___ ____/ __ \__ _____ ___ / /_  /  _/__  ____
 _\ \/ __/ / _ \/ -_) __/ /_/ / // / -_|_-</ __/ _/ // _ \/ __/
/___/\__/_/_.__/\__/_/  \___\_\_,_/\__/___/\__/ /___/_//_/\__(_)

Copyright 2012 SciberQuest Inc.
*/
#ifndef __StreamlineData_h
#define __StreamlineData_h

#include "FieldTopologyMapData.h"

#include<vector>
using std::vector;

#include<map>
using std::map;
using std::pair;

class IdBlock;
class vtkDataSet;
class vtkFloatArray;
class vtkCellArray;
class vtkUnsignedCharArray;
class vtkIdTypeArray;
class vtkSQCellGenerator;

/// Interface to the topology map.
/**
Abstract collection of datastructures needed to build the topology map.
The details of building the map change drastically depending on the input
data type. Concrete classes deal with these specifics.
*/
class StreamlineData : public FieldTopologyMapData
{
public:
  StreamlineData()
        :
    SourcePts(0),
    SourceCells(0),
    OutPts(0),
    OutCells(0),
    Length(0),
    SourceId(0)
       {}

  virtual ~StreamlineData();

  // Description:
  // Set the dataset to be used as the seed source. Use either
  // a dataset or a cell generator. The dataset explicitly contains
  // all geometry that will be accessed.
  virtual void SetSource(vtkDataSet *s);

  // Description:
  // Set the cell generator to be used as the seed source. Use either
  // a dataset or a cell generator. The cell generator implicitly contains
  // all geometry that will be accessed, generating only what is needed
  // on demand.
  virtual void SetSource(vtkSQCellGenerator *s);

  // Description:
  // Copy the IntersectColor and SourceId array into the output.
  virtual void SetOutput(vtkDataSet *o);

  // Description:
  // Convert a list of seed cells (sourceIds) to FieldLine
  // structures and build the output (if any).
  virtual vtkIdType InsertCells(IdBlock *SourceIds);

  // Description:
  // Move streamline geometry from the internal structure
  // into the vtk output data.
  virtual int SyncGeometry();

  /**
  Set the global id of cell 0 in this processes source cells.
  */
  void SetSourceCellGid(unsigned long gid){ this->SourceCellGid=gid; }
  unsigned long GetSourceCellGid(){ return this->SourceCellGid; }

  /**
  Remove segments that resulted from a periodic bc application
  from the output set of lines. The whole bounding box is used
  to identify these segments.
  */
  void CullPeriodicTransitions(double *bounds);

private:
  void ClearSource();
  void ClearOut();

private:
  vtkFloatArray *SourcePts;
  vtkCellArray *SourceCells;

  vtkFloatArray *OutPts;
  vtkCellArray *OutCells;
  vtkFloatArray *Length;
  vtkIntArray *SourceId;

  unsigned int SourceCellGid;
};

#endif

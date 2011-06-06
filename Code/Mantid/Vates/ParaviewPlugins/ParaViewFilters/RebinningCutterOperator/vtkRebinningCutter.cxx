#include "MantidVatesAPI/MDHistogramRebinningPresenter.h"
#include "MantidVatesAPI/NullRebinningPresenter.h"
#include "ClipperAdapter.h"

#include "vtkRebinningCutter.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkAlgorithm.h"
#include "vtkPVClipDataSet.h"
#include "vtkSmartPointer.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkImplicitFunction.h"
#include "vtkPointData.h"
#include "vtkBox.h"

#include "MantidKernel/Exception.h"
#include "MantidMDAlgorithms/PlaneImplicitFunction.h"
#include "MantidMDAlgorithms/BoxImplicitFunction.h"
#include "MantidMDAlgorithms/NullImplicitFunction.h"
#include "MantidGeometry/MDGeometry/IMDDimensionFactory.h"
#include "MantidVatesAPI/EscalatingRebinningActionManager.h"
#include "MantidVatesAPI/RebinningCutterXMLDefinitions.h"
#include "MantidVatesAPI/vtkThresholdingUnstructuredGridFactory.h"
#include "MantidVatesAPI/vtkThresholdingHexahedronFactory.h"
#include "MantidVatesAPI/vtkThresholdingQuadFactory.h"
#include "MantidVatesAPI/vtkThresholdingLineFactory.h"
#include "MantidVatesAPI/IMDWorkspaceProxy.h"
#include "MantidVatesAPI/vtkProxyFactory.h"
#include "MantidVatesAPI/TimeToTimeStep.h"
#include "MantidVatesAPI/FilteringUpdateProgressAction.h"
#include "MantidVatesAPI/Common.h"
#include "MantidVatesAPI/vtkDataSetToGeometry.h" 
#include "MantidGeometry/MDGeometry/MDGeometryXMLParser.h"
#include "MantidGeometry/MDGeometry/MDGeometryXMLBuilder.h"

#include <boost/functional/hash.hpp>
#include <sstream>

/** Plugin for ParaView. Performs simultaneous rebinning and slicing of Mantid data.

@author Owen Arnold, Tessella plc
@date 14/03/2011

Copyright &copy; 2010 ISIS Rutherford Appleton Laboratory & NScD Oak Ridge National Laboratory

This file is part of Mantid.

Mantid is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

Mantid is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

File change history is stored at: <https://svn.mantidproject.org/mantid/trunk/Code/Mantid>
Code Documentation is available at: <http://doxygen.mantidproject.org>
*/






vtkImplicitFunction* vtkRebinningCutter::getImplicitFunction() const
{
  return m_clipFunction;
}

double vtkRebinningCutter::getMaxThreshold() const
{
  return m_thresholdMax;
}

double vtkRebinningCutter::getMinThreshold() const
{
  return m_thresholdMin;
}

bool vtkRebinningCutter::getApplyClip() const
{
  return m_clip == ApplyClipping;
}

double vtkRebinningCutter::getTimeStep() const
{
  return m_timestep;
}

const char* vtkRebinningCutter::getAppliedGeometryXML() const
{
  return m_appliedGeometryXML.c_str();
}


vtkCxxRevisionMacro(vtkRebinningCutter, "$Revision: 1.0 $")
  ;
vtkStandardNewMacro(vtkRebinningCutter)
  ;

using namespace Mantid::VATES;

vtkRebinningCutter::vtkRebinningCutter() :
m_presenter(new NullRebinningPresenter()),
  m_clipFunction(NULL),
  m_clip(ApplyClipping),
  m_originalExtents(IgnoreOriginal),
  m_setup(Pending),
  m_timestep(0),
  m_thresholdMax(10000),
  m_thresholdMin(0)
{
  this->SetNumberOfInputPorts(1);
  this->SetNumberOfOutputPorts(1);
}

vtkRebinningCutter::~vtkRebinningCutter()
{
  delete m_presenter;
}


int vtkRebinningCutter::RequestData(vtkInformation* vtkNotUsed(request), vtkInformationVector **inputVector,
  vtkInformationVector *outputVector)
{
  using namespace Mantid::VATES;

  //Setup is not complete until metadata has been correctly provided.
  if(SetupDone == m_setup)
  {

    FilterUpdateProgressAction<vtkRebinningCutter> updatehandler(this);

    vtkInformation *outInfo = outputVector->GetInformationObject(0);
    vtkUnstructuredGrid *output = vtkUnstructuredGrid::SafeDownCast(outInfo->Get(
      vtkDataObject::DATA_OBJECT()));

    if (outInfo->Has(vtkStreamingDemandDrivenPipeline::UPDATE_TIME_STEPS()))
    {
      // usually only one actual step requested
      m_timestep = outInfo->Get(vtkStreamingDemandDrivenPipeline::UPDATE_TIME_STEPS())[0];
    }

    std::string scalarName = XMLDefinitions::signalName();
    vtkThresholdingLineFactory* vtkGridFactory = new vtkThresholdingLineFactory(scalarName, m_thresholdMin, m_thresholdMax);
    vtkThresholdingQuadFactory* p_2dSuccessorFactory = new vtkThresholdingQuadFactory(scalarName, m_thresholdMin, m_thresholdMax);
    vtkThresholdingHexahedronFactory* p_3dSuccessorFactory = new vtkThresholdingHexahedronFactory(scalarName, m_thresholdMin, m_thresholdMax);
    vtkThresholdingUnstructuredGridFactory<TimeToTimeStep>* p_4dSuccessorFactory = new vtkThresholdingUnstructuredGridFactory<TimeToTimeStep>(scalarName, m_timestep, m_thresholdMin, m_thresholdMax);
    vtkGridFactory->SetSuccessor(p_2dSuccessorFactory);
    p_2dSuccessorFactory->SetSuccessor(p_3dSuccessorFactory);
    p_3dSuccessorFactory->SetSuccessor(p_4dSuccessorFactory);


    vtkUnstructuredGrid* outData = m_presenter->execute(vtkGridFactory, updatehandler);
    delete vtkGridFactory;

    output->ShallowCopy(outData);
  }
  return 1;
}

void vtkRebinningCutter::UpdateAlgorithmProgress(double progress)
{
  this->SetProgressText("Executing Mantid Rebinning Algorithm...");
  this->UpdateProgress(progress);
}

int vtkRebinningCutter::RequestInformation(vtkInformation* vtkNotUsed(request), vtkInformationVector **inputVector,
  vtkInformationVector *outputVector)
{
  using namespace Mantid::VATES;

  enum Status{Bad=0, Good=1};
  Status status=Good;
  if (Pending == m_setup)
  {
    vtkInformation * inputInf = inputVector[0]->GetInformationObject(0);
    vtkDataSet * inputDataset = vtkDataSet::SafeDownCast(inputInf->Get(vtkDataObject::DATA_OBJECT()));

    using namespace Mantid::VATES;

    RebinningActionManager* requester = new EscalatingRebinningActionManager();
    requester->ask(RecalculateAll);
    MDHistogramRebinningPresenter<vtkRebinningCutter>* temp;
    try
    {
      temp = new MDHistogramRebinningPresenter<vtkRebinningCutter>(inputDataset, requester, this, new ClipperAdapter(vtkPVClipDataSet::New()));
      delete this->m_presenter;
      m_presenter = temp;
      m_appliedGeometryXML = m_presenter->getAppliedGeometryXML();
      m_setup = SetupDone;
    }
    catch(std::logic_error&)
    {
      vtkErrorMacro("Rebinning operations require Rebinning Metadata. Have you provided a rebinning source?");
      status = Bad;
    }
  }

  setTimeRange(outputVector);
  return status;
}

int vtkRebinningCutter::RequestUpdateExtent(vtkInformation* vtkNotUsed(info), vtkInformationVector** vtkNotUsed(inputVector),
  vtkInformationVector* vtkNotUsed(outputVector))
{
  return 1;
}
;

int vtkRebinningCutter::FillInputPortInformation(int vtkNotUsed(port), vtkInformation* info)
{
  info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkDataSet");
  return 1;
}

void vtkRebinningCutter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

void vtkRebinningCutter::SetApplyClip(int applyClip)
{
  Clipping temp = applyClip == 1 ? ApplyClipping : IgnoreClipping;
  if(temp != m_clip)
  {
    m_clip = temp;
    m_presenter->updateModel();
    this->Modified();
  }
}

void vtkRebinningCutter::SetClipFunction(vtkImplicitFunction * func)
{
  vtkBox* box = dynamic_cast<vtkBox*>(func);

  if (box != m_clipFunction)
  {
    this->m_clipFunction = box;
    m_presenter->updateModel();
    this->Modified();
  }
}

void vtkRebinningCutter::SetMaxThreshold(double maxThreshold)
{
  if (maxThreshold != m_thresholdMax)
  {
    this->m_thresholdMax = maxThreshold;
    m_presenter->updateModel();
    this->Modified();
  }
}

void vtkRebinningCutter::SetMinThreshold(double minThreshold)
{
  if (minThreshold != m_thresholdMin)
  {
    this->m_thresholdMin = minThreshold;
    m_presenter->updateModel();
    this->Modified();
  }
}


void vtkRebinningCutter::SetAppliedGeometryXML(std::string appliedGeometryXML)
{
  if(SetupDone == m_setup)
  {
    m_appliedGeometryXML = appliedGeometryXML;
    m_presenter->updateModel();
    this->Modified(); 
  }
}

const char* vtkRebinningCutter::GetInputGeometryXML()
{
  try
  {
    return this->m_presenter->getAppliedGeometryXML().c_str(); //TODO, check xml lives beyond function call.
  }
  catch(std::runtime_error&)
  {
    return "";
  }
}

unsigned long vtkRebinningCutter::GetMTime()
{
  unsigned long mTime = this->Superclass::GetMTime();
  unsigned long time;

  if (this->m_clipFunction != NULL)
  {

    time = this->m_clipFunction->GetMTime();
    if(time > mTime)
    {
      mTime = time;
    }
  }

  return mTime;
}



void vtkRebinningCutter::setTimeRange(vtkInformationVector* outputVector)
{
  if(SetupDone == m_setup)
  {
    if(m_presenter->hasTDimensionAvailable())
    {
      vtkInformation *outInfo = outputVector->GetInformationObject(0);
      std::vector<double> timeStepValues = m_presenter->getTimeStepValues();
      outInfo->Set(vtkStreamingDemandDrivenPipeline::TIME_STEPS(), &timeStepValues[0],
        static_cast<int> (timeStepValues.size()));
      double timeRange[2];
      timeRange[0] = timeStepValues.front();
      timeRange[1] = timeStepValues.back();

      outInfo->Set(vtkStreamingDemandDrivenPipeline::TIME_RANGE(), timeRange, 2);
    }
  }
}


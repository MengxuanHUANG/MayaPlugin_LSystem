#include "LSystemNode.h"

#include <maya/MGlobal.h>

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnUnitAttribute.h>

#include <maya/MIOStream.h>
#include <maya/MTime.h>
#include <maya/MFnMesh.h>
#include <maya/MPoint.h>
#include <maya/MFloatPoint.h>
#include <maya/MFloatPointArray.h>
#include <maya/MIntArray.h>
#include <maya/MDoubleArray.h>

#include <maya/MFnStringData.h>
#include <maya/MFnMeshData.h>

#include "LSystem.h"
#include "Cylinder.h"

MTypeId LSystemNode::id(0x80000);
MObject LSystemNode::iAngleAttr;
MObject LSystemNode::iStepAttr;
MObject LSystemNode::iGrammarAttr;
MObject LSystemNode::iTimeAttr;
MObject LSystemNode::oGeometryAttr;

void* LSystemNode::creator()
{
	return new LSystemNode();
}

MStatus LSystemNode::initialize()
{
	MFnNumericAttribute nAttr;
	MFnTypedAttribute tAttr;
	MFnUnitAttribute uAttr;

	MStatus returnStatus;

    // output geometry
    LSystemNode::oGeometryAttr = tAttr.create("OutGeometry", "out", MFnData::kMesh, &returnStatus);
    returnStatus = tAttr.setStorable(false);
    addAttribute(LSystemNode::oGeometryAttr);

    // input attributes
    // input angle
    LSystemNode::iAngleAttr = nAttr.create("iAngle", "ia", MFnNumericData::kFloat, 90.0);
    returnStatus = nAttr.setStorable(false);
    addAttribute(LSystemNode::iAngleAttr);

    // input step
    LSystemNode::iStepAttr = nAttr.create("iStep", "is", MFnNumericData::kFloat, 5.0);
    returnStatus = nAttr.setStorable(false);
    addAttribute(LSystemNode::iStepAttr);

    // input grammar file
    MFnStringData fileFnStringData;
    MObject fileNameDefaultObject = fileFnStringData.create("./");
    LSystemNode::iGrammarAttr = tAttr.create("iGrammar", "ig", MFnData::kString, fileNameDefaultObject);
    returnStatus = tAttr.setStorable(false);
    returnStatus = tAttr.setUsedAsFilename(true);
    addAttribute(LSystemNode::iGrammarAttr);

    // input time
    LSystemNode::iTimeAttr = uAttr.create("iTime", "it", MFnUnitAttribute::kTime, 0.0);
    returnStatus = nAttr.setStorable(false);
    addAttribute(LSystemNode::iTimeAttr);

    returnStatus = attributeAffects(LSystemNode::iAngleAttr, LSystemNode::oGeometryAttr);
    returnStatus = attributeAffects(LSystemNode::iStepAttr, LSystemNode::oGeometryAttr);
    returnStatus = attributeAffects(LSystemNode::iGrammarAttr, LSystemNode::oGeometryAttr);
    returnStatus = attributeAffects(LSystemNode::iTimeAttr, LSystemNode::oGeometryAttr);

	return MS::kSuccess;
}

MStatus LSystemNode::compute(const MPlug& plug, MDataBlock& data)
{
    MStatus returnStatus;
    if (plug == LSystemNode::oGeometryAttr) {
        /* Get time */
        MDataHandle timeData = data.inputValue(iTimeAttr, &returnStatus);
        MTime time = timeData.asTime();

        /* Get angle */
        MDataHandle angleData = data.inputValue(iAngleAttr, &returnStatus);
        double angle = angleData.asDouble();

        /* Get step */
        MDataHandle stepData = data.inputValue(iStepAttr, &returnStatus);
        double step = stepData.asDouble();

        /* Get grammar */
        //MDataHandle grammarData = data.inputValue(iGrammarAttr, &returnStatus);
        MString grammarFile = "";

        /* Get output object */
        MDataHandle outputHandle = data.outputValue(oGeometryAttr, &returnStatus);
        MFnMeshData dataCreator;
        MObject newOutputData = dataCreator.create(&returnStatus);

        ComputeLSystem(angle, step, grammarFile, time, newOutputData, returnStatus);

        outputHandle.set(newOutputData);
        data.setClean(plug);
    }
    else
        return MS::kUnknownParameter;
    return MS::kSuccess;
}

MObject LSystemNode::ComputeLSystem(const double& angle,
                                    const double& step,
                                    const MString& grammarFile,
                                    const MTime& time,
                                    MObject& outData,
                                    MStatus& stat)
{
    LSystem lsystem;
    lsystem.setDefaultAngle(20);
    lsystem.setDefaultStep(1);
    lsystem.loadProgram("E:/Study/Upenn/CIS6600_Advanced_Topics_in_CG/HW2/MayaPlugin_LSystem/plants/simple1.txt");

    std::vector<LSystem::Branch> branches;
    lsystem.process(3, branches);

    MPointArray points;
    MIntArray faceCounts;
    MIntArray faceConnects;

    for (LSystem::Branch& branch : branches)
    {
        const vec3& start = branch.first;
        const vec3& end = branch.second;

        CylinderMesh cylinder(MPoint(start[0], start[1], start[2], 1.f),
                              MPoint(end[0], end[1], end[2], 1.f));

        cylinder.appendToMesh(points, faceCounts, faceConnects);
    }

    MObject newMesh = MFnMesh().create(points.length(), faceCounts.length(), points, faceCounts, faceConnects, outData, &stat);
    return newMesh;
}

#pragma once

#include <maya/MPxNode.h>
#include <maya/MStatus.h>

class LSystemNode : public MPxNode
{
public:
	LSystemNode() {}
	virtual ~LSystemNode() = default;

	virtual MStatus compute(const MPlug& plug, MDataBlock& data);

public:
	static MTypeId  id;

	// input attributes
	static MObject iAngleAttr;
	static MObject iStepAttr;
	static MObject iGrammarAttr;
	static MObject iIterationAttr;

	// out attributes
	static MObject oGeometryAttr;

public:
	static  void* creator();
	static  MStatus initialize();

protected:
	MObject ComputeLSystem(const float& angle,
						   const float& step,
						   const MString& grammarFile,
						   const int& iteration, 
						   MObject& outData, 
						   MStatus& stat);
};


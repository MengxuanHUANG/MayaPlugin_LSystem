#include "LSystemCmd.h"


#include <maya/MGlobal.h>
#include <maya/MArgDatabase.h>
#include <list>
#include "LSystem.h"

const char* stepFlag = "-s", * stepLongFlag = "-stepSize";
const char* angleFlag = "-a", * angleLongFlag = "-angle";
const char* grammarFlag = "-g", * grammarLongFlag = "-grammar";
const char* iterationFlag = "-it", * iterationLongFlag = "-iteration";

MString Vec3toStr(const vec3& p)
{
	for (int i = 0; i < 3; ++i)
	{

	}
	return MString();
}

MSyntax LSystemCmd::newSyntax()
{
	MSyntax syntax;

	syntax.addFlag(stepFlag, stepLongFlag, MSyntax::kDouble);
	syntax.addFlag(angleFlag, angleLongFlag, MSyntax::kDouble);
	syntax.addFlag(grammarFlag, grammarLongFlag, MSyntax::kString);
	syntax.addFlag(iterationFlag, iterationLongFlag, MSyntax::kUnsigned);

	return syntax;
}

LSystemCmd::LSystemCmd() : MPxCommand()
{
}

LSystemCmd::~LSystemCmd() 
{
}

MStatus LSystemCmd::doIt( const MArgList& args )
{
	// message in Maya output window
    cout<<"Implement Me!"<<endl;
	std::cout.flush();

	MArgDatabase argData(newSyntax(), args);
	
	MString grammar;

	if (argData.isFlagSet(grammarFlag))
	{
		argData.getFlagArgument(grammarFlag, 0, grammar);
	}
	unsigned int iteration;
	if (argData.isFlagSet(iterationFlag))
	{
		argData.getFlagArgument(iterationFlag, 0, iteration);
		MString str;
		str += iteration;
	}
	double step;
	if (argData.isFlagSet(stepFlag))
	{
		argData.getFlagArgument(stepFlag, 0, step);
		MString str;
		str += step;
	}
	double angle;
	if (argData.isFlagSet(angleFlag))
	{
		argData.getFlagArgument(angleFlag, 0, angle);
		MString str;
		str += angle;
	}

	// message in scriptor editor
	MString extrude = R"(createNode transform -n LSystem1;
						 createNode mesh -n LSystemShape1 -p LSystem1;
						 sets -add initialShadingGroup LSystemShape1;
						 createNode LSystemNode -n LSystemNode1;
						 setAttr LSystemNode1.iIteration ^1s;
						 setAttr LSystemNode1.iAngle ^2s;
						 setAttr LSystemNode1.iStep ^3s;
						 setAttr LSystemNode1.iGrammar -type "string" "^4s";
					     connectAttr LSystemNode1.OutGeometry LSystemShape1.inMesh; )";
	MString angleStr;
	angleStr += angle;

	MString stepStr;
	stepStr += step;

	MString itStr;
	itStr += iteration;

	MString command;
	command.format(extrude, itStr, angleStr, stepStr, grammar);
	MStatus status = MGlobal::executeCommand(command);
	return MStatus::kSuccess;
}
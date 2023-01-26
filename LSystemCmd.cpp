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
		MGlobal::displayInfo(grammar);
	}
	unsigned int iteration;
	if (argData.isFlagSet(iterationFlag))
	{
		argData.getFlagArgument(iterationFlag, 0, iteration);
		MString str;
		str += iteration;
		MGlobal::displayInfo(str);
	}
	double step;
	if (argData.isFlagSet(stepFlag))
	{
		argData.getFlagArgument(stepFlag, 0, step);
		MString str;
		str += step;
		MGlobal::displayInfo(str);
	}
	double angle;
	if (argData.isFlagSet(angleFlag))
	{
		argData.getFlagArgument(angleFlag, 0, angle);
		MString str;
		str += angle;
		MGlobal::displayInfo(str);
	}

	LSystem lsystem;
	lsystem.setDefaultAngle(angle);

	lsystem.setDefaultStep(step);

	lsystem.loadProgramFromString(grammar.asChar());

	std::vector<LSystem::Branch> branches;
	std::vector<LSystem::Geometry> geometry;
	lsystem.process(iteration, branches, geometry);

	MString curveTemplate = "curve -d 1 -p ^1s -p ^2s -k 0 - k 1;";
	MStatus status;
	for (LSystem::Branch& branch : branches)
	{
		const vec3& start = branch.first;
		const vec3& end = branch.second;

		MString startStr = " ", endStr = " ";
		for (int i = 0; i < 3; ++i)
		{
			startStr += start[i];
			startStr += " ";

			endStr += end[i];
			endStr += " ";
		}

		MString command;
		command.format(curveTemplate, startStr, endStr);
		status = MGlobal::executeCommand(command);
	}

	// message in scriptor editor
	MString extrude = R"(curve -d 1 -p 0 0 0 - p 1 1 1 -k 0 - k 1 - name curve1;
						 select - r ^1s curve1;
						 extrude - ch true - rn false - po 1 - et 2 - ucp 1 - fpt 1 - upn 1 - rotation 0 - scale 1 - rsp 1 "nurbsCircle1" "curve1"; )";
	//MString command;
	//command.format(extrude, "nurbsCircle1");
	//MStatus status;// = MGlobal::executeCommand(command);
	return MStatus::kSuccess;
}
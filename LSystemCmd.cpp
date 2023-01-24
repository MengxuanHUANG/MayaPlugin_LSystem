#include "LSystemCmd.h"


#include <maya/MGlobal.h>
#include <list>
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

	// message in scriptor editor
	MString extrude = R"(curve -d 1 -p 0 0 0 - p 1 1 1 -k 0 - k 1 - name curve1;
						 select - r ^1s curve1;
						 extrude - ch true - rn false - po 1 - et 2 - ucp 1 - fpt 1 - upn 1 - rotation 0 - scale 1 - rsp 1 "nurbsCircle1" "curve1"; )";
	MString command;
	command.format(extrude, "nurbsCircle1");
	MStatus status = MGlobal::executeCommand(command);
	return status;
}


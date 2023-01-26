#ifndef CreateLSystemCmd_H_
#define CreateLSystemCmd_H_

#include <maya/MPxCommand.h>
#include <maya/MArgList.h>
#include <maya/MSyntax.h>

class LSystemCmd : public MPxCommand
{
public:
    LSystemCmd();
    virtual ~LSystemCmd();
    static void* creator() { return new LSystemCmd(); }
    MStatus doIt( const MArgList& args );

    static MSyntax newSyntax();
};

#endif
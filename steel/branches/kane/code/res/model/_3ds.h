//---------------------------------------------------------------------------

#ifndef _3dsH
#define _3dsH

#include "model.h"
#include "bones.h"

class _3DS_Animation: public Bones
{
	virtual bool load(string& name);
	virtual bool unload() { return true; }
};

class _3DS: public Model
{
	virtual bool load(string& name);
	virtual bool unload() { return true; }
};

//---------------------------------------------------------------------------
#endif

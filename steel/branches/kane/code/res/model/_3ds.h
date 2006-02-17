//---------------------------------------------------------------------------

#ifndef _3dsH
#define _3dsH

#include "model.h"


class _3DS: public Model
{
	virtual bool load(string& name);
	virtual bool unload() { return true; }
};

//---------------------------------------------------------------------------
#endif

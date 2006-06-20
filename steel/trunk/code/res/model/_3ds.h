//---------------------------------------------------------------------------

#ifndef _3dsH
#define _3dsH

#include "model.h"
#include <fstream>


class _3DS;

typedef int(F)(_3DS&, rstream&, int);

struct chainProcessor
{
	chainProcessor(): tag(0) {}
	chainProcessor(unsigned short TAG, F _F): tag(TAG), f(_F) {}

	unsigned short tag;
	F *f;
};


class _3DS: public Model
{
public:
	bool init(const std::string name);
};

Model* create3DS(const std::string filename);


//---------------------------------------------------------------------------
#endif

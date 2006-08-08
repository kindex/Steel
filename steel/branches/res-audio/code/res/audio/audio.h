#ifndef __RES_AUDIO_H
#define __RES_AUDIO_H

#include "../res.h"


using namespace std;



class Audio : public Res
{
public:
	void *data;
	char *fileName;
	int format;
	int size;
	int frequency;
	bool loop;
};

#endif

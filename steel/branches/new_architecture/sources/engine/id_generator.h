#ifndef ENGINE__ID_GENERATOR_H
#define ENGINE__ID_GENERATOR_H

typedef unsigned int uid;// уникальный идентификатор модели

class IdGenerator
{
protected:
	uid freeId;
public:
	IdGenerator(void) { freeId = 1; }
	uid genUid(void);
};

extern IdGenerator objectIdGenerator;

#endif

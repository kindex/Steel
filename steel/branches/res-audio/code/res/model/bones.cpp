/***************************************************************************
 *            bones.cpp
 *
 *  Thu Feb  2 22:57:53 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#include "bones.h"
#include "../../common/logger.h"

//	набор костей
boneSet::boneSet( unsigned int initialSize )
{
	bonesCnt=initialSize;
	bones=(boneNode*)malloc(sizeof(boneNode)*bonesCnt);
	if ( bones==NULL )
	{
		steel::log.out("boneSet::boneSet: failed to allocate a memory!\n");
		bonesCnt=0;
	}
}

boneSet::~boneSet( void )
{
	free(bones);
	bonesCnt=0;
}

boneNode *boneSet::operator [] ( unsigned int i )
{
	if ( i>=bonesCnt )
	{
		steel::log.out("boneSet::operator[]: requested value is out of range!\n");
		return NULL;
	}
	return bones+i;
}

//	анимация
boneAnim::boneAnim( unsigned int initialSize )
{
	animsCnt=initialSize;
	anims=(boneANode*)malloc(sizeof(boneANode)*animsCnt);
	if ( anims==NULL )
	{
		steel::log.out("boneAnim::boneAnim: failed to allocate a memory!\n");
		animsCnt=0;
	}
}

boneAnim::~boneAnim( void )
{
	free(anims);
	animsCnt=0;
}

boneANode *boneAnim::operator [] ( unsigned int i )
{
	if ( i>=animsCnt )
	{
		steel::log.out("boneAnim::operator[]: requested value is out of range!\n");
		return NULL;
	}
	return anims+i;
}

//	главный класс
Bones::Bones( void )
{
	flags=BFL_NONE;
}

Bones::~Bones( void )
{
	if ( (flags&BFL_LOADED)==BFL_LOADED )
		unload();
}

bool Bones::load( const std::string file )
{
	if ( (flags&BFL_LOADED)==BFL_LOADED )
	{
//		lastError=SE_LOADED;
		return false;
	}
	
	flags|=BFL_LOADED;
	bones=NULL;
	animation=NULL;
	curFrame=0;
	curTime=0;
	
	return true;
}

bool Bones::unload( void )
{
	if ( (flags&BFL_LOADED)!=BFL_LOADED )
	{
//		lastError=SE_NOT_LOADED;
		return false;
	}
	free(bones_static);
	free(animation_static);
	flags&=~BFL_LOADED;
	return true;
}

bool Bones::resetAnimation( unsigned int animNum )
{
	if ( (flags&BFL_LOADED)!=BFL_LOADED )
	{
//		lastError=SE_NOT_LOADED;
		return false;
	}
	return true;
}

bool Bones::frame( steelTimeM_t ftime )
{
	if ( (flags&BFL_LOADED)!=BFL_LOADED )
	{
//		lastError=SE_NOT_LOADED;
		return false;
	}
	return true;
}

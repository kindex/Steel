/***************************************************************************
 *            bones.cpp
 *
 *  Thu Feb  2 22:57:53 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#include "bones.h"
#include "../../common/logger.h"
#include "bones_engines/simple.h"

using namespace boneSpace;

//	набор костей
boneSet::boneSet( unsigned int initialSize )
{
	bonesCnt=initialSize;
	bones=(boneNode*)malloc(sizeof(boneNode)*bonesCnt);
	if ( bones==NULL )
	{
		alog.out("boneSet::boneSet: failed to allocate a memory!\n");
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
		alog.out("boneSet::operator[]: requested value is out of range!\n");
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
		alog.out("boneAnim::boneAnim: failed to allocate a memory!\n");
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
		alog.out("boneAnim::operator[]: requested value is out of range!\n");
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
		uload();
}

boneErr Bones::load( const std::string file, boneFmt format )
{
	if ( (flags&BFL_LOADED)==BFL_LOADED )
		return BE_LOADED;
	
	flags|=BFL_LOADED;
	bones=NULL;
	animation=NULL;
	curFrame=0;
	curTime=0;
	
	switch ( format )
	{
		case FMT_SIMPLE:
			return simple_load(file,bones_static,animation_static,totalAnims,totalTime);
		
		default:
			flags&=~BFL_LOADED;
			return BE_INV_FMT;
	}
	
	return BE_NONE;
}

boneErr Bones::uload( void )
{
	if ( (flags&BFL_LOADED)!=BFL_LOADED )
		return BE_NOT_LOADED;
	free(bones_static);
	free(animation_static);
	flags&=~BFL_LOADED;
	return BE_NONE;
}

boneErr Bones::resetAnimation( unsigned int animNum )
{
	if ( (flags&BFL_LOADED)!=BFL_LOADED )
		return BE_NOT_LOADED;
	return BE_NONE;
}

boneErr Bones::frame( steelTimeM_t ftime )
{
	if ( (flags&BFL_LOADED)!=BFL_LOADED )
		return BE_NOT_LOADED;
	return BE_NONE;
}

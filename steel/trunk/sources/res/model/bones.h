/*id*********************************************************
	File: res/model/bones.h
	Unit: res/model
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* Kane [J. Anton, kane@mail.berlios.de]
	License:
		Steel Engine License
	Description:
		TODO
 ************************************************************/

#ifndef __BONES_H__
#define __BONES_H__

#include "../../steel.h"
#include <string>
#include "../../common/types.h"
#include "model.h"

// TODO: change types
/*
	Промежуток времени (миллисекунды)
*/
typedef
unsigned int steelTimeM_t;
/*
	Кадры
*/
typedef
unsigned int steelFrame_t;
/*
	Поле флагов (32 бита)
*/
typedef
unsigned int steelFlag32_t;

/*
	Угол (0 = 0 градусов, 2^32-1 -> 360-0 градусам)
*/ 
typedef
unsigned long int steelAngle_t;


/*
	Форматы переходов в анимации
*/
enum boneAFmt
{
	FMT_POSITION,	//	установка положения кости
	FMT_SPEED,		//	установка скорости
	FMT_ACCEL,		//	установка ускорения
	FMT_CIRCLE		//	повтор анимации заного
};

/*
	Флаги:
		NONE:		не установлен ни один флаг
		LOADED:		файл загружен
*/
const steelFlag32_t BFL_NONE=0;
const steelFlag32_t BFL_LOADED=1;

/*
	Одна кость,
		представлена в виде вектора с началом в начале системы
	координат,
		используется сферическая система координат
*/
struct boneNode
{
	//	родительская кость (номер в наборе)
	unsigned int parent;
	//	угол кости относительно oX в XoY (правая ориентация)
	steelAngle_t angle;
	//	угол между костью и XoY  (направление oZ - положительное)
	steelAngle_t altitude;
	//	угол поворота кости вокруг своей оси (правая ориентация)
	steelAngle_t rotation;
	//	длина кости
	steelLength_t len;
};

/*
	Набор костей,
		номера в наборе начанаются с 0
*/
class boneSet
{
public:
	/*
		Создать набор,
		удалить набор,
			initialSize - размер набора
	*/
	boneSet( unsigned int initialSize );
	~boneSet( void );
	
	/*
		Получить кость,
			i - номер кости в наборе
	*/
	boneNode *operator [] ( unsigned int i );
	
private:
	//	количество костей
	unsigned int bonesCnt;
	//	кости
	boneNode* bones;
};

/*
	Квант анимации
*/
struct boneANode
{
	//	время кванта
	steelTimeM_t time;
	//	формат кванта
	boneAFmt format;
	//	кость, которая анимируется (номер в наборе)
	unsigned int bone;
	//	углы изменений (angle, altitude, rotation)
	steelAngle_t angles[3];
};

/*
	Анимация,
		нумерация квантов начанается с 0
*/
class boneAnim
{
public:
	/*
		Создать анимацию,
		удалить анимацию,
			initialSize - количество квантов
	*/
	boneAnim( unsigned int initialSize );
	~boneAnim( void );
	
	/*
		Получить квант,
			i - его номер в анимации
	*/
	boneANode *operator [] ( unsigned int i );
	
private:
	//	количество квантов
	unsigned int animsCnt;
	//	кванты анимации
	boneANode* anims;
};

/*
	Модель скелетной анимации
*/
class Bones: public Model
{
public:
	Bones( void );
	virtual ~Bones( void );
	
	/*
		Загрузка и выгрузка файла,
			file - имя файла
			format - формат файла
	*/
	virtual bool load( const std::string file );
	virtual bool unload( void );
	
	/*
		Сброс анимации,
			после этого вызова изменяются параметры
		родительского класса Model
			animNum - номер анимации после сброса
	*/
	bool resetAnimation( unsigned int animNum );
	
	/*
		Смена кадра,
			после этого вызова изменяются параметры
		родительского класса Model
			ftime - время запрашиваемого кадра
	*/
	bool frame( steelTimeM_t ftime );
	
protected:
	//	флаги
	steelFlag32_t flags;
	
	//	номер текущего кадра
	steelFrame_t curFrame;
	
	//	общее время анимации
	steelTimeM_t totalTime;
	//	тукущее время
	steelTimeM_t curTime;
	
	//	количество анимаций
	unsigned int totalAnims;
	
	//	кости и анимация
	boneSet *bones_static;
	boneSet *bones;
	boneAnim *animation_static;
	boneAnim *animation;
};

#endif

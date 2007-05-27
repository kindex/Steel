/*id*******************************************************************************
	File: res/res.h
	Unit: res
	Part of: Steel engine
	(C) DiVision, 2003-2007
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Молуль для хранения, загрузки игровых ресурсов и контроля над ними. 
	Parts:
		res.cpp
		В поддиректориях хранятся классы для загрузки и хранения всех типов ресурсов.
**************************************************************************************/

#ifndef __RES_H
#define __RES_H

#include "../steel.h"
#include "../engine/id_generator.h"

/*
	Класс - храниель и рагрузчик одного ресурса.
	Хранение - стандартизовано для каждого типа ресурса. Загруска - для каждого типа может быть несколько классов для загрузки.

	От этого класса наследуются классы для хранения ресурсов для каждого типа (image, model).
	В них реализуется деструктор, который уничтожает структуры, созданные при загрузке.
	От них наследуются загрузчики, которые переопределяют метод init.
	init загружает ресурс или генерирует по строковому идентификатору.
*/
class Res
{
public:
	uid	getId() { return resId; }
	void setId(uid id) { resId = id; }
	virtual ~Res() {}

protected:
	uid resId; // уникальный идентификатор
};

void addDataSearchDirectories(const std::string& newDataDir); // in rstream.cpp

#endif

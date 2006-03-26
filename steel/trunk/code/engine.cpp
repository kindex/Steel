/*id*********************************************************
    Unit: core
    Part of: Steel engine
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Kane [Anton]
		* Bond [Andrey Bondarenko]
    Licence:
        Только для Division
    Description:
		Класс Interface - прототип класса, от которого должны наследоваться 
		классы, которые должны обрабатываться классом Engine.
		Класс Engine (движок) через функцию inject() получает объекты, которые
		должны быть обработаны, но обрабатывает их только внутри process().
		Для этого объекты должны поддерживать интервейс (interface) обмена 
		информацией с движком.
 ************************************************************/

#include "engine.h"


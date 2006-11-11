
#ifndef OBJECTS__LIGHT_H
#define OBJECTS__LIGHT_H

/*
Источник освещения.
*/

class GameLight: public GameObj
{
protected:
	Lights *lights;
public:
	GameLight(void)
	{
		lights = new Lights(1);
		lights->at(0).position.set(0.0f, 0.0f, 0.0f);
		lights->at(0).color.set(1.0f, 1.0f, 1.0f, 1.0f);
		lights->at(0).distance = 3;
		lights->at(0).intensivity = 1;
	}
	
	~GameLight(void)
	{
		delete lights;
	}

	Lights* getLights(void)
	{
		return lights;
	}

};

#endif

#ifndef ENGINE__INTERFACE_H
#define ENGINE__INTERFACE_H

typedef int InterfaceId;

class GameObject;

class Interface
{

};

typedef
void (GameObject::*Callback)(Interface*);

#endif

#include "Component.h"
Component::Component()
{

}

Component::Component(Entity& e) :
	entity(&e)
{
}

bool Component::init()
{
	return true;
}

void Component::OnRender()
{

}

void Component::OnUpdate(double deltaTime)
{

}

void Component::OnImGuiRender()
{

}

void Component::OnExit()
{

}

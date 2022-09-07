#include "Queue.h"

Queue::Queue(unsigned short int _size)
{
	size_=_size;
	elements=new float[size_];
	for(int i = 0; i < size_; i ++)
		elements[i]=0;
}

Queue::Queue(unsigned short int _size, float _el)
{
	size_=_size;
	elements=new float[size_];
	for(int i = 0; i < size_; i ++)
		elements[i]=_el;
}

Queue::~Queue()
{
	//free(elements);
}

void Queue::push(float _el)
{
	for(int i = 0; i < size_-1; i ++)
		elements[i+1]=elements[i];
	elements[0]=_el;
}

float Queue::pop()
{
	return elements[size_-1];
}

unsigned short int Queue::get_size()
{
	return size_;
}

float* get_elements()
{
	return elements;
}

void set_elements(float* _elements)
{
	elements=_elements;
}

bool Queue::operator*(Queue _queue)
{
	if(size_!=_queue.get_size())
		return false;
	float result=0;
	for(int i = 0; i<size_;i++)
	{
		result+=elements[i]*_queue.get_elements()[i];
	}
	return result;
}

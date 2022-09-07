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

Queue::Queue(unsigned short int _size, float* _elements)
{
  size_=_size;
  elements=new float[size_];
  for(int i = 0; i < size_; i ++)
    elements[i]=_elements[i];
}

void Queue::initialize(unsigned short int _size)
{
  size_=_size;
  elements=new float[size_];
  for(int i = 0; i < size_; i ++)
    elements[i]=0;
}

void Queue::initialize(unsigned short int _size, float _el)
{
  size_=_size;
  elements=new float[size_];
  for(int i = 0; i < size_; i ++)
    elements[i]=_el;
}

void Queue::push(float _el)
{
	for(int i = size_-1; i >= 0 ; i --)
		elements[i+1]=elements[i];
	elements[0]=_el;
}

float Queue::pop()
{
	return elements[size_-1];
}

unsigned short int Queue::get_size() const
{
	return size_;
}

float* Queue::get_elements() const
{
	return elements;
}

void Queue::set_elements(float* _elements)
{
	for(int i = 0; i < size_; i++)
		elements[i]=_elements[i];
}

float Queue::operator*(Queue _queue)
{
	if(size_!=_queue.get_size())
		return false;
	float result=0;
	for(int i = 0; i<size_;i++)
	{
		result+=elements[i]*_queue[i];
	}
	return result;
}

float Queue::operator[](unsigned int _index)
{
	return elements[_index];
}

void Queue::print_()
{
  for(int i = 0; i<size_;i++)
  {
      Serial.println(elements[i]);  
  }
}

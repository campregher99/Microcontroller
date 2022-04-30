#include "Queue.h"

Queue::Queue(){}


Queue::Queue(unsigned int _length)
{
    elements=new T[_length];
    length=_length;
}


T Queue::push(T _element)
{
    for(int i = 0; i < length - 1; i++)
    {
        elements[i + 1] = elements[i];
    }
    elements[0] = _element;
    last_element++;
    if(last_element > length - 1)
    {
        last_element = length - 1;
    }
    return _element;
}


T Queue::pop(void)
{
    last_element --;
    return elements[last_element + 1];
}


T Queue::operator[](unsigned int _index)
{
    return elements[_index];
}
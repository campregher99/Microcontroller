#include "Queue.h"

Queue::Queue(unsigned short int _size)
{
  size_ = _size;
  elements = new float[size_];
  for (int i = 0; i < size_; i ++)
    elements[i] = 0;
}

Queue::Queue(unsigned short int _size, float _el)
{
  size_ = _size;
  elements = new float[size_];
  for (int i = 0; i < size_; i ++)
    elements[i] = _el;
}

Queue::Queue(unsigned short int _size, float* _elements)
{
  size_ = _size;
  elements = new float[size_];
  for (int i = 0; i < size_; i ++)
    elements[i] = _elements[i];
}

void Queue::initialize(unsigned short int _size)
{
  size_ = _size;
  elements = new float[size_];
  for (int i = 0; i < size_; i ++)
    elements[i] = 0;
}

void Queue::initialize(unsigned short int _size, float _el)
{
  size_ = _size;
  elements = new float[size_];
  for (int i = 0; i < size_; i ++)
    elements[i] = _el;
}

void Queue::push(float _el)
{
  for (int i = size_ - 1; i >= 0 ; i --)
    elements[i + 1] = elements[i];
  elements[0] = _el;
}

float Queue::pop()
{
  return elements[size_ - 1];
}

void Queue::clear_()
{
  for (int i = 0; i < size_; i ++)
    elements[i] = 0;
}

void Queue::clear_(float _el)
{
  for (int i = 0; i < size_; i ++)
    elements[i] = _el;
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
  for (int i = 0; i < size_; i++)
    elements[i] = _elements[i];
}

float Queue::mean()
{
  float mean = 0;
  for (int i = 0; i < size_; i ++)
  {
    mean += elements[i];
  }
  return mean / size_;
}

float Queue::max_()
{
  float max__ = elements[0];
  for (int i = 1; i < size_; i ++)
  {
    if (max__ < elements[i])
    {
      max__ = elements[i];
    }
  }
  return max__;
}

float Queue::min_()
{
  float min__ = elements[0];
  for (int i = 1; i < size_; i ++)
  {
    if (min__ > elements[i])
    {
      min__ = elements[i];
    }
  }
  return min__;
}

float Queue::iae()
{
  float min__ = min_(), iae_ = 0;
  for (int i = 0; i < size_; i++)
  {
    iae_ += elements[i] - min__;
  }
  return iae_;
}

float Queue::std()
{
  float std_=0, mean_=mean();
  for (int i = 0; i < size_; i++)
  {
    std_ += pow(elements[i] - mean_,2);
  }
  return sqrt(std_/size_);
}

float Queue::operator*(Queue _queue)
{
  if (size_ != _queue.get_size())
    return false;
  float result = 0;
  for (int i = 0; i < size_; i++)
  {
    result += elements[i] * _queue[i];
  }
  return result;
}

float Queue::operator[](unsigned int _index)
{
  return elements[_index];
}

void Queue::print_()
{
  for (int i = 0; i < size_; i++)
  {
    Serial.println(elements[i]);
  }
}

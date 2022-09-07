
#ifndef QUEUE_H
#define QUEUE_H 


class Queue
{
  unsigned short int size_;
  float* elements;
public:
  Queue(unsigned short int _size);
  Queue(unsigned short int _size, float _el);
  ~Queue();
  void push(float _el);
  float pop();
  unsigned short int get_size();
  float* get_elements();
  void set_elements(float* _elements);
  bool operator*(Queue _queue);
};

#endif

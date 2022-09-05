
#ifndef QUEUE_H
#define QUEUE_H 

struct Queues
{
    unsigned short int size_;
    int indexPop;
    int indexPush;
    int numberOfElements;
    float* elements;
};

bool pushQueue(Queues* _queue, float* _ptr);
bool queueInitializer(Queues* _queue, unsigned short int _size);

bool pushQueue(Queues* _queue, float _numb)
{
  for(int i = 0; i < _queue->size_-1; i++)
  {
    _queue->elements[_queue->size_-i]=_queue->elements[_queue->size_-i-1];
  }
  _queue->elements[0]=_numb;
  return true;
}

inline bool queueInitializer(Queues* _queue, unsigned short int _size)
{
  _queue->size_=_size;
  _queue->indexPush=0;
  _queue->indexPop=0;
  _queue->numberOfElements=0;
  _queue->elements= new float[_size];
  return true;
}

#endif

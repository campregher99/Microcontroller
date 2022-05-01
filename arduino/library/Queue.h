
#ifndef QUEUE_H
#define QUEUE_H 

template<typename T>
class Queue
{
    T* elements;
    unsigned int length;
    unsigned int last_element{0};
public:
    Queue();
    Queue(unsigned int _length);
    ~Queue();
    T push(T _element);
    T pop(void);
    T operator[](unsigned int _index);

};

#endif


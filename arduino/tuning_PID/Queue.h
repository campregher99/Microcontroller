#ifndef QUEUES_H
#define QUEUES_H

#include "Arduino.h"
class Queue
{
    unsigned short int size_;
    float* elements;
  public:
    //constructors
    Queue() {};
    Queue(unsigned short int _size);
    Queue(unsigned short int _size, float _el);
    Queue(unsigned short int _size, float* _elements);
    Queue (const Queue& _queue): size_{_queue.get_size()}, elements {_queue.get_elements()} {}; //coping

    //initializer for void constructor
    void initialize(unsigned short int _size);
    void initialize(unsigned short int _size, float _el);

    void push(float _el);   //shift all the element and insert the new one
    float pop();            //return the last elements
    void clear_();          //make 0 all the elements
    void clear_(float _el);          //make equal to _el all the elements
    unsigned short int get_size() const;  //return the queue size
    float* get_elements() const;          //return the pointer to the elements
    void set_elements(float* _elements);  //permit to fill all the elements at once
    float mean();   //return the mean value
    float max_();    //return the max value
    float min_();    //return the min value
    float iae();      //return the sum of the absolute error respect the mean
    float std();    //return the standard deviation at 68% (*2 => 95% ; *3 => 99%)

    //overload operator
    float operator*(Queue _queue);
    float operator[](unsigned int _index);

    void print_();    //print the elements on Serial
};

#endif

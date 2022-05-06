



template<typename T> class Queue
{
    T* elements;
    unsigned int length;
    unsigned int last_element{0};
public:
    Queue(){};
    Queue(unsigned int _length);
    void set(unsigned int _length);
    T push(T _element);
    T pop(void);
    T operator[](unsigned int _index);

};

template<typename T>
Queue<T>::Queue(unsigned int _length)
{
    elements=new T[_length];
    length=_length;
}

template<typename T>
void Queue<T>::set(unsigned int _length)
{
    elements=new T[_length];
    length=_length;
}

template<typename T>
T Queue<T>::push(T _element)
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

template<typename T>
T Queue<T>::pop(void)
{
    last_element --;
    return elements[last_element + 1];
}

template<typename T>
T Queue<T>::operator[](unsigned int _index)
{
    return elements[_index];
}

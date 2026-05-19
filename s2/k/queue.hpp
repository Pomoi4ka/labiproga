#ifndef QUEUE_HPP_
#define QUEUE_HPP_

template <typename T, size_t limit>
class Queue: List<T> {
    size_t length;
public:
    Queue();

    void enqueue(T &); // calls transfer_from
    void dequeue(T &); // calls transfer_from
    T &lastOrEnqueueDefault();
};

template <typename T, size_t limit>
Queue<T, limit>::Queue()
    : List<T>()
    , length()
{}

template <typename T, size_t limit>
void Queue<T, limit>::enqueue(T &a)
{
    if (length == limit) {
        T ignore;
        List<T>::popLeft(ignore);
    } else length += 1;
    List<T>::append()->transfer_from(a);
}

template <typename T, size_t limit>
void Queue<T, limit>::dequeue(T &a)
{
    if (!length) return;
    length -= 1;
    List<T>::popLeft(a);
}

template <typename T, size_t limit>
T &Queue<T, limit>::lastOrEnqueueDefault()
{
    T *tail = List<T>::tail();
    if (!tail) {
        T def;
        enqueue(def);
        tail = List<T>::tail();
    }
    return *tail;
}


#endif // QUEUE_HPP_

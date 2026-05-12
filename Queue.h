        // Ziad Waleed Sallam 320250274
#ifndef QUEUE_h
#define QUEUE_h
template <typename T>
struct Node
{
    T data;
    Node *next;
    Node(T x)
    {
        data = x;
        next = nullptr;
    }
};
template <typename T>
class Queue
{
private:
    Node<T> *Front;
    Node<T> *Rear;
    int Size;

public:
    Queue()
    {
        Front = nullptr;
        Rear = nullptr;
        Size = 0;
    }
    Queue(const Queue &x)
    {
        Front = nullptr;
        Rear = nullptr;
        Size = 0;
        if (x.empty())
        {
            return;
        }
        Node<T> *ptr = x.Front;
        while (ptr != nullptr)
        {
            enqueue(ptr->data);
            ptr = ptr->next;
        }
    }
    bool empty() const
    {
        if (Size == 0)
        {
            return 1;
        }
        return 0;
    }
    int size() const
    {
        return Size;
    }
    void enqueue(T x)
    {
        Node<T> *Q = new Node<T>(x);

        if (!empty())
        {
            Rear->next = Q;
        }
        Rear = Q;
        if (Size == 0)
        {
            Front = Q;
        }
        Size++;
    }
    void dequeue()
    {
        if (empty())
        {
            return;
        }
        Node<T> *Q = Front->next;
        delete Front;
        Front = Q;
        Size--;
        if (empty())
        {
            Rear = nullptr;
        }
    }
    T front() const
    {
        if (!empty())
        {
            return Front->data;
        }
        return T();
    }
    Queue &operator=(const Queue &x)
    {
        if (this == &x)
        {
            return *this;
        }
        while (!empty())
        {
            dequeue();
        }
        if (x.empty())
        {
            return *this;
        }
        Node<T> *ptr = x.Front;
        while (ptr != nullptr)
        {
            enqueue(ptr->data);
            ptr = ptr->next;
        }
        return *this;
    }
    ~Queue()
    {
        while (!empty())
        {
            dequeue();
        }
    }
};

#endif
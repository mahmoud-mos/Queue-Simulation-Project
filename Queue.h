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

        // queue is empty
        if (empty())
        {
            Front = Q;
            Rear = Q;
            Size++;
            return;
        }

        // customer is a VIP
        if (x.getIsVip())
        {
            // the first person in line is a regular customer > VIP cuts to the front.
            if (!Front->data.getIsVip())
            {
                Q->next = Front;
                Front = Q;
            }
            else
            {
                // find last VIP node to insert after it.
                Node<T> *curr = Front;
                while (curr->next != nullptr && curr->next->data.getIsVip())
                {
                    curr = curr->next;
                }

                // insert the new VIP node right after 'curr'
                Q->next = curr->next;
                curr->next = Q;

                // inserted at the very end, update Rear
                if (Q->next == nullptr)
                {
                    Rear = Q;
                }
            }
        }
        // standard customer appends normally
        else
        {
            Rear->next = Q;
            Rear = Q;
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
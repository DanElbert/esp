#ifndef RINGARRAY_H
#define RINGARRAY_H

class RingArray {
  public:
    RingArray(int size);
    RingArray(const RingArray& other);
    ~RingArray();
    void add(int value);
    int getValue(int idx);
    int sum();
    int average();
    int count();
  private:
    int* _values;
    int _size;
    int _count;
    int _head;
    int _tail;

    void incrementIndex(int &idx);
};

#endif

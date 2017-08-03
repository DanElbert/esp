#include "RingArray.h"

RingArray::RingArray(int size) {
  _head = -1;
  _tail = 0;
  _count = 0;
  _size = size;
  _values = new int[size];
}

RingArray::RingArray(const RingArray& other) {
  _head = other._head;
  _tail = other._tail;
  _count = other._count;
  _size = other._size;
  _values = new int[_size];
  for (int x = 0; x < _size; x++) {
    _values[x] = other._values[x];
  }
}

RingArray::~RingArray() {
  delete [] _values;
}

void RingArray::add(int value) {
  if (_count < _size) {
    incrementIndex(_head);
    _values[_head] = value;
    _count++;
  } else {
    incrementIndex(_head);
    incrementIndex(_tail);
    _values[_head] = value;
  }
}

int RingArray::getValue(int idx) {
  idx = (_tail + idx) % _size;
  return _values[idx];
}

int RingArray::count() {
  return _count;
}

int RingArray::sum() {
  int s = 0;
  for (int x = 0; x < _count; x++) {
    s += _values[x];
  }
  return s;
}

int RingArray::average() {
  return sum() / _count;
}

void RingArray::incrementIndex(int &idx) {
  idx = (idx + 1) % _size;
}

#ifndef CUSTOMCONTAINERS_H
#define CUSTOMCONTAINERS_H

#include <stdint.h>

using namespace std;

namespace custom{

/*class RuntimeObject{

public:
//    virtual static string getObjectTypeName() = 0;
    virtual static uint64_t getObjectTypeId() = 0;


};



template<typename T,typename U>
T* runtime_cast(U* u)
{
    if(static_cast<RuntimeObject*>(u)->getObjectTypeId())
    {

    }
}*/


/*template<typename Base,typename Derived>
class StaticInheritor : Base
{
    template<typename... Args>
    Inherit(Args&&... args) :
        Base(static_cast<Args&&>(args)...) {}

    Base returnBase()
    {
        return static_cast<Base>(*this);
    }

};*/


class AbstractException
{
public:
    virtual string getMessage() = 0;
};

class OutOfBoundsException : public AbstractException
{
public:
    OutOfBoundsException() noexcept
    {

    }
    string getMessage() override
    {
        return "Index is out of bounds \n";
    }
};

class InitializationException : public AbstractException
{
public:
    InitializationException() noexcept
    {

    }
    string getMessage() override
    {
        return "Failed to initialize Class \n";
    }

};

template<typename T> class shared_ptr;
template<typename T> class LinkedList;

template <typename T> class unique_ptr{

    T* _value;
public:
    unique_ptr<T>(T& value)
    {
        _value = new T(value);
    }
    unique_ptr<T>(T value)
    {
        _value = new T(value);
    }
    unique_ptr<T>(unique_ptr<T>& ptr)
    {
        _value = ptr._value;
        ptr._value = nullptr;
    }
    /*unique_ptr<T>(T* ptr)
    {
        _value = ptr;
    }*/
    T getValue()
    {
        return *_value;
    }
    T operator *()
    {
        return *_value;
    }
    ~unique_ptr()
    {
        delete _value;
    }
};

template<typename T> class _SharedResource{
T* _value;
int _count;
private:
    _SharedResource()
    {
        _value = new T;
    }
    _SharedResource(T value)
    {
        _value = new T(value);
    }
    void decrement()
    {
        _count--;
        if(_count == 0)
            delete _value;
    }
    void increment()
    {
        _count++;
    }
public:
    ~_SharedResource()
    {
        delete _value;
    }
    friend class shared_ptr<T>;
};

template<typename T> class shared_ptr  {
    _SharedResource<T>* _root = nullptr;
    shared_ptr(_SharedResource<T>* root)
    {
        this->_root = root;
        root->increment();
    }
public:
    shared_ptr(const shared_ptr<T>& reference)
    {
        _root = reference._root;
        _root->increment();
    }
    static shared_ptr<T> createSharedResource()
    {
        _SharedResource<T>* s = new _SharedResource<T>();
        shared_ptr<T> sharedptr(s);
        return sharedptr;
    }
    static shared_ptr<T> createSharedResource(T value)
    {
        _SharedResource<T>* s = new _SharedResource<T>(value);
        shared_ptr<T> sharedptr(s);
        return sharedptr;
    }
    T operator *()
    {
        return *(_root->_value);
    }
    ~shared_ptr()
    {
        _root->decrement();
        if(_root->_count == 0)
            delete _root;
    }
};

template<typename T> class abstract_iterator{
public:
    virtual T next() = 0;
    virtual bool isEnd() = 0;
    virtual T valueAt() = 0;
};

template<typename T> class abstract_bidirectional_iterator : public abstract_iterator<T>
{
public:
    virtual T previous() = 0;
    virtual bool isBeginning() = 0;
};

template<typename T> class abstract_randomaccess_iterator : public abstract_bidirectional_iterator<T>
{
public:
    virtual int indexAt() = 0;
    virtual T goTo(int index) = 0;
    virtual T next(int offset) = 0;
    virtual T previous(int offset) = 0;
};

template<typename T,int Size> class Array
{
    T values[Size];
public:
    class iterator: public abstract_randomaccess_iterator<T>{
        int _index;
        int _size;
        T* _listPtr;

    public:
        iterator(Array<T,Size> *ptr,bool isEnd = false)
        {
            if(!isEnd)
            {
                _index = 0;
                _size = ptr->getSize();
                _listPtr = ptr->values;
            }
            else
            {
                _index = ptr->getSize() - 1;
                _size = ptr->getSize();
                _listPtr = &(ptr->values[_index]);
            }
        }

        T next() override
        {
            if(!isEnd())
            {
                _index++;
            }
            else
            {
                return nullptr;
            }
        }
        T next(int offset) override
        {
        }
        bool isEnd() override
        {
            if(_index == (_listPtr->getSize() - 1))
               return true;
            else
                return false;
        }
        T valueAt() override
        {
        }
        T previous()
        {
            if(isBeginning())
            {
                _index--;
            }
            else
                return nullptr;
        }
        T previous(int offset)
        {
            if(_index - offset >= 0)
            {
                _index = _index - offset;
                return _listPtr->valueAt(_index);
            }
            else
                return nullptr;
        }
        bool isBeginning()
        {

            if(_index == 0)
               return true;
            else
                return false;
        }
        int indexAt()
        {
            return _index;
        }
        T goTo(int index)
        {
            _index = index;
            return _listPtr->at(index);
        }
    };
    constexpr int getSize() { return Size;}
    int& operator [](int index)
    {
        return values[index];
    }
};

template<typename T> class _LinkedElement{
    T value;
    _LinkedElement<T>* _previous = nullptr;
    _LinkedElement<T>* _next = nullptr;
    _LinkedElement(T value){
        this->value = value;
    }
    _LinkedElement(){
        if(_next != nullptr)
            delete _next;
    }
    friend class LinkedList<T>;
    friend class LinkedList<T>::bidirectional_iterator;
};

template<typename T> class LinkedList{
public:
  class iterator: public abstract_bidirectional_iterator<T>{
      //LinkedList<T>* _current;
      _LinkedElement<T>* _current;
  public:
      iterator(LinkedList<T> &ptr,bool isEnd = false)//:_current(&ptr)
      {
         isEnd ? _current = ptr._end : _current = ptr._begin;
      }
      T next() override
      {
          if(_current->_next == nullptr)
              return nullptr;
            _current = _current->_next;
          return _current->T;
      }
      T previous() override
      {
          if(_current->_previous == nullptr)
              return nullptr;
            _current = _current->_previous;
          return _current->T;
      }
      bool isEnd() override
      {
            if(_current->_next == nullptr)
                return true;
            return false;
      }
      bool isBeginning() override
      {
          if(_current->_previous == nullptr)
              return true;
          return false;
      }

      T valueAt() override
      {
          return _current->T;
      }
  };
  LinkedList(){
      _begin = new _LinkedElement<T>();
      _end = _begin;
  }
  LinkedList(T value){
      _begin = new _LinkedElement<T>(value);
      _end = _begin;
  }
  LinkedList<T> begin()
  {
      return *_begin;
  }
  LinkedList<T> end()
  {
      return *_end;
  }
  void pushInFront(T value)
  {
      _LinkedElement<T>* addedElement = new _LinkedElement<T>(value);
      addedElement->_previous = nullptr;
      addedElement->_next = _begin;
      _begin = addedElement;
  }
  void pushBack(T value)
  {
      _LinkedElement<T>* addedElement = new _LinkedElement<T>(value);
      addedElement->_previous = _end;
      addedElement->_next = nullptr;
      _end = addedElement;
  }
  void insertAhead(LinkedList<T>::iterator iterator)
  {
      _LinkedElement<T>* addedElement = new _LinkedElement<T>(iterator.valueAt());
      addedElement->_previous = iterator._current;
      addedElement->_next = iterator._current->_next;

      iterator._current->_next->_previous = addedElement;
      iterator._current->_next = addedElement;
  }
  void insertBehind(LinkedList<T>::iterator iterator)
  {
      _LinkedElement<T>* addedElement = new _LinkedElement<T>(iterator.valueAt());
      addedElement->_previous = iterator._current->_previous;
      addedElement->_next = iterator._current;

      iterator._current->_previous->_next = addedElement;
      iterator._current->_previous = addedElement;
  }
  ~LinkedList(){
      delete _begin;
  }
private:
  _LinkedElement<T>* _begin;
  _LinkedElement<T>* _end;

};


template<typename T> class ContiguousList{
public:
    class iterator: public abstract_randomaccess_iterator<T>{
        int _index;
        ContiguousList<T>* _listPtr;

    public:
        iterator(ContiguousList<T> *ptr,bool isEnd = false)
        {
            _listPtr = ptr;
            isEnd ? _index = _listPtr->_usedSize - 1 : _index = 0;
        }

        T next() override
        {
            if(!isEnd())
            {
                _index++;
                return _listPtr->valueAt(_index);
            }
            else
            {
                return nullptr;
            }
        }
        T next(int offset) override
        {
            if(offset < _usedSize - _index)
            {
                _index = _index + offset;
                return _listPtr->valueAt(_index);
            }
            else
                return nullptr;
        }
        bool isEnd() override
        {
            if(_index == (_listPtr->_usedSize - 1))
               return true;
            else
                return false;
        }
        T valueAt() override
        {
            return _listPtr->valueAt(_index);
        }
        T previous()
        {
            if(isBeginning())
            {
                _index--;
                return _listPtr->valueAt(_index);
            }
            else
                return nullptr;
        }
        T previous(int offset)
        {
            if(_index - offset >= 0)
            {
                _index = _index - offset;
                return _listPtr->valueAt(_index);
            }
            else
                return nullptr;
        }
        bool isBeginning()
        {

            if(_index == 0)
               return true;
            else
                return false;
        }
        int indexAt()
        {
            return _index;
        }
        T goTo(int index)
        {
            _index = index;
            return _listPtr->valueAt(index);
        }
    };
    ContiguousList()
    {
        _begin = new T[10];
    }
    ContiguousList(int size)
    {
        _begin = new T[size];
        _allocatedMaxSize = size;
    }
    T operator[](int index)
    {
        if(_usedSize > index)
            return _begin[index];
        else
            return nullptr;
    }
    T valueAt(int index)
    {
        if(_usedSize > index)
            return _begin[index];
        else
            return nullptr;
    }
    T* resize(int newSize)
    {
        T* newBegin = new T[newSize];
        for(int i = 0;i < _usedSize && i < newSize;i++)
            newBegin[i] = _begin[i];
        delete _begin;
        _begin = newBegin;
        if(_usedSize > newSize)
        {
            _usedSize = newSize;
            _allocatedMaxSize = newSize;
        }
        else
        {
            _allocatedMaxSize = newSize;
        }
    }

    int insert(int index,T value)
    {
        T* newBegin;
        if(_usedSize + 1 > _allocatedMaxSize)
            newBegin = new T[_allocatedMaxSize + 10];
        else
            newBegin = new T[_allocatedMaxSize];

        for(int i = 0;i < _usedSize + 1;i++)
        {
            if(i == index)
            {
                newBegin[i] = value;
            }
            else if(i > index)
            {
                newBegin[i + 1] = _begin[i];
            }
            else
            {
                newBegin[i] = _begin[i];
            }
        }
        delete _begin;
        _begin = newBegin;
        return _usedSize;
    }

    int remove(int index)
    {
        T* newBegin;
        if(_usedSize - 1 < _allocatedMaxSize - 20)
            newBegin = new T[_allocatedMaxSize - 10];
        else
            newBegin = new T[_allocatedMaxSize];

        for(int i = 0;i < _usedSize ;i++)
        {
            if(i >= index)
            {
                newBegin[i] = _begin[i + 1];
            }
            else
            {
                newBegin[i] = _begin[i];
            }
        }
        delete _begin;
        _begin = newBegin;
        return _usedSize;
    }

    int pushBack(T value)
    {
        if(_usedSize >= _allocatedMaxSize)
        {
            resize(_allocatedMaxSize + (_allocatedMaxSize / 2 > 10 ? _allocatedMaxSize / 2 : 10));
            _begin[_usedSize - 1] = value;
            _usedSize++;
        }
        else
        {
            _begin[_usedSize - 1] = value;
            _usedSize++;
        }
        return _usedSize - 1;
    }
    int popBack(T value)
    {
        if(_usedSize < _allocatedMaxSize - 20)
        {
            resize(_allocatedMaxSize - 10);
            _begin[_usedSize - 1] = value;
            _usedSize++;
        }
        else
        {
            _begin[_usedSize - 1] = value;
            _usedSize++;
        }
        return _usedSize;
    }
private:
    T* _begin;
    int _allocatedMaxSize = 10;
    int _usedSize = 0;
};


/*template<typename T> struct _Node
{
    int _hash;
    T* _ptr;
    _Node* _left;
    _Node* _right;
    bool _isLeftTraversed;
    bool _isRightTraversed;
};

template<typename T> class HashSet{
    _Node* _rootNode;
    _Node* _deepestNode;
public:
    class iterator:public abstract_iterator<T>
    {
      _Node* _rootNode;
      _Node* _currentNode;
      _Node* _nextNode;
      _Node* _laterNode;
      //int depth;
      public:
        iterator(HashSet<T> *ptr)
        {
            _currentNode = ptr->_rootNode;
            _nextNode = ptr->_rootNode->_left;
            _laterNode = ptr->_rootNode->_right;
        }
      T next() override
      {

      }
      bool isEnd() override
      {

      }
      T at() override
      {

      }
    private:
      bool recurseTraverse()
      {

      }
    };
};

template<typename KeyType,typename ValueType> class HashMap{

};*/

}
#endif // CUSTOMCONTAINERS_H

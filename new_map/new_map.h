#pragma once
#include <string>
#include <algorithm>
#include <iostream>

template<typename Key, typename Value>
class Element;

template<typename Key, typename Value>
class iterator;

template<typename Key, typename Value>
class new_map
{
	size_t m_size;
	size_t m_dOccupiedSize;
	Element<Key,Value>* m_pData;

	bool tryInsert(size_t _bucketNr, Key _key, Value _value);
	void resize();
	size_t m_begin;
	Element<Key,Value>* makeptr(size_t);
	iterator<Key, Value> make_npos();
public:
	new_map();
	new_map(size_t);
	typedef iterator<Key, Value> iterator;
	friend class iterator;
	iterator npos;
	Element<Key,Value>& begin();
	Element<Key, Value>& end() const;
	size_t size() const;
	bool empty() const;
	void clear();
	void insert(Key, Value);
	iterator find(Key);
	void print();
	~new_map();

};

template<typename Key, typename Value>
class iterator 
{
	size_t m_index;
	Element<Key, Value> ptr;
public:
	Key first;
	Value second;
	typedef Value* pointer;
	typedef Value& reference;
	typedef std::random_access_iterator_tag iterator_category;

	iterator(Element<Key, Value> &p);
	pointer operator++()
	{
		return ptr->makeptr(m_index++);
	}
	iterator operator--()
	{
		return ptr->makeptr(m_index--);
	}
	bool operator!=(Element<Key, Value> &);
	bool operator==(Element<Key, Value> &);
	//reference operator*() { return ptr->make(index); }
	/*pointer operator->() { return ptr->makeptr(index); }

	pointer operator+ (const int n) { return ptr->makeptr(index + n); }
	pointer operator- (const int n) { return ptr->makeptr(index - n); }
	pointer operator+=(const int n) { return ptr->makeptr(index += n); }
	pointer operator-=(const int n) { return ptr->makeptr(index -= n); }*/
	//reference operator[](int i) { return ptr->make(i); }
	//operator int() { return index; }
};

template<typename Key, typename Value>
class Element
{
	friend class iterator<Key, Value>;
	friend class new_map<Key,Value>;
	
	Key m_key;
	Value m_value;
	size_t m_index;
	enum {
		NOT_OCCUPIED, OCCUPIED, DELETED, NULLPTR
	} m_status;
	bool operator==(Element&);
	bool operator!=(Element&);
};

template<typename Key, typename Value>
inline bool new_map<Key, Value>::tryInsert(size_t _index, Key _key, Value _value)
{
	if (m_pData[_index].m_status != Element<Key, Value>::OCCUPIED)
	{
		m_pData[_index].m_status = Element<Key, Value>::OCCUPIED;
		m_pData[_index].m_key = _key;
		m_pData[_index].m_value = _value;
		m_pData[_index].m_index = _index;
		m_dOccupiedSize++;
		return true;
	}
	else
		return false;
}

template<typename Key, typename Value>
inline void new_map<Key, Value>::resize()
{
	size_t oldSize = m_size;
	m_size <<= 1;

	Element<Key, Value>* oldData = m_pData;
	m_pData = new Element<Key, Value>[m_size];
	for (int i = 0; i < m_size; i++)
		m_pData[i].m_status = Element<Key, Value>::NOT_OCCUPIED;

	m_dOccupiedSize = 0;

	for (int i = 0; i < oldSize; i++)
		if (oldData[i].m_status == Element<Key, Value>::OCCUPIED) {
			if ((m_dOccupiedSize << 1) >= m_size)
				resize();

			Key hashCode = oldData[i].m_key;
			auto bucketNr = hashCode % m_size;

			for (size_t i = bucketNr; i < m_size; i++)
				if (tryInsert(i, oldData[i].m_key, oldData[i].m_value))
					return;

			for (int i = 0; i < bucketNr; i++)
				if (tryInsert(i, oldData[i].m_key, oldData[i].m_value))
					return;

			insert(oldData[i].m_key, oldData[i].m_value);
		}

	delete[] oldData;
}

template<typename Key, typename Value>
inline Element<Key, Value> * new_map<Key, Value>::makeptr(size_t _index)
{
	if (m_pData[_index].m_status == Element<Key, Value>::OCCUPIED)
		return &m_pData[_index];
}

template<typename Key, typename Value>
inline new_map<Key, Value>::new_map() :
	m_size(100), m_begin(0), m_dOccupiedSize(0), npos(make_npos())
{
	m_pData = new Element<Key,Value>[m_size];

	clear();
}

template<typename Key, typename Value>
inline new_map<Key, Value>::new_map(size_t _size) :
	m_size(_size), m_begin(0), m_dOccupiedSize(0), npos(make_npos)
{
	m_pData = new Element[m_size];

	clear();
}

template<typename Key, typename Value>
inline Element<Key, Value>& new_map<Key, Value>::begin()
{
	while (m_pData[m_begin].m_status != Element<Key,Value>::OCCUPIED && 
		m_dOccupiedSize >= m_begin) m_begin++;

	return m_pData[m_begin];
}

template<typename Key, typename Value>
inline Element<Key, Value>& new_map<Key, Value>::end() const
{
	size_t _index = m_dOccupiedSize;
	while (m_pData[m_dOccupiedSize].m_status != Element<Key, Value>::OCCUPIED &&
		m_dOccupiedSize >= 0) --_index;

	return m_pData[_index - 1];
}

template<typename Key, typename Value>
inline size_t new_map<Key, Value>::size() const
{
	return m_dOccupiedSize;
}

template<typename Key, typename Value>
inline bool new_map<Key, Value>::empty() const
{
	return m_dOccupiedSize;
}

template<typename Key, typename Value>
inline void new_map<Key, Value>::clear()
{
	m_dOccupiedSize = 0;
	for (int i = 0; i < m_size; i++)
		if (m_pData[i].m_status == Element<Key, Value>::OCCUPIED)
			m_pData[i].m_status = Element<Key, Value>::NOT_OCCUPIED;
}

template<typename Key, typename Value>
inline void new_map<Key, Value>::insert(Key _key, Value _value)
{
	if ((m_dOccupiedSize << 1) >= m_size)
		resize();

	size_t bucketNr = _key % m_size;

	for (size_t i = bucketNr; i < m_size; i++)
		if (tryInsert(i, _key, _value))
			return;

	for (int i = 0; i < bucketNr; i++)
		if (tryInsert(i, _key, _value))
			return;
}

template<typename Key, typename Value>
inline iterator<Key, Value> new_map<Key, Value>::find(Key _key)
{
	for (size_t i = 0; i < m_dOccupiedSize; i++)
		if (m_pData[i].m_status == Element<Key, Value>::OCCUPIED)
			if (m_pData[i].m_key == _key)
				return iterator(m_pData[i]);

	return npos;
}

template<typename Key, typename Value>
inline void new_map<Key, Value>::print()
{
	for (size_t i = 0; i < m_dOccupiedSize; i++)
		std::cout << m_pData[i].m_key << " " << m_pData[i].m_value << std::endl;
}

template<typename Key, typename Value>
inline new_map<Key, Value>::~new_map()
{
	delete[] m_pData;
}

template<typename Key, typename Value>
inline iterator<Key, Value>::iterator(Element<Key, Value>& _data)
{
	ptr = _data;
	first = _data.m_key;
	second = _data.m_value;
}

template<typename Key, typename Value>
inline bool iterator<Key, Value>::operator!=(Element<Key, Value>& _other)
{
	return this->ptr != _other;
}

template<typename Key, typename Value>
inline bool iterator<Key, Value>::operator==(Element<Key, Value>& _other)
{
	return this->ptr == _other;
}

template<typename Key, typename Value>
inline bool Element<Key, Value>::operator==(Element &_other)
{
	return this->m_key == _other.m_key && this->m_value == _other.m_value && this->m_status == _other.m_status;
}

template<typename Key, typename Value>
inline bool Element<Key, Value>::operator!=(Element &_other)
{
	return this->m_key != _other.m_key && this->m_value != _other.m_value && this->m_status != _other.m_status;

}

template<typename Key, typename Value>
inline iterator<Key, Value> new_map<Key, Value>::make_npos()
{
	Element<Key, Value> null_ptr;
	null_ptr.m_index = -1;
	//
	null_ptr.m_key = nullptr;
	null_ptr.m_value = nullptr;
	//
	null_ptr.m_status = Element<Key, Value>::NULLPTR;
	return iterator(null_ptr);
}

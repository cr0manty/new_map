#pragma once
#include <string>
#include <algorithm>
template<typename Key, typename Value>
class new_map
{
	struct Element
	{
		Key m_key;
		Value *m_value;
		enum { NOT_OCCUPIED, OCCUPIED, DELETED } m_status;
	};

	size_t m_size;
	size_t m_end;
	Element* m_pData;

	bool tryInsert(int _bucketNr, Key _key, Value _value);
	void resize();
	size_t m_begin;
public:
	new_map(size_t = 100);
	size_t npos = -1;
	size_t begin() const;
	size_t end() const;
	size_t size() const;
	void clear();
	void insert(Key, Value);
	size_t find(Key) const;
	~new_map();
};

template<typename Key, typename Value>
inline bool new_map<Key, Value>::tryInsert(int _bucketNr, Key _key, Value _value)
{
	//переделать под key и value
	if (m_pData[_bucketNr].m_status != HashTable::Element::OCCUPIED)
	{
		m_pData[_bucketNr].m_status = HashTable::Element::OCCUPIED;
		m_pData[_bucketNr].m_key = _key;
		m_pData[_bucketNr].m_value = _value;

		m_end++;
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

	Element* oldData = m_pData;
	m_pData = new Element[m_size];
	for (int i = 0; i < m_size; i++)
		m_pData[i].m_status = Element::NOT_OCCUPIED;

	m_end = 0;

	for (int i = 0; i < oldSize; i++)
		if (oldData[i].m_status == HashTable::Element::OCCUPIED) {
			if ((m_end << 1) >= m_size)
				resize();

			Key hashCode = oldData[i].m_key;
			auto bucketNr = hashCode % m_size;

			for (int i = bucketNr; i < m_size; i++)
				if (tryInsert(i, oldData[i].m_key, oldData[i].m_value))
					return;

			for (int i = 0; i < bucketNr; i++)
				if (tryInsert(i, oldData[i].m_key, oldData[i].m_value))
					return;

			insert(oldData[i].m_key, oldData[i].m_value);
		}

	for (int i = 0; i < oldSize; i++)
		if (oldData[i].m_status == HashTable::Element::OCCUPIED)
			delete[] oldData[i].m_value;
	delete[] oldData;
}

template<typename Key, typename Value>
inline new_map<Key, Value>::new_map(size_t _size) :
	m_size(_size), m_begin(0), m_end(0)
{
	m_pData = new Element[m_size];

	clear();
}

template<typename Key, typename Value>
inline size_t new_map<Key, Value>::begin() const
{
	return m_begin;
}

template<typename Key, typename Value>
inline size_t new_map<Key, Value>::end() const
{
	return m_end;
}

template<typename Key, typename Value>
inline size_t new_map<Key, Value>::size() const
{
	return m_end;
}

template<typename Key, typename Value>
inline void new_map<Key, Value>::clear()
{
	m_end = 0;
	for (int i = 0; i < m_size; i++)\
	{
		if (m_pData[i].m_status == Element::OCCUPIED)
		{
			if (m_pData[i].m_value)
				delete[] m_pData[i].m_value;
			m_pData[i].m_status = Element::NOT_OCCUPIED;
		}
	}
}

template<typename Key, typename Value>
inline void new_map<Key, Value>::insert(Key _key, Value _value)
{
}

template<typename Key, typename Value>
inline size_t new_map<Key, Value>::find(Key _key) const
{
	for (size_t i = 0; i < m_end; i++)
		if (m_pData[i].m_status == Element::OCCUPIED)
			if (m_pData[i].m_key == _key)
				return i;

	return npos;
}

template<typename Key, typename Value>
inline new_map<Key, Value>::~new_map()
{
	for (int i = 0; i < m_size; i++)
		if (m_pData[i].m_status == Element::OCCUPIED)
			delete[] m_pData[i].m_value;

	delete[] m_pData;
}

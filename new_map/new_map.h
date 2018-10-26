#pragma once

template<class nKey, class nValue>
class Element;

template<class nKey, class nValue>
class iterator;

template<typename Key, typename Value>
class new_map 
{
public:
	typedef iterator<Key, Value> iterator;
private:
	typedef Element<Key, Value> Element;
	friend class iterator;
	size_t m_size;
	size_t m_dOccupiedSize;
	Element* m_pData;
	bool tryInsert(size_t, Key, Value);
	void resize();
	size_t m_begin;
	iterator make_npos();
	iterator *iter;
public:
	new_map();
	new_map(size_t);
	iterator npos;
	iterator begin();
	iterator end();
	size_t size() const;
	bool empty() const;
	void clear();
	void emplace(Key, Value);
	void insert(Key, Value);
	iterator* find(Key);
	void print(std::ostream&);
	~new_map();
};

template<class nKey, class nValue>
class iterator
{
	typedef Element<nKey, nValue> Element;
	Element *m_Data;
public:
	iterator() {}
	iterator(Element *);
	nKey *first;
	nValue *second;
	
	iterator &operator=(Element *);
	bool operator!=(iterator);
	bool operator==(iterator);
	bool operator<=(iterator);
	bool operator>=(iterator);
	bool operator<(iterator);
	bool operator>(iterator);
	iterator operator++(int);
	iterator &operator++();
	iterator &operator--();
	iterator operator--(int);
};

template<class nKey, class nValue>
class Element
{
	friend class iterator<nKey, nValue>;
	friend class new_map<nKey,nValue>;
	
	nKey m_key;
	nValue m_value;
	enum {	NOT_OCCUPIED, OCCUPIED, DELETED, NULLPTR } m_status;

	bool operator==(Element);
	bool operator!=(Element);
	bool operator<=(Element);
	bool operator>=(Element);
	bool operator<(Element);
	bool operator>(Element);
};

template<typename Key, typename Value>
inline bool new_map<Key, Value>::tryInsert(size_t _index, Key _key, Value _value)
{
	if (m_pData[_index].m_status != Element::OCCUPIED)
	{
		m_pData[_index].m_status = Element::OCCUPIED;
		m_pData[_index].m_key = _key;
		m_pData[_index].m_value = _value;
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
	Element* oldData = m_pData;
	m_pData = new Element[m_size];
	for (int i = 0; i < m_size; i++)
		m_pData[i].m_status = Element::NOT_OCCUPIED;

	m_dOccupiedSize = 0;

	for (size_t i = 0; i < oldSize; i++)
		if (oldData[i].m_status == Element::OCCUPIED) {
			if ((m_dOccupiedSize << 1) >= m_size)
				resize();

			for (size_t j = i; j < m_size; j++)
				if (tryInsert(j, oldData[j].m_key, oldData[j].m_value))
					break;

			for (size_t j = 0; j < i; j++)
				if (tryInsert(j, oldData[j].m_key, oldData[j].m_value))
					break;
		}

	delete[] oldData;
}

template<typename Key, typename Value>
inline new_map<Key, Value>::new_map() :
	m_size(100), m_begin(0), m_dOccupiedSize(0), npos(make_npos())
{
	m_pData = new Element[m_size];

	clear();
}

template<typename Key, typename Value>
inline new_map<Key, Value>::new_map(size_t _size) :
	m_size(_size), m_begin(0), m_dOccupiedSize(0), npos(make_npos())
{
	m_pData = new Element[m_size];

	clear();
}

template<typename Key, typename Value>
inline iterator<Key, Value> new_map<Key, Value>::begin()
{
	while (m_pData[m_begin].m_status != Element::OCCUPIED && 
		m_dOccupiedSize >= m_begin) m_begin++;

	return &m_pData[m_begin];
}

template<class nKey, class nValue>
inline iterator<nKey, nValue> new_map<nKey, nValue>::end()
{
	size_t _index = m_dOccupiedSize;
	while (m_pData[_index].m_status == Element::OCCUPIED &&
		--_index >= 0);

	return &m_pData[_index];
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
inline void new_map<Key, Value >::clear()
{
	m_dOccupiedSize = 0;
	for (int i = 0; i < m_size; i++)
		if (m_pData[i].m_status == Element::OCCUPIED)
			m_pData[i].m_status = Element::NOT_OCCUPIED;
}

template<typename Key, typename Value>//
inline void new_map<Key, Value>::emplace(Key _key, Value _value)
{
	if (find(_key) != npos) {
		npos; //������ ��������
	}
	else{
		insert(_key, _value);
	}
}

template<typename Key, typename Value>
inline void new_map<Key, Value>::insert(Key _key, Value _value)
{
	if ((m_dOccupiedSize << 1) >= m_size)
		resize();

	for (size_t i = m_dOccupiedSize; i < m_size; i++)
		if (tryInsert(i, _key, _value))
			return;

	for (size_t i = 0; i < m_dOccupiedSize; i++)
		if (tryInsert(i, _key, _value))
			return;
}

template<typename Key, typename Value>
inline iterator<Key, Value>* new_map<Key, Value>::find(Key _key)
{
	for (size_t i = 0; i < m_dOccupiedSize; i++)
		if (m_pData[i].m_status == Element::OCCUPIED)
			if (m_pData[i].m_key == _key)
				return new iterator(&m_pData[i]);

	return &npos;;
}

template<class nKey, class nValue>
inline void new_map<nKey, nValue>::print(std::ostream& _st)
{
	for (size_t i = 0; i < m_dOccupiedSize; i++)
		_st << m_pData[i].m_key << " " << m_pData[i].m_value << '\n';
}

template<class nKey, class nValue>
inline new_map<nKey, nValue>::~new_map()
{
	delete[] m_pData;
}

template<class nKey, class nValue>
inline iterator<nKey, nValue>::iterator(Element* _data)
{
	m_Data = _data;
	first = &(*m_Data).m_key;
	second = &(*m_Data).m_value;
}

template<class nKey, class nValue>
inline iterator<nKey, nValue> & iterator<nKey, nValue>::operator=(Element * _data)
{
	m_Data = _data;
	first = &(*m_Data).m_key;
	second = &(*m_Data).m_value;
	return *this;
}

template<class nKey, class nValue>
inline bool iterator<nKey, nValue>::operator!=(iterator _other)
{
	return (*m_Data != *_other.m_Data);
}

template<class nKey, class nValue>
inline bool iterator<nKey, nValue>::operator==(iterator _other)
{
	return (*m_Data == *_other.m_Data);
}

template<class nKey, class nValue>
inline bool iterator<nKey, nValue>::operator<=(iterator _other)
{
	return (*m_Data <= *_other.m_Data);
}

template<class nKey, class nValue>
inline bool iterator<nKey, nValue>::operator>=(iterator _other)
{
	return (*m_Data >= *_other.m_Data);
}

template<class nKey, class nValue>
inline bool iterator<nKey, nValue>::operator<(iterator _other)
{
	return (*m_Data < *_other.m_Data);
}

template<class nKey, class nValue>
inline bool iterator<nKey, nValue>::operator>(iterator _other)
{
	return (*m_Data > *_other.m_Data);
}

template<class nKey, class nValue>
inline bool Element<nKey, nValue>::operator==(Element _other)
{
	return this->m_key == _other.m_key;
}

template<class nKey, class nValue>
inline bool Element<nKey, nValue>::operator!=(Element  _other)
{
	return this->m_key != _other.m_key;
}

template<class nKey, class nValue>
inline bool Element<nKey, nValue>::operator<=(Element _other)
{
	return this->m_key <= _other.m_key;
}

template<class nKey, class nValue>
inline bool Element<nKey, nValue>::operator>=(Element _other)
{
	return this->m_key >= _other.m_key;
}

template<class nKey, class nValue>
inline bool Element<nKey, nValue>::operator<(Element _other)
{
	return this->m_key < _other.m_key;
}

template<class nKey, class nValue>
inline bool Element<nKey, nValue>::operator>(Element _other)
{
	return this->m_key > _other.m_key;
}

template<class nKey, class nValue>
inline iterator<nKey, nValue> new_map<nKey, nValue>::make_npos()
{
	Element *null_ptr = new Element;
	null_ptr->m_status = Element::NULLPTR;
	return iterator(null_ptr);
}

template<class nKey, class nValue>
inline iterator<nKey, nValue>& iterator<nKey, nValue>::operator++()
{
	if (m_Data) {
		*m_Data++;
		return *this;
	}
}

template<class nKey, class nValue>
inline iterator<nKey, nValue> iterator<nKey, nValue>::operator++(int)
{
	if (m_Data) {
		*m_Data++;
		return *this;
	}
}

template<class nKey, class nValue>
inline iterator<nKey, nValue> & iterator<nKey, nValue>::operator--()
{
	if (m_Data) {
		*m_Data--;
		return *this;
	}
}

template<class nKey, class nValue>
inline iterator<nKey, nValue> iterator<nKey, nValue>::operator--(int)
{
	if (m_Data) {
		*m_Data--;
		return *this;
	}
}

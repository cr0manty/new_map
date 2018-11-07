#pragma once

template<class Key, class Value>
class new_map 
{
public: class iterator;
private:
	class Element;
	size_t m_size;
	size_t m_dOccupiedSize;
	Element* m_pData;
	bool _tryinsert(const size_t &, const Key &, const Value &);
	void resize();
	Element * make_npos() const;
	
public:
	new_map();
	new_map(const size_t &);
	new_map(const new_map&);
	new_map(new_map&&);
	new_map &operator=(const new_map&);
	new_map &operator=(new_map&&);

	Element * const npos;
	size_t size() const;
	bool empty() const;
	void clear();
	void emplace(const Key &, const Value &);
	void insert(const Key &, const Value &);
	Element* find(const Key &);
	iterator* rfind(const Key &);
	const iterator begin() const;
	iterator end() const;
	void print(std::ostream &);
	~new_map();
};

template<class BasicKey, class BasicValue>
class new_map<BasicKey, BasicValue>::iterator
{
	Element *m_Data;
public:
	iterator();
	iterator(Element *);
	iterator(const iterator&);
	iterator(iterator&&);
	iterator &operator=(iterator&&);

	Element &operator*();
	iterator &operator=(Element &);
	iterator operator++(int);
	iterator &operator++();
	iterator &operator--();
	iterator operator--(int);
	bool operator!=(const iterator &) const;
	bool operator==(const iterator &) const;
};

template<class BasicKey, class BasicValue>
class new_map<BasicKey, BasicValue>::Element
{
	friend class new_map<BasicKey, BasicValue>;
	enum STATUS{ NOT_OCCUPIED, OCCUPIED, DELETED, NULLPTR } m_status;
	void _newdata(const BasicKey &, const BasicValue &);
	bool operator==(const Element &);
	bool const operator!=(const Element &) const;
	operator bool();
	Element(STATUS) {
		m_status = NULLPTR;
	}
	Element();
public:
	BasicKey first;
	BasicValue second;

};

template<class Key, class Value>
inline bool new_map<Key, Value>::_tryinsert(const size_t &_index, const Key &_key, const Value &_value)
{
	if (m_pData[_index].m_status != Element::OCCUPIED)
	{
		m_pData[_index].m_status = Element::OCCUPIED;
		m_pData[_index].first = _key;
		m_pData[_index].second = _value;
		m_dOccupiedSize++;
		return true;
	}
	else
		return false;
}

template<class Key, class Value>
inline void new_map<Key, Value>::resize()
{
	size_t oldSize = m_size;
	m_size <<= 1;
	const Element* oldData = m_pData;
	m_pData = new Element[m_size];
	for (int i = 0; i < m_size; i++)
		m_pData[i].m_status = Element::NOT_OCCUPIED;

	m_dOccupiedSize = 0;

	for (size_t i = 0; i < oldSize; i++)
		if (oldData[i].m_status == Element::OCCUPIED) {
			if ((m_dOccupiedSize << 1) >= m_size)
				resize();

			for (size_t j = i; j < m_size; j++)
				if (_tryinsert(j, oldData[j].first, oldData[j].second))
					break;

			for (size_t j = 0; j < i; j++)
				if (_tryinsert(j, oldData[j].first, oldData[j].second))
					break;
		}

	delete[] oldData;
}

template<class Key, class Value>
inline typename new_map<Key, Value>::Element* new_map<Key, Value>::make_npos() const
{
	npos = new Element;
	npos->m_status = Element::NULLPTR;
}

template<class Key, class Value>
inline new_map<Key, Value>::new_map() :
	m_size(100), npos(make_npos())
{
	m_pData = new Element[m_size];

	clear();
}

template<class Key, class Value>
inline new_map<Key, Value>::new_map(const size_t &_size) :
	m_size(_size), m_dOccupiedSize(0), npos(make_npos())
{
	m_pData = new Element[m_size];

	clear();
}

template<class Key, class Value>
inline new_map<Key, Value>::new_map(const new_map &_other)
{
	m_size = _other.m_size;
	m_pData = new Element[m_size];
	m_dOccupiedSize = _other.m_dOccupiedSize;

	for (size_t i = 0; i < m_dOccupiedSize; i++)
		_tryinsert(i, _other.m_pData[i].first, _other.m_pData[i].second);

	return *this;
}

template<class Key, class Value>
inline new_map<Key, Value>::new_map(new_map &&_other):
	m_pData(_other.m_pData), m_size(_other.m_size),
	m_dOccupiedSize(_other.m_dOccupiedSize)
{
	delete[]_other.m_pData;
	_other.m_size = 0;
	m_dOccupiedSize.m_nUsed = 0;
}

template<class Key, class Value>
inline new_map<Key, Value> & new_map<Key, Value>::operator=(const new_map &_other)
{
	m_size = _other.m_size;
	m_pData = new Element[m_size];
	m_dOccupiedSize = _other.m_dOccupiedSize;

	for (size_t i = 0; i < m_dOccupiedSize; i++)
		_tryinsert(i, _other.m_pData[i].first, _other.m_pData[i].second);

	return *this;
}

template<class Key, class Value>
inline new_map<Key, Value> & new_map<Key, Value>::operator=(new_map &&_other)
{
	m_pData = _other.m_pData;
	m_size = _other.m_size;
	m_dOccupiedSize = _other.m_dOccupiedSize;

	delete[] _other.m_pData;
	_other.m_dOccupiedSize = 0;
	_other.m_size = 0;

	return *this;
}

template<class Key, class Value>
inline size_t new_map<Key, Value>::size() const
{
	return m_dOccupiedSize;
}

template<class Key, class Value>
inline bool new_map<Key, Value>::empty() const
{
	return m_dOccupiedSize;
}

template<class Key, class Value>
inline void new_map<Key, Value >::clear()
{
	m_dOccupiedSize = 0;
	for (int i = 0; i < m_size; i++)
		if (m_pData[i].m_status == Element::OCCUPIED)
			m_pData[i].m_status = Element::NOT_OCCUPIED;
}

template<class Key, class Value>
inline void new_map<Key, Value>::emplace(const Key &_key, const Value &_value)
{
	auto iter = find(_key);

	if (iter != npos)
		iter->_newdata(_key, _value);
	else
		insert(_key, _value);
}

template<class Key, class Value>
inline void new_map<Key, Value>::insert(const Key &_key, const Value &_value)
{
	if ((m_dOccupiedSize << 1) >= m_size)
		this->resize();

	for (size_t i = m_dOccupiedSize; i < m_size; i++)
		if (_tryinsert(i, _key, _value))
			return;

	for (size_t i = 1; i < m_dOccupiedSize; i++)
		if (_tryinsert(i, _key, _value))
			return;
}

template<class Key, class Value>
inline typename new_map<Key, Value>::Element* new_map<Key, Value>::find(const Key &_key)
{
	for (size_t i = 0; i < m_dOccupiedSize; i++)
		if (m_pData[i].m_status == Element::OCCUPIED)
			if (m_pData[i].first == _key)
				return &m_pData[i];

	return npos;
}

template<class Key, class Value>
inline typename new_map<Key, Value>::iterator* new_map<Key, Value>::rfind(const Key &_key)
{
	for (size_t i = m_dOccupiedSize; i > 0; i--)
		if (m_pData[i].m_status == Element::OCCUPIED)
			if (m_pData[i].first == _key)
				return &m_pData[i];

	return npos;
}

template<class Key, class Value>
inline typename const new_map<Key, Value>::iterator new_map<Key, Value>::begin() const
{
	size_t m_begin = 0;

	while (m_pData[m_begin].m_status != Element::OCCUPIED &&
		m_dOccupiedSize >= m_begin) m_begin++;

	return &m_pData[m_begin];
}

template<class Key, class Value>
inline typename new_map<Key, Value>::iterator new_map<Key, Value>::end() const
{
	return &m_pData[m_dOccupiedSize];
}

template<class Key, class Value>
inline void new_map<Key, Value>::print(std::ostream& _st)
{
	for (size_t i = 0; i < m_dOccupiedSize; i++)
		_st << m_pData[i].first << " " << m_pData[i].second << '\n';
}

template<class Key, class Value>
inline new_map<Key, Value>::~new_map()
{
	delete[] m_pData;
}

template<class BasicKey, class BasicValue>
inline void new_map<BasicKey, BasicValue>::Element::_newdata(const BasicKey &_key, const BasicValue &_value)
{
	first = _key;
	second = _value;
}

template<class BasicKey, class BasicValue>
inline new_map<BasicKey, BasicValue>::iterator::iterator() 
{
}

template<class Key, class Value>
inline new_map<Key, Value>::iterator::iterator(Element * _data) :
	m_Data(_data)
{
}

template<class BasicKey, class BasicValue>
inline new_map<BasicKey, BasicValue>::iterator::iterator(const iterator &_other) :
	m_Data(_other.m_Data)
{
}

template<class BasicKey, class BasicValue>
inline new_map<BasicKey, BasicValue>::iterator::iterator(iterator &&_other) :
	m_Data(_other.m_Data)
{
	delete[] _other.m_Data;
}

template<class BasicKey, class BasicValue>
inline typename new_map<BasicKey, BasicValue>::iterator & new_map<BasicKey, BasicValue>::iterator::operator=(iterator &&_other)
{
	m_Data = _other.m_Data;

	delete[] _other.m_Data;
	return *this;
}

template<class BasicKey, class BasicValue>
inline typename new_map<BasicKey, BasicValue>::Element & new_map<BasicKey, BasicValue>::iterator::operator*()
{
	return (*m_Data);
}

template<class BasicKey, class BasicValue>
inline typename new_map<BasicKey, BasicValue>::iterator & new_map<BasicKey, BasicValue>::iterator::operator=(Element &_data)
{
	m_Data = _data;
	return *this;
}

template<class BasicKey, class BasicValue>
inline typename new_map<BasicKey, BasicValue>::iterator new_map<BasicKey, BasicValue>::iterator::operator++(int)
{
	if (m_Data) {
		*m_Data++;
		return *this;
	}
}

template<class BasicKey, class BasicValue>
inline typename new_map<BasicKey, BasicValue>::iterator & new_map<BasicKey, BasicValue>::iterator::operator++()
{
	if (m_Data) {
		*++m_Data;
		return *this;
	}
}

template<class BasicKey, class BasicValue>
inline typename new_map<BasicKey, BasicValue>::iterator & new_map<BasicKey, BasicValue>::iterator::operator--()
{
	if (m_Data) {
		*--m_Data;
		return *this;
	}
}

template<class BasicKey, class BasicValue>
inline typename new_map<BasicKey, BasicValue>::iterator new_map<BasicKey, BasicValue>::iterator::operator--(int)
{
	if (m_Data) {
		*m_Data--;
		return *this;
	}
}

template<class BasicKey, class BasicValue>
inline bool new_map<BasicKey, BasicValue>::iterator::operator!=(const iterator &_other) const
{
	return (*m_Data != *_other.m_Data);
}

template<class BasicKey, class BasicValue>
inline bool new_map<BasicKey, BasicValue>::iterator::operator==(const iterator &_other)const
{
	return (*m_Data == *_other.m_Data);
}

template<class BasicKey, class BasicValue>
inline new_map<BasicKey, BasicValue>::Element::Element()
{
}


template<class BasicKey, class BasicValue>
inline bool new_map<BasicKey, BasicValue>::Element::operator==(const Element &_other)
{
	return this->first == _other.first;
}

template<class BasicKey, class BasicValue>
inline const bool new_map<BasicKey, BasicValue>::Element::operator!=(const Element &_other) const
{
	return this->first != _other.first;
}

template<class BasicKey, class BasicValue>
inline new_map<BasicKey, BasicValue>::Element::operator bool()
{
	return this;
}

#pragma once

template<class Key, class Value>
class new_map 
{
public: class iterator;
private:
	class Element;
	friend class iterator;
	size_t m_size;
	size_t m_dOccupiedSize;
	Element* m_pData;
	bool _tryinsert(const size_t &, const Key &, const Value &);
	void resize();
	iterator make_npos() const;
	
public:
	new_map();
	new_map(const size_t &);
	new_map(const new_map&);
	new_map(new_map&&);
	new_map &operator=(const new_map&);
	new_map &operator=(new_map&&);

	iterator npos;
	size_t size() const;
	bool empty() const;
	void clear();
	void emplace(const Key &, const Value &);
	void insert(const Key &, const Value &);
	iterator* find(const Key &);
	iterator* rfind(const Key &);
	iterator begin() const;
	iterator const end() const;
	void print(std::ostream &);
	~new_map();
};

template<class BasicKey, class BasicValue>
class new_map<BasicKey, BasicValue>::iterator
{
	friend class new_map<BasicKey, BasicValue>;
	Element *m_Data;
	void _redata();
	void _newdata(const BasicKey &, const BasicValue &);
	iterator(const iterator&) = 0;
public:
	BasicKey &first;
	BasicValue &second;

	//iterator() {}
	iterator(Element &);

	iterator(iterator&&);
	iterator &operator=(iterator&&);

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
	BasicKey m_key;
	BasicValue m_value;
	enum { NOT_OCCUPIED, OCCUPIED, DELETED, NULLPTR } m_status;

	bool operator==(const Element &);
	bool const operator!=(const Element &) const;
};

template<class Key, class Value>
inline bool new_map<Key, Value>::_tryinsert(const size_t &_index, const Key &_key, const Value &_value)
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
				if (_tryinsert(j, oldData[j].m_key, oldData[j].m_value))
					break;

			for (size_t j = 0; j < i; j++)
				if (_tryinsert(j, oldData[j].m_key, oldData[j].m_value))
					break;
		}

	delete[] oldData;
}

template<class Key, class Value>
inline typename new_map<Key, Value>::iterator new_map<Key, Value>::make_npos() const
{
	Element null_ptr;
	null_ptr.m_status = Element::NULLPTR;
	null_ptr.m_key = Key("-1");
	null_ptr.m_value = Value("-1");

	return iterator(null_ptr);
}

template<class Key, class Value>
inline new_map<Key, Value>::new_map() :
	m_size(100), m_dOccupiedSize(0), npos(make_npos())
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
		_tryinsert(i, _other.m_pData[i].m_key, _other.m_pData[i].m_value);

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
		_tryinsert(i, _other.m_pData[i].m_key, _other.m_pData[i].m_value);

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
		iter._newdata(_key, _value);

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

	for (size_t i = 0; i < m_dOccupiedSize; i++)
		if (_tryinsert(i, _key, _value))
			return;
}

template<class Key, class Value>
inline typename new_map<Key, Value>::iterator* new_map<Key, Value>::find(const Key &_key)
{
	for (size_t i = 0; i < m_dOccupiedSize; i++)
		if (m_pData[i].m_status == Element::OCCUPIED)
			if (m_pData[i].m_key == _key)
				return new iterator(m_pData[i]);

	return &npos;
}

template<class Key, class Value>
inline typename new_map<Key, Value>::iterator* new_map<Key, Value>::rfind(const Key &_key)
{
	for (size_t i = m_dOccupiedSize; i > 0; i--)
		if (m_pData[i].m_status == Element::OCCUPIED)
			if (m_pData[i].m_key == _key)
				return new iterator(m_pData[i]);

	return &npos;
}

template<class Key, class Value>
inline typename new_map<Key, Value>::iterator new_map<Key, Value>::begin() const
{
	size_t m_begin = 0;

	while (m_pData[m_begin].m_status != Element::OCCUPIED &&
		m_dOccupiedSize >= m_begin) m_begin++;

	return m_pData[m_begin];
}

template<class Key, class Value>
inline typename new_map<Key, Value>::iterator const new_map<Key, Value>::end() const
{
	size_t _index = m_dOccupiedSize;
	while (m_pData[_index].m_status == Element::OCCUPIED &&
		--_index >= 0);

	return m_pData[_index];
}

template<class Key, class Value>
inline void new_map<Key, Value>::print(std::ostream& _st)
{
	for (size_t i = 0; i < m_dOccupiedSize; i++)
		_st << m_pData[i].m_key << " " << m_pData[i].m_value << '\n';
}

template<class Key, class Value>
inline new_map<Key, Value>::~new_map()
{
	delete[] m_pData;
}

template<class BasicKey, class BasicValue>
inline void new_map<BasicKey, BasicValue>::iterator::_redata()
{
	first = (*m_Data).m_key;
	second = (*m_Data).m_value;
}

template<class BasicKey, class BasicValue>
inline void new_map<BasicKey, BasicValue>::iterator::_newdata(const BasicKey &_key, const BasicValue &_value)
{
	this->m_Data = Element(_key, _value);
	_redata();
}

template<class Key, class Value>
inline new_map<Key, Value>::iterator::iterator(Element &_data) :
	m_Data(&_data), first((*m_Data).m_key), second((*m_Data).m_value)
{
}

template<class BasicKey, class BasicValue>
inline new_map<BasicKey, BasicValue>::iterator::iterator(const iterator &_other) :
	m_Data(_other.m_Data), second(_other.second), first(_other.first)
{
}

template<class BasicKey, class BasicValue>
inline new_map<BasicKey, BasicValue>::iterator::iterator(iterator &&_other) :
	m_Data(_other.m_Data), second(_other.second), first(_other.first)
{
	delete[] _other.m_Data;
}

template<class BasicKey, class BasicValue>
inline typename new_map<BasicKey, BasicValue>::iterator & new_map<BasicKey, BasicValue>::iterator::operator=(iterator &&_other)
{
	m_Data = _other.m_Data;
	first = _other.first;
	second = _other.second;

	delete[] _other.m_Data;
	return *this;
}

template<class BasicKey, class BasicValue>
inline typename new_map<BasicKey, BasicValue>::iterator & new_map<BasicKey, BasicValue>::iterator::operator=(Element &_data)
{
	m_Data = _data;
	_redata();
	return *this;
}

template<class BasicKey, class BasicValue>
inline typename new_map<BasicKey, BasicValue>::iterator new_map<BasicKey, BasicValue>::iterator::operator++(int)
{
	if (m_Data) {
		*++m_Data;
		_redata();
		return *this;
	}
}

template<class BasicKey, class BasicValue>
inline typename new_map<BasicKey, BasicValue>::iterator & new_map<BasicKey, BasicValue>::iterator::operator++()
{
	if (m_Data) {
		*m_Data++;
		_redata();
		return *this;
	}
}

template<class BasicKey, class BasicValue>
inline typename new_map<BasicKey, BasicValue>::iterator & new_map<BasicKey, BasicValue>::iterator::operator--()
{
	if (m_Data) {
		*m_Data--;
		_redata();
		return *this;
	}
}

template<class BasicKey, class BasicValue>
inline typename new_map<BasicKey, BasicValue>::iterator new_map<BasicKey, BasicValue>::iterator::operator--(int)
{
	if (m_Data) {
		*--m_Data;
		_redata();
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
inline bool new_map<BasicKey, BasicValue>::Element::operator==(const Element &_other)
{
	return this->m_key == _other.m_key;
}

template<class BasicKey, class BasicValue>
inline const bool new_map<BasicKey, BasicValue>::Element::operator!=(const Element &_other) const
{
	return this->m_key != _other.m_key;
}
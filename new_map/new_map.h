#ifndef NEW_MAP_H
#define NEW_MAP_H
#include <cassert>

namespace std {
	template<class Key, class Value>
	class new_map
	{
	public: class iterator;
	private:
		class Element;
		size_t m_size;
		Element* m_pData;
		void _destroy(Element*);
		void _transplant(Element *, Element*);
	public:
		new_map();
		new_map(const new_map&);

		new_map &operator=(const new_map&);
		Value &operator[](const Key &);

		size_t size() const;
		bool empty() const;
		void erase(const Key &);
		void insert(const Key &, const Value &);
		Element* at(const Key &) const;
		Element* find(const Key &) const;
		Element* vfind(const Value &) const;
		iterator begin() const;
		iterator end() const;

		~new_map();
	};

	template<class BasicKey, class BasicValue>
	class new_map<BasicKey, BasicValue>::iterator
	{
		friend class new_map<BasicKey, BasicValue>;
		typedef new_map<BasicKey, BasicValue>::Element Element;
		Element *m_Data;
		Element *m_current;
		Element *m_previous;
		void operatorInk(size_t = 1);
	public:
		iterator();
		iterator(Element *);
		iterator(const iterator*);

		Element &operator*();
		iterator &operator=(Element &);
		iterator &operator++();
		iterator operator++(int);
		iterator &operator--();
		iterator operator--(int);
		iterator &operator-(const size_t &);
		iterator &operator-=(const size_t &);
		iterator &operator+(const size_t &);
		iterator &operator+=(const size_t &);
		bool operator!=(const iterator &) const;
		bool operator==(const iterator &) const;
	};

	template<class BasicKey, class BasicValue>
	class new_map<BasicKey, BasicValue>::Element
	{
		friend class new_map<BasicKey, BasicValue>;
		Element(const BasicKey &, const BasicValue &);
		Element * m_parent;
		Element * m_left;
		Element * m_right;
		bool operator==(const Element &) const;
		bool const operator!=(const Element &) const;
		operator bool() const;
	public:
		BasicKey first;
		BasicValue second;
	};
}
#endif

template<class Key, class Value>
inline void std::new_map<Key, Value>::_destroy(Element *_root)
{
	if (!_root)
		return;

	_destroy(_root->m_left);
	_destroy(_root->m_right);

	delete _root;
}

template<class Key, class Value>
inline void std::new_map<Key, Value>::_transplant(Element *_node, Element *_otherNode)
{
	if (!_node->m_parent)
	{
		assert(_node == m_pData);
		m_pData = _otherNode;
	}

	else if (_node->m_parent->m_left == _node)
		_node->m_parent->m_left = _otherNode;

	else if (_node->m_parent->m_right == _node)
		_node->m_parent->m_right = _otherNode;

	else
		assert(0);

	if (_otherNode)
		_otherNode->m_parent = _node->m_parent;
}

template<class Key, class Value>
inline std::new_map<Key, Value>::new_map() :
	m_pData(nullptr)
{
}

template<class Key, class Value>
inline std::new_map<Key, Value>::new_map(const new_map &_other)
{
	if (m_pData)
		_destroy(m_pData);

	m_pData = _other.m_pData;
	m_size = _other.m_size;

	return *this;
}

template<class Key, class Value>
inline std::new_map<Key, Value> & std::new_map<Key, Value>::operator=(const new_map &_other)
{
	if (m_pData)
		_destroy(m_pData);

	m_pData = _other.m_pData;
	m_size = _other.m_size;
	return *this;
}

template<class Key, class Value>
inline typename Value & std::new_map<Key, Value>::operator[](const Key &_key)
{
	Element *_find = find(_key);

	if (!_find) {
		insert(_key, Value());
		return find(_key)->second;
	}
	else
		return _find->second;
}

template<class Key, class Value>
inline size_t std::new_map<Key, Value>::size() const
{
	return m_size;
}

template<class Key, class Value>
inline bool std::new_map<Key, Value>::empty() const
{
	return m_size;
}

template<class Key, class Value>
inline void std::new_map<Key, Value>::erase(const Key &_key)
{
	Element * _node = find(_key);
	if (!_node)
		return;

	if (!_node->m_left)
		_transplant(_node, _node->m_right);

	else if (!_node->m_right)
		_transplant(_node, _node->m_left);

	else
	{
		Element *_newxtNode = begin().m_Data;
		if (_newxtNode->m_parent != _node)
		{
			_transplant(_newxtNode, _newxtNode->m_right);
			_newxtNode->m_right = _node->m_right;
			_newxtNode->m_right->m_parent = _newxtNode;
		}

		_transplant(_node, _newxtNode);
		_newxtNode->m_left = _node->m_left;
		_newxtNode->m_left->m_parent = _newxtNode;
	}

	delete _node;
	m_size--;
}

template<class Key, class Value>
inline void std::new_map<Key, Value>::insert(const Key &_key, const Value &_value)
{
	Element * _current = m_pData;

	if (!_current)
	{
		m_pData = new Element(_key, _value);
		m_size++;
		return;
	}

	while (_current)
	{
		if (_current->first == _key)
			return;

		else if (_key < _current->first)
		{
			if (_current->m_left)
				_current = _current->m_left;

			else
			{
				Element * _newNode = new Element(_key, _value);
				_newNode->m_parent = _current;
				_current->m_left = _newNode;
				m_size++;
				return;
			}
		}

		else
		{
			if (_current->m_right)
				_current = _current->m_right;

			else
			{
				Element* _newNode = new Element(_key, _value);
				_newNode->m_parent = _current;
				_current->m_right = _newNode;
				m_size++;
				return;
			}
		}
	}
}

template<class Key, class Value>
inline typename std::new_map<Key, Value>::Element* std::new_map<Key, Value>::at(const Key &_key) const
{
	Element * _current = m_pData;

	while (_current)
	{
		if (_key == _current->first)
			return _current;

		else if (_key < _current->first)
			_current = _current->m_left;

		else
			_current = _current->m_right;
	}

	return nullptr;
}

template<class Key, class Value>
inline typename std::new_map<Key, Value>::Element* std::new_map<Key, Value>::find(const Key &_key) const
{
	Element * _current = m_pData;

	while (_current)
	{
		if (_key == _current->first)
			return _current;

		else if (_key < _current->first)
			_current = _current->m_left;

		else
			_current = _current->m_right;
	}

	return nullptr;
}

template<class Key, class Value>
inline typename std::new_map<Key, Value>::Element * std::new_map<Key, Value>::vfind(const Value & _value) const
{
	Element * _current = m_pData;

	while (_current)
	{
		if (_value == _current->second)
			return _current;

		else if (_value < _current->second)
			_current = _current->m_left;

		else
			_current = _current->m_right;
	}

	return nullptr;
}

template<class Key, class Value>
inline typename std::new_map<Key, Value>::iterator std::new_map<Key, Value>::begin() const
{
	Element * _node = m_pData;
	while (_node && _node->m_left)
		_node = _node->m_left;

	return _node;
}

template<class Key, class Value>
inline typename std::new_map<Key, Value>::iterator std::new_map<Key, Value>::end() const
{
	Element * _node = m_pData;
	while (_node && _node->m_right)
		_node = _node->m_right;

	return _node;
}

template<class Key, class Value>
inline std::new_map<Key, Value>::~new_map()
{
	_destroy(m_pData);
}

template<class BasicKey, class BasicValue>
inline std::new_map<BasicKey, BasicValue>::iterator::iterator() :
	m_Data(nullptr), m_current(nullptr), m_previous(nullptr)
{
}

template<class Key, class Value>
inline std::new_map<Key, Value>::iterator::iterator(Element * _data) :
	m_Data(_data), m_current(m_Data), m_previous(nullptr)
{
}

template<class BasicKey, class BasicValue>
inline std::new_map<BasicKey, BasicValue>::iterator::iterator(const iterator *_other)
{
	if(m_Data)
		delete m_Data;

	m_Data = _other.m_Data;
}

template<class BasicKey, class BasicValue>
inline typename void std::new_map<BasicKey, BasicValue>::iterator::operatorInk(size_t _amount)
{
	while (_amount--) {
		if (m_previous == m_current->m_parent) {
			m_previous = m_current;
			if (m_current->m_left) {
				m_current = m_current->m_left;
			}
			else {
				if (m_current->m_right)
					m_current = m_current->m_right;
				else
					m_current = m_current->m_parent;
			}
		}
		else if (m_previous == m_current->m_left) {
			m_previous = m_current;
			if (m_current->m_right)
				m_current = m_current->m_right;
			else
				m_current = m_current->m_parent;
		}
		else if (m_previous == m_current->m_right) {
			m_previous = m_current;
			m_current = m_current->m_parent;
		}
	}
}

template<class BasicKey, class BasicValue>
inline typename std::new_map<BasicKey, BasicValue>::Element & std::new_map<BasicKey, BasicValue>::iterator::operator*()
{
	return (*m_current);
}

template<class BasicKey, class BasicValue>
inline typename std::new_map<BasicKey, BasicValue>::iterator & std::new_map<BasicKey, BasicValue>::iterator::operator=(Element &_data)
{
	m_Data = _data;
	return *this;
}

template<class BasicKey, class BasicValue>
inline typename std::new_map<BasicKey, BasicValue>::iterator & std::new_map<BasicKey, BasicValue>::iterator::operator++()
{
	operatorInk();
	return *this;
}

template<class BasicKey, class BasicValue>
inline typename std::new_map<BasicKey, BasicValue>::iterator std::new_map<BasicKey, BasicValue>::iterator::operator++(int)
{
	operatorInk();
	return *this;
}

template<class BasicKey, class BasicValue>
inline typename std::new_map<BasicKey, BasicValue>::iterator & std::new_map<BasicKey, BasicValue>::iterator::operator--()
{
	operatorInk();
	return *this;
}

template<class BasicKey, class BasicValue>
inline typename std::new_map<BasicKey, BasicValue>::iterator std::new_map<BasicKey, BasicValue>::iterator::operator--(int)
{
	operatorInk();
	return *this;
}

template<class BasicKey, class BasicValue>
inline typename std::new_map<BasicKey, BasicValue>::iterator & std::new_map<BasicKey, BasicValue>::iterator::operator-(const size_t &_amount)
{
	operatorInk(_amount);
	return *this;
}

template<class BasicKey, class BasicValue>
inline typename std::new_map<BasicKey, BasicValue>::iterator & std::new_map<BasicKey, BasicValue>::iterator::operator-=(const size_t &_amount)
{
	operatorInk(_amount);
	return *this;
}

template<class BasicKey, class BasicValue>
inline typename std::new_map<BasicKey, BasicValue>::iterator & std::new_map<BasicKey, BasicValue>::iterator::operator+(const size_t &_amount)
{
	operatorInk(_amount);
	return *this;
}

template<class BasicKey, class BasicValue>
inline typename std::new_map<BasicKey, BasicValue>::iterator & std::new_map<BasicKey, BasicValue>::iterator::operator+=(const size_t &_amount)
{
	operatorInk(_amount);
	return *this;
}

template<class BasicKey, class BasicValue>
inline bool std::new_map<BasicKey, BasicValue>::iterator::operator!=(const iterator &_other) const
{
	return (m_current && !(*this == _other));
}

template<class BasicKey, class BasicValue>
inline bool std::new_map<BasicKey, BasicValue>::iterator::operator==(const iterator &_other)const
{
	return (m_current && *m_previous == *_other.m_current);
}

template<class BasicKey, class BasicValue>
inline std::new_map<BasicKey, BasicValue>::Element::Element(const BasicKey & _key, const BasicValue & _value) :
	first(_key), second(_value), m_parent(nullptr), m_left(nullptr), m_right(nullptr)
{
}

template<class BasicKey, class BasicValue>
inline bool std::new_map<BasicKey, BasicValue>::Element::operator==(const Element &_other) const
{
	return (&first == &_other.first && &second == &_other.second);
}

template<class BasicKey, class BasicValue>
inline const bool std::new_map<BasicKey, BasicValue>::Element::operator!=(const Element &_other) const
{
	return (!(*this == _other));
}

template<class BasicKey, class BasicValue>
inline std::new_map<BasicKey, BasicValue>::Element::operator bool() const
{
	return (*this);
}

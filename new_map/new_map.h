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
		enum Color { RED, BLACK };
		size_t m_size;
		Element* m_pData;
		void _destroy(Element*);
		void _transplant(Element *, Element*);
		Element* _tryinsert(const Key &, const Value &);
		Color _getcolor(const Element*);
		void _leftRotate(Element*);
		void _rightRotate(Element*);
	public:
		new_map();
		new_map(const new_map&);
		new_map(new_map&&);

		new_map &operator=(const new_map&);
		new_map &operator=(new_map&&);
		Element &operator[](const Key &);

		size_t size() const;
		bool empty() const;
		void erase(const Key &);
		void insert(const Key &, const Value &);
		Element* find(const Key &) const;
		Element* vfind(const Value &) const;
		Element* begin();
		Element* end();
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
	public:
		iterator();
		iterator(Element *);
		iterator(const iterator&);
		iterator(iterator&&);
		iterator &operator=(iterator&&);

		Element &operator*();
		iterator &operator=(Element &);
		iterator &operator++();
		iterator operator++(int);
		iterator &operator--();
		iterator operator--(int);
		bool operator!=(const iterator &) const;
		bool operator==(const iterator &) const;
	};

	template<class BasicKey, class BasicValue>
	class new_map<BasicKey, BasicValue>::Element
	{
		friend class new_map<BasicKey, BasicValue>;
		typedef new_map<BasicKey, BasicValue>::Color Color;
		Element(const BasicKey &, const BasicValue &);
		Color m_color;
		Element * m_parent;
		Element * m_left;
		Element * m_right;
		bool operator==(const Element &);
		bool const operator!=(const Element &) const;
		operator bool();
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
inline typename std::new_map<Key, Value>::Color std::new_map<Key, Value>::_getcolor(const Element *_node)
{
	return (_node) ? _node->m_color : Color::BLACK;
}

template<class Key, class Value>
inline void std::new_map<Key, Value>::_leftRotate(Element *_left)
{
	Element* right = _left->m_right;
	if (!right)
		return;

	_left->m_right = right->m_left;
	if (_left->m_right)
		_left->m_right->m_parent = _left;

	right->m_parent = _left->m_parent;
	if (!right->m_parent)
		m_pData = right;
	else if (_left == _left->m_parent->m_left)
		_left->m_parent->m_left = right;
	else if (_left == _left->m_parent->m_right)
		_left->m_parent->m_right = right;
	else
		assert(0);

	right->m_left = _left;
	_left->m_parent = right;
}

template<class Key, class Value>
inline void std::new_map<Key, Value>::_rightRotate(Element *_right)
{
	Element* left = _right->m_left;
	if (!left)
		return;

	_right->m_left = left->m_right;
	if (_right->m_left)
		_right->m_left->m_parent = _right;

	left->m_parent = _right->m_parent;
	if (!left->m_parent)
		m_pData = left;
	else if (_right == _right->m_parent->m_left)
		_right->m_parent->m_left = left;
	else if (_right == _right->m_parent->m_right)
		_right->m_parent->m_right = left;
	else
		assert(0);

	left->m_right = _right;
	_right->m_parent = left;
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
inline std::new_map<Key, Value>::new_map(new_map &&_other):
	m_size(_other.m_size)
{
	if (m_pData)
		_destroy(m_pData);

	m_pData = _other.m_pData;
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
inline std::new_map<Key, Value> & std::new_map<Key, Value>::operator=(new_map &&_other)
{
	if (m_pData)
		_destroy(m_pData);

	m_pData = _other.m_pData;
	m_size = _other.m_size;

	return *this;
}

template<class Key, class Value>
inline typename std::new_map<Key, Value>::Element & std::new_map<Key, Value>::operator[](const Key &_key)
{
	return (*find(_key));
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
		Element *_newxtNode = begin();
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
	Element * _newElement = _tryinsert(_key,_value);
	if (!_newElement)
		return;

	Element * _parent;
	while (_newElement && (_parent = _newElement->m_parent) && _getcolor(_parent) == Color::RED)
	{
		Element* _new_parent = _parent->m_parent;
		if (_parent == _new_parent->m_left)
		{
			Element* right = _new_parent->m_right;
			if (_getcolor(right) == Color::RED)
			{
				_parent->m_color = Color::BLACK;
				right->m_color = Color::BLACK;
				_new_parent->m_color = Color::RED;
				_newElement = _new_parent;
			}
			else
			{
				if (_newElement == _parent->m_right)
				{
					_newElement = _parent;
					_leftRotate(_newElement);
				}

				_parent = _newElement->m_parent;
				_new_parent = _parent->m_parent;

				_parent->m_color = Color::BLACK;
				if (_new_parent)
				{
					_new_parent->m_color = Color::RED;
					_rightRotate(_new_parent);
				}
			}
		}
		else
		{
			Element* left = _new_parent->m_left;
			if (_getcolor(left) == Color::RED)
			{
				_parent->m_color = Color::BLACK;
				left->m_color = Color::BLACK;
				_new_parent->m_color = Color::RED;
				_newElement = _new_parent;
			}
			else
			{
				if (_newElement == _parent->m_left)
				{
					_newElement = _parent;
					_rightRotate(_newElement);
				}

				_parent = _newElement->m_parent;
				_new_parent = _parent->m_parent;

				_parent->m_color = Color::BLACK;
				if (_new_parent)
				{
					_new_parent->m_color = Color::RED;
					_leftRotate(_new_parent);
				}
			}
		}
	}

	m_size++;
	m_pData->m_color = Color::BLACK;
}

template<class Key, class Value>
inline typename std::new_map<Key,Value>::Element* std::new_map<Key, Value>::_tryinsert(const Key &_key, const Value &_value)
{
	Element * _current = m_pData;
	
	if (!_current)
	{
		m_pData = new Element(_key, _value);
		return m_pData;
	}

	while (_current)
	{
		if (_current->first == _key)
			return nullptr;

		else if (_key < _current->first)
		{
			if (_current->m_left)
				_current = _current->m_left;

			else
			{
				Element * _newNode = new Element(_key, _value);
				_newNode->m_parent = _current;
				_current->m_left = _newNode;
				return _newNode;
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
				return _newNode;
			}
		}
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
inline typename std::new_map<Key, Value>::Element* std::new_map<Key, Value>::begin()
{
	Element * _node = m_pData;
	while (_node && _node->m_left)
		_node = _node->m_left;

	return _node;
}

template<class Key, class Value>
inline typename std::new_map<Key, Value>::Element* std::new_map<Key, Value>::end()
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
	m_Data(nullptr)
{
}

template<class Key, class Value>
inline std::new_map<Key, Value>::iterator::iterator(Element * _data) :
	m_Data(_data), m_current(m_Data), m_previous(nullptr)
{
}

template<class BasicKey, class BasicValue>
inline std::new_map<BasicKey, BasicValue>::iterator::iterator(const iterator &_other)
{
	if(m_Data)
		delete m_Data;

	m_Data = _other.m_Data;
}

template<class BasicKey, class BasicValue>
inline std::new_map<BasicKey, BasicValue>::iterator::iterator(iterator &&_other)
{
	if (m_Data)
		delete m_Data;

	m_Data = _other.m_Data;
}

template<class BasicKey, class BasicValue>
inline typename std::new_map<BasicKey, BasicValue>::iterator & std::new_map<BasicKey, BasicValue>::iterator::operator=(iterator &&_other)
{
	if (m_Data)
		delete m_Data;

	m_Data = _other.m_Data;

	return *this;
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
	return *this;
}

template<class BasicKey, class BasicValue>
inline bool std::new_map<BasicKey, BasicValue>::iterator::operator!=(const iterator &_other) const
{
	return m_current && (m_current->first != _other.m_Data->first);
}

template<class BasicKey, class BasicValue>
inline bool std::new_map<BasicKey, BasicValue>::iterator::operator==(const iterator &_other)const
{
	return (m_Data->first == _other.m_Data->first);
}

template<class BasicKey, class BasicValue>
inline std::new_map<BasicKey, BasicValue>::Element::Element(const BasicKey & _key, const BasicValue & _value) :
	first(_key), second(_value), m_parent(nullptr), m_left(nullptr), m_right(nullptr), m_color(Color::RED)
{
}

template<class BasicKey, class BasicValue>
inline bool std::new_map<BasicKey, BasicValue>::Element::operator==(const Element &_other)
{
	return this->first == _other.first;
}

template<class BasicKey, class BasicValue>
inline const bool std::new_map<BasicKey, BasicValue>::Element::operator!=(const Element &_other) const
{
	return this->first != _other.first;
}

template<class BasicKey, class BasicValue>
inline std::new_map<BasicKey, BasicValue>::Element::operator bool()
{
	return (*this);
}

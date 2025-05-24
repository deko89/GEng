#ifndef MassivPtr_H
#define MassivPtr_H

#include <vector>

/// Массив указателей.
template<class C> class MassivPtr: public std::vector<C*>
{   public:
    typedef std::vector<C*> Base;
	~MassivPtr() { clear(); }
	C* Add() { C* p = new C;   Base::push_back(p);   return p; }
	C* Add(C* p) { Base::push_back(p);   return p; }
	template<typename T = C, typename... Arg> T* Make(Arg... arg) { T* p = new T(arg...);   Base::push_back(p);   return p; }
	void clear()
	{
		for (C* e: *this) delete e;
		Base::clear();
	}

	MassivPtr& operator= (const MassivPtr& B) = delete;
	typename Base::iterator erase(typename Base::const_iterator i) = delete;
	typename Base::iterator erase(typename Base::const_iterator first, typename Base::const_iterator last) = delete;
};

#endif

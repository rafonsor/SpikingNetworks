#pragma once

#include <memory>
#include <stdexcept>
	
#define SN_EXCEPTION(name)\
	class name : public std::runtime_error {using std::runtime_error::runtime_error;};

#define SN_CLASS_POINTERS(cls)\
	public:\
		using Unique = std::unique_ptr<cls>;\
		using Shared = std::shared_ptr<cls>;\
		using Ptr = cls*;

#define SN_CLASS_CLONE_ABSTRACT(cls)\
	public:\
		cls::Unique clone() const\
		{\
			return std::unique_ptr<cls>(this->_clone());\
		}\
	private:\
		virtual cls* _clone() const = 0;

#define SN_CLASS_CLONE(cls)\
	public:\
		cls::Unique clone() const\
		{\
			return std::unique_ptr<cls>(this->_clone());\
		}\
	private:\
		virtual cls* _clone() const\
		{\
			return new cls(*this);\
		}
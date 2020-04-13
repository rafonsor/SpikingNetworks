#pragma once

#include <memory>
#include <stdexcept>

#define SN_CLASS_POINTERS(cls)\
	using Unique = std::unique_ptr<cls>;\
	using Shared = std::shared_ptr<cls>;\
	using Ptr = cls*;
	
#define SN_EXCEPTION(name)\
	class name : public std::runtime_error {using std::runtime_error::runtime_error;};
/****************************************************************************
 *
 *   Copyright (c) 2018 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/**
 * @file param.h
 *
 * C++ Parameter class
 */

#pragma once

#include "param_macros.h"
#include <float.h>
#include <math.h>

#include <parameters/px4_parameters.hpp>
#include <uORB/topics/parameter.h>
#include <uORB/topics/parameter_update.h>

/**
 * get the parameter handle from a parameter enum
 */
inline static constexpr param_t param_handle(px4::params p)
{
	return (param_t)p;
}




#define _DEFINE_SINGLE_PARAMETER(x) \
	do_not_explicitly_use_this_namespace::PAIR(x);

#define _CALL_UPDATE(x) \
	STRIP(x).update();

#define _SET_PARAMETER_UPDATE(x) \
	case STRIP(x).handle(): STRIP(x).setValue(parameter_update.changed_param); return;

// define the parameter update method, which will update all parameters.
// It is marked as 'final', so that wrong usages lead to a compile error (see below)
#define _DEFINE_PARAMETER_UPDATE_METHOD(...) \
	protected: \
	void updateParamsImpl() final { \
		APPLY_ALL(_CALL_UPDATE, __VA_ARGS__) \
	} \
	void updateParamsImpl(const parameter_update_s &parameter_update) { \
		switch(parameter_update.changed_param.index) { \
			APPLY_ALL(_SET_PARAMETER_UPDATE, __VA_ARGS__) \
		} \
	} \
	private:

// Define a list of parameters. This macro also creates code to update parameters.
// If you get a compile error like:
//   error: virtual function ‘virtual void <class>::updateParamsImpl()’
// It means you have a custom inheritance tree (at least one class with params that inherits from another
// class with params) and you need to use DEFINE_PARAMETERS_CUSTOM_PARENT() for **all** classes in
// that tree.
#define DEFINE_PARAMETERS(...) \
	APPLY_ALL(_DEFINE_SINGLE_PARAMETER, __VA_ARGS__) \
	_DEFINE_PARAMETER_UPDATE_METHOD(__VA_ARGS__)


#define _DEFINE_PARAMETER_UPDATE_METHOD_CUSTOM_PARENT(parent_class, ...) \
	protected: \
	void updateParamsImpl() override { \
		parent_class::updateParamsImpl(); \
		APPLY_ALL(_CALL_UPDATE, __VA_ARGS__) \
	} \
	void updateParamsImpl(const parameter_update_s &parameter_update) { \
		parent_class::updateParamsImpl(parameter_update); \
		switch(parameter_update.changed_param.index) { \
			APPLY_ALL(_SET_PARAMETER_UPDATE, __VA_ARGS__) \
		} \
	} \
	private:

#define DEFINE_PARAMETERS_CUSTOM_PARENT(parent_class, ...) \
	APPLY_ALL(_DEFINE_SINGLE_PARAMETER, __VA_ARGS__) \
	_DEFINE_PARAMETER_UPDATE_METHOD_CUSTOM_PARENT(parent_class, __VA_ARGS__)



// This namespace never needs to be used directly. Use the DEFINE_PARAMETERS_CUSTOM_PARENT and
// DEFINE_PARAMETERS macros instead (the Param classes don't depend on using the macro, the macro just
// makes sure that update() is automatically called).
namespace do_not_explicitly_use_this_namespace
{

template<typename T, px4::params p>
class Param
{
};

// We use partial template specialization for each param type. This is only supported for classes, not individual methods,
// which is why we have to repeat the whole class
template<px4::params p>
class Param<float, p>
{
public:
	// static type-check
	static_assert(px4::parameters_type[(int)p] == PARAM_TYPE_FLOAT, "parameter type must be float");

	Param()
	{
		update();
	}

	float get() const { return _val; }

	const float &reference() const { return _val; }

	/// Set and commit a new value. Returns true if the value changed.
	bool set(float val)
	{
		if (fabsf(val - _val) > FLT_EPSILON) {
			_val = val;
			return (param_set(handle(), &_val) == 0);
		}

		return false;
	}

	bool update() { return param_get(handle(), &_val) == 0; }

	void setValue(const parameter_s &parameter_update) { _val = parameter_update.float32_value; }

	static constexpr param_t handle() { return param_handle(p); }
private:
	float _val;
};

// external version
template<px4::params p>
class Param<float &, p>
{
public:
	// static type-check
	static_assert(px4::parameters_type[(int)p] == PARAM_TYPE_FLOAT, "parameter type must be float");

	Param(float &external_val)
		: _val(external_val)
	{
		update();
	}

	float get() const { return _val; }

	const float &reference() const { return _val; }

	/// Set and commit a new value. Returns true if the value changed.
	bool set(float val)
	{
		_val = val;
		return (param_set(handle(), &_val) == 0);
	}

	bool update() { return param_get(handle(), &_val) == 0; }

	void setValue(const parameter_s &parameter_update) { _val = parameter_update.float32_value; }

	static constexpr param_t handle() { return param_handle(p); }
private:
	float &_val;
};

template<px4::params p>
class Param<int32_t, p>
{
public:
	// static type-check
	static_assert(px4::parameters_type[(int)p] == PARAM_TYPE_INT32, "parameter type must be int32_t");

	Param()
	{
		update();
	}

	int32_t get() const { return _val; }

	const int32_t &reference() const { return _val; }

	/// Set and commit a new value. Returns true if the value changed.
	bool set(int32_t val)
	{
		_val = val;
		return (param_set(handle(), &_val) == 0);
	}

	bool update() { return param_get(handle(), &_val) == 0; }

	void setValue(const parameter_s &parameter_update) { _val = parameter_update.int32_value; }

	static constexpr param_t handle() { return param_handle(p); }
private:
	int32_t _val;
};

//external version
template<px4::params p>
class Param<int32_t &, p>
{
public:
	// static type-check
	static_assert(px4::parameters_type[(int)p] == PARAM_TYPE_INT32, "parameter type must be int32_t");

	Param(int32_t &external_val)
		: _val(external_val)
	{
		update();
	}

	int32_t get() const { return _val; }

	const int32_t &reference() const { return _val; }

	/// Store the parameter value to the parameter storage (@see param_set())
	bool set(int32_t val)
	{
		_val = val;
		return (param_set(handle(), &_val) == 0);
	}

	bool update() { return param_get(handle(), &_val) == 0; }

	void setValue(const parameter_s &parameter_update) { _val = parameter_update.int32_value; }

	static constexpr param_t handle() { return param_handle(p); }
private:
	int32_t &_val;
};

template<px4::params p>
class Param<bool, p>
{
public:
	// static type-check
	static_assert(px4::parameters_type[(int)p] == PARAM_TYPE_INT32, "parameter type must be int32_t");

	Param()
	{
		update();
	}

	bool get() const { return _val; }

	const bool &reference() const { return _val; }

	/// Set and commit a new value. Returns true if the value changed.
	bool set(bool val)
	{
		_val = val;
		int32_t value_int = (int32_t)_val;
		return (param_set(handle(), &value_int) == 0);
	}

	bool update()
	{
		int32_t value_int;
		int ret = param_get(handle(), &value_int);

		if (ret == 0) {
			_val = value_int != 0;
			return true;
		}

		return false;
	}

	void setValue(const parameter_s &parameter_update) { _val = parameter_update.int32_value; }

	static constexpr param_t handle() { return param_handle(p); }
private:
	bool _val;
};

template <px4::params p>
using ParamFloat = Param<float, p>;

template <px4::params p>
using ParamInt = Param<int32_t, p>;

template <px4::params p>
using ParamExtFloat = Param<float &, p>;

template <px4::params p>
using ParamExtInt = Param<int32_t &, p>;

template <px4::params p>
using ParamBool = Param<bool, p>;

} /* namespace do_not_explicitly_use_this_namespace */


// Raise an appropriate compile error if a Param class is used directly (just to simplify debugging)
template<px4::params p>
class ParamInt
{
	static_assert((int)p &&false, "Do not use this class directly, use the DEFINE_PARAMETERS macro instead");
};
template<px4::params p>
class ParamFloat
{
	static_assert((int)p &&false, "Do not use this class directly, use the DEFINE_PARAMETERS macro instead");
};

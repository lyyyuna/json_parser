#include "lyyjson.h"

namespace lyy
{
	/*
	* default JNULL
	*/
	JsonValue::JsonValue() : type(JsonType::JNULL) {}

	/* bool type */
	JsonValue::JsonValue(bool bool_value)
	{
		if (true == bool_value)
			this->type = JsonType::JTRUE;
		else
			this->type = JsonType::JFALSE;
	}

	/* double */
	JsonValue::JsonValue(double double_value) : type(JsonType::JNUMBER), number_value(double_value) {}

	/*
	* init context
	*/
	JsonContext::JsonContext(const char* json) : json(json) {}

	/*
	* get value type
	*/
	JsonType JsonValue::get_type()
	{
		return this->type;
	}

}
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

	/* string */
	JsonValue::JsonValue(Str string_value) : type(JsonType::JSTRING), string_value(string_value) {}

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

	/* get value */
	ValueRet JsonValue::get_value(double& number)
	{
		if (JsonType::JNUMBER == this->type)
		{
			number = this->number_value;
			return ValueRet::OK;
		} else {
			number = 0;
			return ValueRet::ERR;
		}
	}

	/* get string value */
	ValueRet JsonValue::get_value(JsonValue::Str& str)
	{
		if (JsonType::JSTRING == this->type)
		{
			str = this->string_value;
			return ValueRet::OK;
		}
		else {
			str = JsonValue::Str();
			return ValueRet::ERR;
		}
	}

}
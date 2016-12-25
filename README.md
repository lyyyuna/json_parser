# json_parser

又一个 json 解析器实现。

## 使用

### 自定义数据类型

* json 字符串，JsonValue::Str
* json 数组，JsonValue::Array
* json 对象，JsonValue::Object

### 例子

```cpp
#include <iostream>
#include "lyyjson.h"

using namespace lyy;

int main()
{
	const char* json = " { "
		"\"n\" : null , "
		"\"f\" : false , "
		"\"t\" : true , "
		"\"i\" : 123 , "
		"\"s\" : \"abc\", "
		"\"a\" : [ 1, 2, 3 ],"
		"\"o\" : { \"1\" : 12, \"2\" : 2, \"3\" : 3 }"
		" } ";
	ParseRet ret;
	auto v = JsonParser::parse(json, ret);
	if (ParseRet::PARSE_OK == ret)
	{
		if (JsonType::JOBJECT == v->get_type())
		{
			JsonValue::Object obj;
			v->get_value(obj);
			JsonValue::Str key;
			JsonParser::char2Str("i", key, 1);
			auto value = obj[key];
			double num;
			value->get_value(num);
			cout << num << endl;
		}
	}
	return 0;
}
```

## 简单介绍

这里主要参考了 MiloYip 的[从零开始的 JSON 库教程](https://github.com/miloyip/json-tutorial)。

可以参考 [json introduction](http://www.json.org/) 和 [ECMA-404](http://www.ecma-international.org/publications/files/ECMA-ST/ECMA-404.pdf)。

MiloYip 的教程是 c 语言实现的，内存分配全部由自己实现。我这里是 c++ 实现，所以便偷懒全改成了智能指针+标准数据结构。好处是相对直观，使用起来也不会有内存泄露，但数据不免多复制了几次，相对于 MiloYip 的实现，性能是会下降不少。

当然，这只是个练习，所以还有不少待改进之处：

* 因为直接用 std 的原因，多处数据重复复制。
* JsonValue 类型设计不足，如果是动态类型语言，就不会像我这样把所有东西全塞在一个类型里。如果用虚函数实现多态，在解析接口上也有很多不便。如果用 enum，Visual Stduio 2013 现在只支持 POD 类型的 enum。希望以后能有更好的封装方法。
* 接口用起来远不如 Python 的方便。
* ...

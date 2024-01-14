#pragma once

#define DECLARE_SINGLE(classname)			\
private:									\
	classname() = default					\
public:										\
	static classname* GetInstance()			\
	{										\
		static classname s_instance;		\
		return &s_instance;					\
	}

#define GET_SINGLE(classname)	classname::GetInstance()

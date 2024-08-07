#pragma once
#define THIS_CLASS_IS_STATIC_SINGLETON(TYPE)\
TYPE(const TYPE&) = delete;\
TYPE& operator=(const TYPE&) = delete;\
TYPE(TYPE&&) = delete;\
TYPE& operator=(TYPE&) = delete;\
friend class StaticSingleton<TYPE>;


template<typename T>
class StaticSingleton
{
public:
	static T& GetInstance()
	{
		static T instance;
		return instance;
	}

	//èâä˙âª
	virtual void Init(void) = 0;

protected:
	StaticSingleton() = default;
	~StaticSingleton() = default;

private:
	
};

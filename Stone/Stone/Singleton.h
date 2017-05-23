#ifndef					__SINGLETON_H__
#define					__SINGLETON_H__

template<typename T>
class Singleton
{
public:
	static T& getInstance()
	{
		static T s_instance;
		return s_instance;
	}

protected:
	Singleton() = default;
	~Singleton() = default;

private:
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
};

#endif
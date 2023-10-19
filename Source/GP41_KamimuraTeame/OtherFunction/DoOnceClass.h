#pragma once
class DoOnceClass
{
public:
	bool GetGo(){
		bool r = once;
		once = false;
		return r;
	}
	void Reset() { once = true; }

	void operator=(bool t) { once = t; }
	operator bool()const { return once; }
	bool operator==(bool t) { return once == t; }
private:
	bool once = true;
};

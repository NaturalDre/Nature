#ifndef DEBUG_H
#define DEBUG_H

#include <string>
class CDebug
{
protected:

public:
	CDebug(void);
	~CDebug(void);

	CDebug& operator<<(const std::string& str);
	
private:
};


#endif
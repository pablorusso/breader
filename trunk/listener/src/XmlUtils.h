#ifndef XmlUtils_H
#define	XmlUtils_H

#include <string>
#include <vector>

using namespace std;

class XmlUtils
{
	public:
		static string xmlEncode(const string& raw);
		static string xmlDecode(const string& encoded);
};

#endif

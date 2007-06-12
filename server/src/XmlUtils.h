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

		static string xmlEncode(const unsigned int& raw);
		static string xmlDecode(const unsigned int& encoded);
		static string xmlEncode(const unsigned short& raw);
		static string xmlDecode(const unsigned short& encoded);

		static string uintToStr( const unsigned int &value );
		static string ushortToStr( const unsigned short &value );
		static unsigned int strTouint( const string &value );
		static unsigned short strToushort( const string &value );
};

#endif

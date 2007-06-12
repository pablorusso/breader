#include "General.h"

std::string General::getDataPath()
{
	std::string dataPath( "" );
	dataPath.append( getenv("HOME") );
	dataPath.append( "/.breader/" );
	return dataPath;
}

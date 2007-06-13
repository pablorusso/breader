#include "General.h"

std::string General::getDataPath()
{
	std::string dataPath(getenv("HOME"));
	dataPath.append( "/.breader/" );
	return dataPath;
}

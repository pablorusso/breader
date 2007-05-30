#include "XmlUtils.h"

static const char  AMP = '&';
static const char  rawEntity[] = { '<',   '>',   '&',    '\'',    '\"',    0 };
static const char* xmlEntity[] = { "lt;", "gt;", "amp;", "apos;", "quot;", 0 };
static const int   xmlEntLen[] = { 3,     3,     4,      5,       5 };

string XmlUtils::xmlEncode(const string& raw)
{
	std::string::size_type iRep = raw.find_first_of(rawEntity);
  	if ( iRep == std::string::npos )
    	return raw;

  	std::string encoded(raw, 0, iRep);
  	std::string::size_type iSize = raw.size();

  	while ( iRep != iSize )
	{
    	int iEntity;
    	for ( iEntity=0; rawEntity[iEntity] != 0; ++iEntity )
      	if ( raw[iRep] == rawEntity[iEntity] )
      	{
        	encoded += AMP;
        	encoded += xmlEntity[iEntity];
        	break;
      	}
    	if (rawEntity[iEntity] == 0)
      		encoded += raw[iRep];
    	++iRep;
  	}
  	return encoded;
}

// Replace xml-encoded entities with the raw text equivalents.
string XmlUtils::xmlDecode(const string& encoded)
{
  std::string::size_type iAmp = encoded.find(AMP);
  if (iAmp == std::string::npos)
    return encoded;

  std::string decoded(encoded, 0, iAmp);
  std::string::size_type iSize = encoded.size();
  decoded.reserve(iSize);

  const char* ens = encoded.c_str();
  while (iAmp != iSize) {
    if (encoded[iAmp] == AMP && iAmp+1 < iSize) {
      int iEntity;
      for (iEntity=0; xmlEntity[iEntity] != 0; ++iEntity)
	//if (encoded.compare(iAmp+1, xmlEntLen[iEntity], xmlEntity[iEntity]) == 0)
	if (strncmp(ens+iAmp+1, xmlEntity[iEntity], xmlEntLen[iEntity]) == 0)
        {
          decoded += rawEntity[iEntity];
          iAmp += xmlEntLen[iEntity]+1;
          break;
        }
      if (xmlEntity[iEntity] == 0)    // unrecognized sequence
        decoded += encoded[iAmp++];

    } else {
      decoded += encoded[iAmp++];
    }
  }

  return decoded;
}

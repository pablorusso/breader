#include "CGIClass.h"
#include <sstream>
#include <iostream>

CGIClass::CGIClass()
{
}

CGIClass::~CGIClass()
{
}

void CGIClass::Load( const string &data )
{
	buildMap( data );
}

string CGIClass::GetValue( const string &name )
{
	return url_decode( _mapNameValue[ name ] );
}

int CGIClass::h2c(char hi, char lo)
{
	std::string s;
    int value = -1;
    s += hi;
    s += lo;
    std::istringstream is(s);
    is >> std::hex >> value;
    return value;
}

string CGIClass::url_decode(const string& s)
{
	string result;
	std::istringstream iss(s);
	char c;

	for (iss >> c;!iss.eof();iss >> c)
	{
    	if ('+' == c)
		{
        	result += ' ';
		}
		else
		{
			if ('%' == c)
			{
        		char c1 = 0, c2 = 0;
            	iss >> c1;
            	iss >> c2;
            	if (!iss.eof())
				{
            		int r = h2c(c1,c2);
                	if (r != -1)
	                	result += static_cast<char>(r);
				}
			}
			else
			{
            	result += c;
			}
		}
	}

	return result;
}

int CGIClass::splitString(const string& input, const string& delimiter, vector< string >& results, bool includeEmpties )
{
    size_t iPos = 0;
    size_t newPos = string::npos;
    size_t sizeS2 = delimiter.size();
    size_t isize = input.size();

    if ( ( isize == 0 ) || ( sizeS2 == 0 ) )
    {
		if ( includeEmpties ) results.push_back( input );
        return 0;
    }

    vector< size_t > positions;

    newPos = input.find (delimiter, 0);
    if( newPos == string::npos )
    {
		if ( includeEmpties ) results.push_back( input );
        return 0;
    }

    int numFound = 0;
    while( newPos != string::npos )
    {
        numFound++;
        positions.push_back(newPos);
        iPos = newPos;
        newPos = input.find (delimiter, iPos+sizeS2);
    }

    if( numFound == 0 )
    {
		if ( includeEmpties ) results.push_back( input );
        return 0;
    }

    for( size_t i=0; i <= positions.size(); ++i )
    {
        string s("");
        if( i == 0 )
        {
            s = input.substr( i, positions[i] );
        }
        size_t offset = positions[i-1] + sizeS2;
        if( offset < isize )
        {
            if( i == positions.size() )
            {
                s = input.substr(offset);
            }
            else if( i > 0 )
            {
                s = input.substr( positions[i-1] + sizeS2,
                      positions[i] - positions[i-1] - sizeS2 );
            }
        }
        if( includeEmpties || ( s.size() > 0 ) )
        {
            results.push_back(s);
        }
    }
    return numFound;
}

void CGIClass::buildMap( string buff )
{
	std::string lastName;
	string lastValue;

	vector< string > query_string;
	splitString( buff, "&", query_string, true );

	vector< string >::iterator queryStrIt;
	for( queryStrIt = query_string.begin(); queryStrIt != query_string.end(); queryStrIt++ )
	{
		string singleParam = *( queryStrIt );
		vector< string > pairNameValue;
		splitString( singleParam, "=", pairNameValue, true );

		vector< string >::iterator pairNameIt = pairNameValue.begin();
		if ( pairNameIt != pairNameValue.end() )
		{
			string name = *( pairNameIt );
			pairNameIt++;
			if ( pairNameIt != pairNameValue.end() )
			{
				string value = *( pairNameIt );
				_mapNameValue[ name ] = value;
			}
		}
	}
}


#include <iostream>
#include "Action.h"

Action::Action()
{
}

Action::~Action()
{
	this->emptyParams();
}

Values *Action::getParamValue( string paramName )
{
	Values *value = parameterList[ paramName ];
	if ( value == NULL )
		throw string( "[listener] - El parametro " + paramName + " no se encuentra seteado" );
	return value;
}

int Action::splitString(const string& input, const string& delimiter, Values& results, bool includeEmpties )
{
	string::size_type delSize = delimiter.size();
	string::size_type inputSize = input.size();
	if ( ( inputSize == 0 ) || ( delSize == 0 ) )
	{
		if ( includeEmpties )
		{
			results.push_back( input );
		}
		return 0;
	}

	string myInput;
	// Si lo primero no es un delimiter
	if (input.find(delimiter,0) != 0)
	{
		myInput.append(delimiter);
	}
	myInput.append(input);

//	cout << "myInput: " << myInput << endl;
//	cout << "delimiter: " << delimiter << endl;
	string::size_type lastPos = 0;
	string::size_type pos;
	int numFound = 1;

	while (lastPos != string::npos)
	{
		++numFound;
		pos = myInput.find(delimiter, lastPos+delSize);
// 		cout << "lastPos: " << lastPos << endl;
// 		cout << "pos: " << pos << endl;
		// Found a token, add it to the vector.
		string tmp( myInput.substr(lastPos+delSize, pos-(lastPos+delSize)) );
		if (tmp.size() > 0 || includeEmpties)
		{
			results.push_back(tmp);
		}
		lastPos = pos;
	}
	return --numFound;
}

/* CODIGO DE PABLO 
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
*/

void Action::parseParams( string params )
{
	string delim1 = "|||";
	Values paramListWithName;
	splitString( params, delim1, paramListWithName, true );

	Values::iterator itParamList;
	for( itParamList = paramListWithName.begin(); itParamList != paramListWithName.end(); itParamList++ )
	{
		string delim2 = "||#";
		Values nameAndValue;
		string nameAndValudS = *itParamList;
		splitString( nameAndValudS, delim2, nameAndValue, true );

		Values::iterator itNameAndValue = nameAndValue.begin();
		while( itNameAndValue != nameAndValue.end() )
		{
			string name = static_cast<string>( *itNameAndValue );
			itNameAndValue++;
			if ( itNameAndValue != nameAndValue.end() )
			{
				string delim3 = "||,";

				Values *valuesOnly = new Values();
				string valuesStr = *itNameAndValue;
				splitString( valuesStr, delim3, *valuesOnly, true );
				parameterList[ name ] = valuesOnly;
				itNameAndValue++;
			}
		}
	}
}

string Action::ProcessAction( string params )
{
	emptyParams();
	parseParams( params );
	return this->processAction();
}

void Action::emptyParams()
{
	// Destruyo todos los objetos del map
	ParameterList::iterator parametersIt;
	parametersIt = parameterList.begin( );
	while( parametersIt != parameterList.end() )
	{
		delete parametersIt->second;
		parameterList.erase( parametersIt );
		parametersIt = parameterList.begin();
	}
}

string Action::GetName()
{
	return this->getName();
}

vector<string> Action::GetNeededParams()
{
	return this->getNeededParams();
}



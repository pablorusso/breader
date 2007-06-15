#ifndef GENERAL
#define GENERAL


/*****************************************************************************
Imposiciones de Damian:
----------------------
Nro máximo de categorías: >=1
Nro máximo de categorías: múltiplo de 8.
sizeof(t_idart) = sizeof(t_offset)
*****************************************************************************
Imposiciones de Sergio:
----------------------
El idcat -1 no se usa
*****************************************************************************/


#include <queue>
#include <string>

#define NOM_CAT_MAX_LEN 20
//!< La longitud maxima de los nombres de categorias
#define IDCAT_FAV 1
//!< El id de la categoria correspondiente a "favorito" o "starred"
//!< (ojo que el cero no se puede usar)

typedef unsigned int t_timestamp; //!< tipo formato timestamp (unix time)
typedef unsigned int t_offset; //!< tipo ID para utilizar en los tipos offset
typedef unsigned short t_idfeed; //!< tipo ID para utilizar en los tipos idfeed
typedef unsigned short t_idcat; //!< tipo ID para utilizar en los tipos idcat
typedef unsigned int t_idart; //!< tipo ID para utilizar en los tipos idcat
typedef unsigned short t_freebytes; //!< para los bytes libres en registros
typedef unsigned int t_uint; //!< un tipo entero sin signo generico
typedef unsigned int t_quantity; //!< tipo para utilizar en los tipos cantidad
typedef std::queue<t_idcat> t_queue_idcat; //!< tipo contenedor de idcat
typedef unsigned int t_frecuency; //!< un tipo frecuencia
typedef unsigned short t_cantReg; //!< un tipo csntidad de registros
/* */
typedef std::queue<t_idart> t_cola_idart; //!< el tipo cola de id de articulos
typedef std::queue<t_idfeed> t_cola_idfeeds; //!< el tipo colaidfeeds
typedef std::vector<t_idart> t_c_cant_idcat; //!< el tipo contenedor de
                                             //!< cantidades de idcat
typedef std::queue<std::string> t_word_cont; //!< el tipo contenedor de palabras


class General
{
	public:
		static std::string getDataPath();
};

#endif

#ifndef GENERAL
#define GENERAL

#include <queue>
#include <set>

#define IDCAT_FAV 1
//!< Es el id de la categoria correspondiente a "favorito" o "starred"
//!< (ojo que el cero no se puede usar)


typedef unsigned int t_timestamp; //!< tipo formato timestamp (unix time)
typedef unsigned int t_offset; //!< tipo ID para utilizar en los tipos offset
typedef unsigned short t_idfeed; //!< tipo ID para utilizar en los tipos idfeed
typedef unsigned short t_idcat; //!< tipo ID para utilizar en los tipos idcat
typedef unsigned int t_idart; //!< tipo ID para utilizar en los tipos idcat
typedef unsigned short t_freebytes; //!< para los bytes libres en registros
typedef unsigned int t_uint; //!< un tipo entero sin signo generico
typedef unsigned int t_quantity; //!< tipo para utilizar en los tipos cantidad
typedef std::set<t_idcat> t_set_idcat; //!< tipo contenedor de idcat // TODO pinchara??

typedef std::queue<t_idart> t_cola_idart; //!< el tipo cola de id de articulos
typedef std::queue<t_idfeed> t_cola_idfeeds; //!< el tipo colaidfeeds

#endif

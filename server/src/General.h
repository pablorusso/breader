#ifndef GENERAL
#define GENERAL

/*****************************************************************************
Imposiciones de Damian:
----------------------
Nro máximo de categorías: >=1
Nro máximo de categorías: múltiplo de 8.
sizeof(t_idart) = sizeof(t_offset)
El idart -1 (casteado a unsigned) no se usa
*****************************************************************************
Imposiciones de Sergio:
----------------------
El idcat -1 (casteado a unsigned) no se usa
*****************************************************************************
Imposiciones generales
----------------------
El IDCAT_FAV tiene que ser el cero, para que esté al principio y no haya que
agregar basura en el medio...
******************************************************************************/

#include <queue>
#include <string>
#include <map>

#define UMBRAL_BCLAS 1.0
//!< Vos calculas las prob para cada categoria, bueno si superan ese umbral el
//!< articulo va a ser categorizado con ese ID
#define UMBRAL_FH 0.8
//!< Es el umbral debajo del cual el feedHandler se reestructurara al pedir
//!< una reestructuracion
#define UMBRAL_MW 0.8
//!< Es el umbral debajo del cual el ManagerWord se reestructurara al pedir
//!< una reestructuracion
#define SW_MIN_LEN 4
//!< La minima longitud de las palabras de un articulo, para que no sean
//!< consideradas stopwords
#define NOM_CAT_MAX_LEN 20
//!< La longitud maxima de los nombres de categorias
#define IDCAT_FAV 0
//!< El id de la categoria correspondiente a "favorito" o "starred"
//!< (ojo que si no es la cero no va a funcionar)
#define MAX_MEMORY_TREE 4000.0
//!<Cantidad de memoria maxima que puede hacer uso el arbol 4MB

typedef unsigned int t_timestamp; //!< tipo formato timestamp (unix time)
typedef unsigned int t_offset; //!< tipo ID para utilizar en los tipos offset
typedef unsigned short t_idfeed; //!< tipo ID para utilizar en los tipos idfeed
typedef unsigned short t_idcat; //!< tipo ID para utilizar en los tipos idcat
typedef unsigned int t_idart; //!< tipo ID para utilizar en los tipos idcat
typedef unsigned short t_freebytes; //!< para los bytes libres en registros
typedef unsigned int t_uint; //!< un tipo entero sin signo generico
typedef unsigned int t_quantity; //!< tipo para utilizar en los tipos cantidad
typedef std::queue<t_idcat> t_queue_idcat; //!< tipo contenedor de idcat
typedef unsigned int t_frequency; //!< un tipo frecuencia
typedef int t_diferencia; //!< un tipo diferencia
typedef unsigned short t_cantReg; //!< un tipo cantidad de registros
typedef float t_usedFactor; //!< un tipo factor de ocupados

typedef struct {
	t_frequency cantTrue; //!< Es la cantidad de veces que existe esa palabra 
                           //!< en una categoria porque el usuario clasifico un 
						   //!< artiulo que la contenia, o porque el usuario 
                           //!< acepto la clasificacion automatica del sistema.

	t_frequency cantFalse;//!< Es la cantidad de veces que el sistema clasifico
                           //!< mal (corregido por el usuario) al articulo que 
                           //!< contenia la palabra.
} tFrecuencias;

typedef struct {
	t_diferencia cantTrue;
	t_diferencia cantFalse;
} t_diferencias;

typedef std::queue<t_idart> t_cola_idart; //!< el tipo cola de id de articulos
typedef std::queue<t_idfeed> t_cola_idfeeds; //!< el tipo colaidfeeds
typedef std::vector<t_idart> t_c_cant_idcat; //!< el tipo contenedor de
                                             //!< cantidades de idcat
typedef std::map< std::string , t_diferencias > t_word_cont;
//!< el tipo contenedor de palabras. tiene la palabra y la cantidad de veces que
//!< aparece en el articulo

class General
{
	public:
		/**
		 * Devuelve el directorio donde se guardaran todos los archivos
		 * correspondientes al breader
		 * @return el directorio donde se guardaran todos los archivos
		 */
		static std::string getDataPath();
};

#endif

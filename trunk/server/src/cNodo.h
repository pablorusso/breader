

#if !defined NODO_ARBOL
#define NODO_ARBOL

#include <iostream>
#include "General.h"

/**Valor que corresponde a un bloque vacio*/
#define NULL_BL 0
/**Cantidad de elementos en los nodos, la cantidad minima de elementos es tres.*/
#define CANT_ELEM_X_NODO 24

/*****************************************************************************/
template < class ELEM >
class cNodoDisco{
public:

	t_offset nroNodo; //!< Numero de nodo, es unico y permite identificar la posicion
	                      //!< donde se almacena en disco.
	t_offset vecHijos[CANT_ELEM_X_NODO+1]; //!< Vector de nodos que contiene los numeros
                                               //!< que identifican a los nodos hijos.
	ELEM elem[CANT_ELEM_X_NODO]; //!< Vector de Elementos del nodo
/*---------------------------------------------------------------------------*/
	/**
	 * Constructor. Inicializa atributos con valores por default
	 */
	cNodoDisco(){
		nroNodo=0;
		for(t_offset i=0 ; i < CANT_ELEM_X_NODO+1 ; i++){
			vecHijos[i]=NULL_BL;
		}
	}
/*---------------------------------------------------------------------------*/	
  /**Escribe los datos sobre el buffer de salida.
	* @param salida Buffer sobre el que se escribe.
	* @param dato Elemento que se quiere guardar
	*/
	friend std::ofstream &operator<<(std::ofstream &salida, cNodoDisco<ELEM> &dat){
		salida.write(reinterpret_cast<char *>(&(dat.nroNodo)),sizeof(t_offset));

		for(t_uint i=0 ; i < CANT_ELEM_X_NODO;i++){
		   salida.write(reinterpret_cast<char *>(&(dat.vecHijos[i])),sizeof(t_offset));
		   salida << dat.elem[i];

		}
		salida.write(reinterpret_cast<char *>(&(dat.vecHijos[CANT_ELEM_X_NODO])),sizeof(t_offset));

		return salida;
	}
/*---------------------------------------------------------------------------*/
  /**Obtiene los datos del buffer de entrada.
	* @param entrada Buffer sobre el que se leen los datos.
	* @param dato Elemento que se recupera.
	*/
	friend std::ifstream &operator>>(std::ifstream &entrada, cNodoDisco<ELEM> &dat){
		entrada.read(reinterpret_cast<char *>(&(dat.nroNodo)),sizeof(t_offset));

		for(t_uint i=0 ; i < CANT_ELEM_X_NODO;i++){
		   entrada.read(reinterpret_cast<char *>(&(dat.vecHijos[i])),sizeof(t_offset));
		   entrada >> dat.elem[i];
		}

		entrada.read(reinterpret_cast<char *>(&(dat.vecHijos[CANT_ELEM_X_NODO])),sizeof(t_offset));
		return entrada;
	}
/*---------------------------------------------------------------------------*/
  /** Retorna el tamaño del dato en disco.
	*/
	static unsigned int sizeofNodoDisco(){
		return (sizeof(t_offset) + sizeof(t_offset) * (CANT_ELEM_X_NODO+1) + sizeof(ELEM)*CANT_ELEM_X_NODO);
	}
};
/*****************************************************************************/

template < class CONT, class ELEM > class cArbol;

template < class CONT, class ELEM >
class cNodo {
   /**
	* Constructor
	* @param nroNodo Identificador al nodo ante los demas.
	*/
   cNodo(t_offset nroNodo);
   /**
	* @param nodo Nodo almacenado en disco.
	* @param padre Puntero al padre del nodo.
	*/
   cNodo(cNodoDisco<ELEM> nodo,cNodo* padre);

   /**
	* Destructor
	*/
   ~cNodo();

private:
   t_uint cantClavesUsadas; //!< Claves usadas en el nodo 
   CONT *dato;           //!< Array de claves de elementos del nodo 
   cNodo **ptr;          //!< Array de punteros a cNodo 
   t_offset *ptrNroHijo; //!< Array de int que hace referencia al nodo en
                             //!< que apunta en disco
   cNodo *padre;         //!< Puntero al nodo padre en memoria
   t_offset nroNodo; //!< Identifica al nodo ante los demas

   cNodoDisco<ELEM> getNodoDisco();/**Devuelve el nodo en su formato para
                                     *ser almacenado*/

   static t_uint sizeofNodo();

 /**Declaro a cArbol con los derechos suficientes para acceder
   *libremente a esta clase*/
   friend class cArbol<CONT,ELEM> ;

};

/* -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*- */
/* Constructor: Inicializa el nodo en estado vacio
  Parametro:
			int nroNodo: nmero de nodo
*/
template < class CONT, class ELEM >
cNodo< CONT, ELEM >::cNodo(t_offset nroNodo){
   dato = new CONT[CANT_ELEM_X_NODO];
   ptr = new cNodo*[CANT_ELEM_X_NODO+1];
   ptrNroHijo = new t_offset[CANT_ELEM_X_NODO+1];

   for(t_offset i=0; i <= CANT_ELEM_X_NODO ; i++){
	  ptrNroHijo[i]=NULL_BL;
	  ptr[i]=NULL;
   }

   cantClavesUsadas = 0;
   padre = NULL;
   this->nroNodo=nroNodo;
}
/*--------------------------------------------------------------------------*/
/*Constructor: Inicializo el nodo con el nodo del disco
  Parametro:
			tNodoDisco nodoDisk: Nodo almacenado en el disco
			cNodo* padre: Padre del nodo
			int padreEnDisco: Numero del padre del nodo almacenado en disco.
*/
template < class CONT, class ELEM >
cNodo<CONT,ELEM>::cNodo(cNodoDisco<ELEM> nodoDisk,cNodo* padre){
   dato = new CONT[CANT_ELEM_X_NODO];
   ptr = new cNodo*[CANT_ELEM_X_NODO+1];
   ptrNroHijo = new t_offset[CANT_ELEM_X_NODO+1];

	this->padre=padre;
	this->nroNodo=nodoDisk.nroNodo;
	cantClavesUsadas = 0;

	for(t_offset i=0; i < CANT_ELEM_X_NODO ; i++){
			dato[i]= CONT(nodoDisk.elem[i]);
			ptr[i]=NULL;
			ptrNroHijo[i] = nodoDisk.vecHijos[i];
			if(!dato[i].isEmpty())
				cantClavesUsadas++;
	}

	ptr[CANT_ELEM_X_NODO]=NULL;
	ptrNroHijo[CANT_ELEM_X_NODO] = nodoDisk.vecHijos[CANT_ELEM_X_NODO];
}

/*---------------------------------------------------------------------------*/
/*Destructor*/
template < class CONT, class ELEM >
cNodo< CONT, ELEM >::~cNodo(){
   delete[] dato;
   delete[] ptr;
   delete[] ptrNroHijo;
}
/*---------------------------------------------------------------------------*/
template < class CONT, class ELEM >
cNodoDisco<ELEM> cNodo< CONT, ELEM >::getNodoDisco(){
	cNodoDisco<ELEM> nodoDisk;

	nodoDisk.nroNodo=nroNodo;
	for(t_uint i=0 ; i < cantClavesUsadas ; i++){
		nodoDisk.elem[i]=dato[i].getDato();
		nodoDisk.vecHijos[i]=ptrNroHijo[i];
	}
	nodoDisk.vecHijos[cantClavesUsadas]=ptrNroHijo[cantClavesUsadas];

	return nodoDisk;
}
/*---------------------------------------------------------------------------*/
template < class CONT, class ELEM >
t_uint cNodo< CONT, ELEM >::sizeofNodo(){
	t_uint tamano =  CANT_ELEM_X_NODO * sizeof(CONT) +
	                 (CANT_ELEM_X_NODO+1)*(sizeof(cNodo<CONT,ELEM>*) + 
                         sizeof(t_uint)) * (CANT_ELEM_X_NODO+1) +
                         sizeof(cNodo<CONT,ELEM>);

    return tamano;

}
/*****************************************************************************/

#endif



#if !defined ARBOL
#define ARBOL

#include "cNodo.h"
#include "General.h"
#include "ExceptionArbol.h"
#include <string>
#include <cmath>
#include <fstream>
#include <list>


/**Header del archivo que contiene a los nodos del arbol en disco.*/
class cHeaderArbol{
public:
	t_uint nroRaiz; //!< Numero del nodo Raiz.
	t_uint cantNodos;//!< Cantidad de nodos alamcenados.

  /** Constructor.
	*/
	cHeaderArbol(){
		nroRaiz=0; 
		cantNodos=0;
	}

  /**Escribe los datos sobre el buffer de salida.
	* @param salida Buffer sobre el que se escribe.
	* @param dato Elemento que se quiere guardar
	*/
	friend std::ofstream &operator<<(std::ofstream &salida, cHeaderArbol &header){
		salida.write(reinterpret_cast<char *> (&(header.nroRaiz)) , sizeof(t_uint));
    	salida.write(reinterpret_cast<char *> (&(header.cantNodos)), sizeof(t_uint));
		return salida;
	}

  /**Obtiene los datos del buffer de entrada.
	* @param entrada Buffer sobre el que se leen los datos.
	* @param dato Elemento que se recupera.
	*/
	friend std::ifstream &operator>>(std::ifstream &entrada, cHeaderArbol &header){	
		entrada.read(reinterpret_cast<char *> (&(header.nroRaiz)), sizeof(t_uint));
		entrada.read(reinterpret_cast<char *> (&(header.cantNodos)), sizeof(t_uint));
		return entrada;
	}

  /** Retorna el tamaño del header en disco.
	*/
	static unsigned int sizeofHeader(){
		return (sizeof(t_uint)*2);
	}
};

template < class CONT, class ELEM>
class cArbol{

public:

  /**
	* @param maxOcup Cantidad maxima de memoria que se quiere usar en KB.
	* @throw ExceptioTree  cArbol::ERROR_MMI si la memoria maxima es insuficiente.
    */
   cArbol(double maxOcup);

	/**
	 * Destructor
	 */
	~cArbol();

 /**Busca a un elemento deltro del arbol.
   * @param elem Palabra que se quiere buscar en el arbol
   * @return Devuelve si se encontro o no la palabra y en caso de haber sido encontrada guarda
   * en elem los datos correspondientes.
   * @throw ExceptioTree cArbol::ERROR_NEA no se ha creado el arbol aun.
   */
   bool buscar(CONT &elem);

 /**Inserta un elemento dentro del arbol si este no existe
   * @param  dato : elemento a insertar.
   * @throw ExceptioTree cArbol::ERROR_NEA no se ha creado el arbol aun.
   * @throw ExceptioTree cArbol::ELEM_FOUND el elemento ya se encuentra en el arbol.
   */
   void insertar(CONT dato);

 /**Crea un arbol en disco
   * @param nameFile Nombre del archivo donde se almacenara el arbol.
   * @throw ExceptioTree cArbol::ERROR_NEA no se ha podido crear el arbol.
   */
   void crearArbol(std::string nameFile);

 /**Recupera un arbol del disco
   * @param nameFile Nombre del archivo donde esta el arbol.
   * @throw ExceptioTree cArbol::ERROR_FILE_NOT_FOUND no se ha encontrado el archivo.
   */
   void loadArbol(std::string nameFile);

 /**Salva un arbol en disco
   * @throw ExceptioTree cArbol::ERROR_NEA no se ha creado el arbol aun.
   * @throw ExceptioTree cArbol::ERROR_SAVE_TREE error al salvar el arbol.
   */
   void saveArbol();

/** Borra el arbol del disco. Debe estar cerrado
  * @throw ExceptioTree cArbol::ERROR_DT error al destruir el arbol.
  */
   void destroy();

 /**Borra el arbol de la memoria y lo salva en disco*/
   void closeArbol();

/*------------DESPUES SACAR------------------------------*/
   //TODO: despues sacar
  // void mostrar(); /*Muestra el arbol*/

/*------------------------------------------------------*/

private:

   cNodo<CONT,ELEM> *raiz;     //!< Punteo a la raiz del arbol
   cNodo<CONT,ELEM> *corriente;//!< Puntero al nodo actual del arbol
   std::string nameFile; //!< Nombre del archivo donde se almacena el arbol
   cHeaderArbol header; //!< Header del archivo donde se guarda el arbol
   bool isCreado;       //!< Si esta o no creado el arbol
   t_uint limitLevel;     //!< Nivel hasta donde se carga el arbol en memoria
   
 /**Agrega un elemento a un nodo que tiene espacio suficiente
   * @param ptr Puntero al nodo que se quiere inicializar.
   * @param elem Elemento del nodo
   * @param ptrHijo1 Hijo izquierdo del elemento del nodo
   * @param ptrHijo2 Hijo derecho del elemento del nodo
   */
   void armarNodo(cNodo<CONT,ELEM> * ptr , CONT &elem,cNodo<CONT,ELEM> *ptrHijo1, cNodo<CONT,ELEM> *ptrHijo2);

 /** Realiza una busqueda binaria para encontrar al elemento o a donde deberia
   * ir dentro del nodo.
   * @param ptr : puntero a la raiz del arbol o subarbol desde donde se quiere
   *              empezar a buscar.
   * @param elem : elemento que se esta buscando.
   * @param centro : posicion dentro del nodo.
   * @return true : si el elemento fue encontrado.
   *         false: si no fue encontrado y se posiciona el corriente en la hoja
   *                donde deberia estar y a centro en la posicion en la que debe
   *                tmb estar dentro del nodo.
   */
   bool buscarPos(cNodo<CONT,ELEM> *ptr, const CONT &elem, t_uint &centro);

 /**Busca un dato dentro del arbol retornando la posicion dentro del nodo y
   *con el corriente posicionado en el nodo.
   * @param elem : identificacion del elemento a buscar.
   * @throw ExceptioTree cArbol::ERROR_NEA no se ha creado el arbol aun.
   */
   bool buscarLugar(CONT &elem);

 /** Libera la memoria asignada a los nodos del arbol de forma recursiva.
   * @param ptr : nodo a liberar.   
   */
   void borrarNodo(cNodo<CONT,ELEM> *ptr);

 /**Guarda el arbol en disco en forma recursiva
   * @param outputFile: archivo donde se guardan los nodos.
   * @param nodo: nodo que se va almacenar.
   */
   void guardarRec( std::ofstream &outputFile, cNodo<CONT,ELEM> *nodo);

 /**Carga los nodos almacenados en disco en memoria de forma recursiva
   * @param inputFile: archivo que contiene los nodos.
   * @param padre : nodo padre que se le debe asignar al nodo.
   * @return cNodo*: padre del nodo.
   */
   cNodo<CONT,ELEM>* cargarRec(t_uint nroNodo,cNodo<CONT,ELEM>* padre, t_uint limit);

 /**Carga un nodo en particular
   * @param nroNodo: nmero de nodo a cargar
   * @param padre: padre del nodo a cargar
   * @throw ExceptioTree cArbol::ERROR_FILE_NOT_FOUND no se ha encontrado el archivo.
   * @return Nodo que se leyo del disco.
   */
   cNodo<CONT,ELEM>* cargarNodo(t_uint nroNodo,cNodo<CONT,ELEM>* padre);

  /**Guarda en disco un nodo que fue modificado.
	* @param nodo: Nodo que se va a salvar.
	* @throw ExceptioTree cArbol::ERROR_FILE_NOT_FOUND no se ha encontrado el archivo.
	*/
   void salvarNodo(cNodo<CONT,ELEM> *nodo);

  /**Guarda y libera la memoria de un subarbol.
	* @param nodo: Nodo que se va a salvar.
	*/
   void salvarSubArbol(cNodo<CONT,ELEM> *nodo, t_uint alt=0);

 /*------------DESPUES SACAR------------------------------*/
  // TODO: despues sacar

 //  void ver(cNodo<CONT,ELEM> *ptr); /*Muestra el arbol*/
/*------------------------------------------------------*/

};

/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
/*
Constructor
 Parametros:
   	    maxOcup : Cantidad de memoria que se quiere usar como maximo.
 */

template < class CONT, class ELEM >
cArbol< CONT, ELEM >::cArbol(double maxOcup){
	raiz=NULL;
	corriente=NULL;
	isCreado=false;
	header.cantNodos=0;
	header.nroRaiz=0;

	double tamNodo = (double) cNodo<CONT,ELEM>::sizeofNodo();

	/*CANT_ELEM_X_NODO * sizeof(CONT) + (CANT_ELEM_X_NODO+1)*(sizeof(cNodo<CONT,ELEM>*) + 
          sizeof(t_uint))+ sizeof(cNodo<CONT,ELEM>); */

	tamNodo= tamNodo/1024;
	t_uint cantNodos = (t_uint) (maxOcup / tamNodo);
	t_uint mult=1,nro=1,suma=1;

	if(cantNodos < 1)
		throw ExceptionTree(ERROR_MMI);

	this->limitLevel=0;
	while(suma < cantNodos){
		mult=1;
		for(t_uint i=0 ; i<nro ; i++){
			mult*=(CANT_ELEM_X_NODO+1);
		}
		nro++;
		suma+=mult;
		this->limitLevel++;
	}

	if(suma > cantNodos)
		this->limitLevel--;
}

/*---------------------------------------------------------------------------*/
/*
   Destructor: libera la memoria asignada en los nodos.
*/
template < class CONT, class ELEM >
cArbol< CONT, ELEM >::~cArbol(){
	if(raiz!=NULL)
	   borrarNodo(raiz);
}

/*---------------------------------------------------------------------------*/
/*
Realiza una bsqueda binaria para encontrar al elemento o a donde deberia
ir dentro del nodo

  Parametros:
		cNodo *ptr : puntero al raiz del arbol o subarbol desde donde
                             se quiere empezar a buscar.
		ELEM &elem : elemento que se esta buscando
		t_uint &centro : posicion dentro del nodo
  Retorno:
		true : si el elemento fue encontrado
		false: si no fue encontrado y se posiciona el corriente en la hoja
		       donde deberia estar y a centro en la posicion en la que debe
		       tmb estar dentro del nodo.
*/
template < class CONT, class ELEM >
bool cArbol< CONT, ELEM >::buscarPos(cNodo<CONT,ELEM> * ptr, const CONT &elem, t_uint &centro){
	//Deben ser int
	int primero=0,ultimo = ptr->cantClavesUsadas-1;
	centro=ultimo/2;

	if(ptr->dato[ultimo] == elem){
		centro=ultimo;
		return true;
	}

	while(primero <= ultimo){

		if(ptr->dato[centro] == elem){
			return true;

		}else if( elem < ptr->dato[centro]){
		     	  ultimo=centro-1;

		}else{
			  primero=centro+1;
		}

		centro=(primero+ultimo+1) / 2;
	}

/*  BUSQUEDA LINEAL
	centro=0;
	while(centro < ptr->cantClavesUsadas && elem <= ptr->dato[centro] ){
		if(ptr->dato[centro] == elem)
			return true;
		centro++;
	}
*/
	return false;
}
/*---------------------------------------------------------------------------*/

/*
Busca un dato dentro del arbol o el lugar donde debe insertarse el elemento y
si es necesario carga en memoria los nodos que estan en disco.
 Parametros:
   	    CONT elem : identificacion del elemento a buscar.

 */
template < class CONT, class ELEM >
bool cArbol< CONT, ELEM >::buscarLugar(CONT &elem){
	t_uint pos=0;
	bool salir;
	bool isFirst=true;

	if(!isCreado)
	   throw ExceptionTree(ERROR_NEA);

	raiz == NULL ? salir=true : salir=false;

	corriente=raiz;

	while(!salir){

	  /*Me muevo por los elementos del nodo hasta encontrar uno que
	    tenga clave mayor o igual a la que busco.*/
		if(buscarPos(corriente,elem,pos)){
			elem = corriente->dato[pos];
			return true;

		}else{
			/*Muevo el corriente al hijo si este no era una hoja*/
			if(corriente->ptr[pos]==NULL){

				if(corriente->ptrNroHijo[pos]!=NULL_BL){
					corriente=cargarNodo(corriente->ptrNroHijo[pos],corriente);

					if(isFirst){
					  /*Uno el arbol a los nodos que lei del disco*/
						corriente->padre->ptr[pos]=corriente;
						corriente->padre->ptrNroHijo[pos]=corriente->nroNodo;
					}else isFirst=false;

				}else salir=true;

			}else  corriente = corriente->ptr[pos];
		}
	}

  return false;
}
/*---------------------------------------------------------------------------*/
/*
Busca un dato dentro del arbol.

 Parametros:
  	    CONT : identificacion del elemento a buscar.

 */
template < class CONT, class ELEM >
bool cArbol< CONT, ELEM >::buscar(CONT &elem){
	t_uint pos=0;
	bool salir,enDisco=false;
	cNodo<CONT,ELEM> *aux=NULL;

	if(!isCreado)
	   throw ExceptionTree(ERROR_NEA);

	raiz == NULL ? salir=true : salir=false;

	corriente=raiz;

	while(!salir){

	  /*Me muevo por los elementos del nodo hasta encontrar uno que
	    tenga clave mayor o igual a la que busco.*/
		if(buscarPos(corriente,elem,pos)){
			elem = corriente->dato[pos];
			return true;

		}else{
			/*Muevo el corriente al hijo si este no era una hoja*/
			if(corriente->ptr[pos]==NULL){

				if(corriente->ptrNroHijo[pos]!=NULL_BL){
					aux=corriente;
					corriente=cargarNodo(aux->ptrNroHijo[pos],NULL);
					if(enDisco){
						delete aux;
						aux=NULL;
					}else{
						  enDisco=true;
						  aux=NULL;
					}

				}else salir=true;

			}else{
				  corriente = corriente->ptr[pos];
			}
		}
	}

	if(aux!=NULL)
		delete aux;

  return false;
}
/*---------------------------------------------------------------------------*/
/*
 Carga un nodo en particular
  Parametros:
		t_uint nroNodo: nmero de nodo a cargar
		cNodo *padre: padre del nodo a cargar

  Retorno:
		cNodo*: nodo que se leyo del disco.
*/
template < class CONT, class ELEM>
cNodo<CONT,ELEM> *cArbol<CONT,ELEM>::cargarNodo(t_uint nroNodo,cNodo<CONT,ELEM> *padre){

	std::ifstream inputFile;

	inputFile.open(nameFile.c_str(), std::ios::in | std::ios::binary);

	if(!inputFile.good())
		throw ExceptionTree(ERROR_FILE_NOT_FOUND);

	cNodoDisco<ELEM> nodoDisk;
	inputFile.seekg((nroNodo-1) * cNodoDisco<ELEM>::sizeofNodoDisco() + cHeaderArbol::sizeofHeader());
	inputFile >> nodoDisk;
	inputFile.close();
	return new cNodo<CONT,ELEM>(nodoDisk,padre);

}
/*---------------------------------------------------------------------------*/
/*
Guarda en disco un nodo que fue modificado.
 Parametros:
		cNodo *nodo: nodo que se va a salvar

*/
template < class CONT, class ELEM>
void cArbol< CONT, ELEM >::salvarNodo(cNodo<CONT,ELEM> *nodo){
	std::ofstream outputFile;

	if(nodo==NULL)
		return;

	outputFile.open(nameFile.c_str(), std::ios::in | std::ios::out |
	                                                        std::ios::binary);

	if(!outputFile.good())
		throw ExceptionTree(ERROR_FILE_NOT_FOUND);

	cNodoDisco<ELEM> nodoDisk = nodo->getNodoDisco();

	outputFile.seekp((nodo->nroNodo-1) * cNodoDisco<ELEM>::sizeofNodoDisco() + cHeaderArbol::sizeofHeader());
	outputFile << nodoDisk;
	outputFile.close();

}
/*----------------------------------------------------------------------------*/
/*
Inserta en un nodo con espacio un nuevo elemento.
 Parametros:
              cNodo * ptrNodo: nodo en donde se debe insertar
  	      ELEM &elem: elemento a insertar
	      cNodo *ptrHijo1: hijo izq del nodo
	      cNodo *ptrHijo2: hijo der del nodo
*/
template < class CONT, class ELEM>
void cArbol< CONT, ELEM>::armarNodo(cNodo<CONT,ELEM> *ptrNodo , CONT &elem,cNodo<CONT,ELEM> *ptrHijo1,
                                                              cNodo<CONT,ELEM> *ptrHijo2){
    t_uint i = 0;

	/*Si ya hay elementos en el nodo habro la lista para insertar ordenado.*/
	if(ptrNodo->cantClavesUsadas > 0) {

		/*Calculo donde tengo que insertar*/
		buscarPos(ptrNodo,elem,i);

		/*Corro los punteros y los elementos un lugar a la derecha*/
        for(t_uint j = ptrNodo->cantClavesUsadas; j > i; j--){
            ptrNodo->dato[j] = ptrNodo->dato[j-1];
			ptrNodo->ptr[j+1] = ptrNodo->ptr[j];
			ptrNodo->ptrNroHijo[j+1]=ptrNodo->ptrNroHijo[j];
		}
		ptrNodo->ptr[i+1] = ptrNodo->ptr[i];
		ptrNodo->ptrNroHijo[i+1]=ptrNodo->ptrNroHijo[i];
    }

    ptrNodo->cantClavesUsadas++;
    ptrNodo->dato[i] = elem;
    ptrNodo->ptr[i] = ptrHijo1;
    ptrNodo->ptr[i+1] = ptrHijo2;

    if(ptrHijo1 != NULL){
	   ptrHijo1->padre = ptrNodo;
	   ptrNodo->ptrNroHijo[i]=ptrHijo1->nroNodo;
	}

    if(ptrHijo2 != NULL){
	   ptrHijo2->padre = ptrNodo;
	   ptrNodo->ptrNroHijo[i+1]=ptrHijo2->nroNodo;
	}

}

/*---------------------------------------------------------------------------*/
/*
  Inserta un elemento dentro del arbol si este no existe
  Parametros:
             CONT dato : elemento a insertar
*/
template < class CONT, class ELEM>
void cArbol< CONT, ELEM >::insertar(CONT dato){
   cNodo<CONT,ELEM> *padre=NULL,*ptrHijo2=NULL,*ptrHijo1=NULL;
   CONT *listDatos = new CONT[CANT_ELEM_X_NODO+1];
   cNodo<CONT,ELEM> **listPtrs = new cNodo<CONT,ELEM>*[CANT_ELEM_X_NODO+2];
   t_offset *listPtrsNro = new  t_offset[CANT_ELEM_X_NODO+2];

   	t_uint i=0, j=0;
	bool salir = false;

	if(!isCreado)
	   throw ExceptionTree(ERROR_NEA);

  /*Busco el nodo donde tengo que insertar*/
	if(buscarLugar(dato))
	   throw ExceptionTree(ELEM_FOUND);


   do {

	 /*Entra cuando la raiz es null o se produce
	   un overflow en esta.*/
	  if(corriente==NULL){
         corriente = new cNodo<CONT,ELEM>(++header.cantNodos);
         raiz = corriente;
      }

	  padre = corriente->padre;

	/*Entra si se produce un overflow es decir no hay mas lugar
	  para almacenar los elementos por lo tanto se debe partir el
	  nodo en dos*/
      if(corriente->cantClavesUsadas == CANT_ELEM_X_NODO){
		 cNodo<CONT,ELEM> * nodoNuevo = new cNodo<CONT,ELEM>(++header.cantNodos);

	    /*El nodo corriente va a pasar a ser el nodo izq y el
		  nodo nuevo el nodo derecho del elemento que ascienda*/

         i = 0;
		/* Guardo las claves y punteros del nuevo nodo, tomo hasta el
		   nro medio de claves */
         while(i < CANT_ELEM_X_NODO && corriente->dato[i] < dato) {
            listDatos[i] = corriente->dato[i];
            listPtrs[i] = corriente->ptr[i];
			listPtrsNro[i]=corriente->ptrNroHijo[i];
            i++;
         }

		 /*Inserto ordenado el nuevo dato del nodo*/
         listDatos[i] = dato;
		 listPtrs[i] = ptrHijo1;
		 listPtrs[i+1] = ptrHijo2;

		 if(ptrHijo1!=NULL)
			listPtrsNro[i]=ptrHijo1->nroNodo;
		 else listPtrsNro[i]=NULL_BL;

		 if(ptrHijo1!=NULL)
			listPtrsNro[i+1]=ptrHijo2->nroNodo;
		 else listPtrsNro[i+1]=NULL_BL;

         while(i < CANT_ELEM_X_NODO) {
            listDatos[i+1] = corriente->dato[i];
            listPtrs[i+2] = corriente->ptr[i+1];
			listPtrsNro[i+2]=corriente->ptrNroHijo[i+1];
            i++;
         }

         /*Divido el gran bloque en dos nodos*/

         /* Nodo izquierdo*/
	 t_uint cte = (t_uint) floor(((float)CANT_ELEM_X_NODO/2) + 0.5);
         corriente->cantClavesUsadas = cte;
         for(j = 0; j < corriente->cantClavesUsadas; j++) {
            corriente->dato[j] = listDatos[j];
            corriente->ptr[j] = listPtrs[j];
			corriente->ptrNroHijo[j]=listPtrsNro[j];
         }
         corriente->ptr[corriente->cantClavesUsadas] =
                                          listPtrs[corriente->cantClavesUsadas];

		 corriente->ptrNroHijo[corriente->cantClavesUsadas]=
		                               listPtrsNro[corriente->cantClavesUsadas];

         /* Nodo derecho */
         nodoNuevo->cantClavesUsadas = CANT_ELEM_X_NODO -
                                                   corriente->cantClavesUsadas;

         for(j = 0; j < nodoNuevo->cantClavesUsadas; j++) {
            nodoNuevo->dato[j] = listDatos[j+cte+1];
            nodoNuevo->ptr[j] = listPtrs[j+cte+1];
			nodoNuevo->ptrNroHijo[j]=listPtrsNro[j+cte+1];
			/*Borro los datos excedentes del nodo*/
			corriente->ptrNroHijo[corriente->cantClavesUsadas+j+1]=NULL_BL;
			corriente->dato[corriente->cantClavesUsadas+j].erase();
			corriente->ptr[corriente->cantClavesUsadas+j+1]=NULL;
         }

         nodoNuevo->ptr[nodoNuevo->cantClavesUsadas]=listPtrs[CANT_ELEM_X_NODO+1];
		 nodoNuevo->ptrNroHijo[nodoNuevo->cantClavesUsadas]=listPtrsNro[CANT_ELEM_X_NODO+1];

		 /*Borro el ltimo dato excedente del nodo*/
		 if(corriente->cantClavesUsadas <= CANT_ELEM_X_NODO){
			corriente->ptrNroHijo[corriente->cantClavesUsadas+1]=NULL_BL;
			corriente->ptr[corriente->cantClavesUsadas+1]=NULL;
		 }

		 /*Reasigno los padres*/

         for(j = 0; j <= corriente->cantClavesUsadas; j++){
			 if(corriente->ptr[j] != NULL )
				(corriente->ptr[j])->padre = corriente;
		 }

		 for(j = 0; j <= nodoNuevo->cantClavesUsadas; j++){
			 if(nodoNuevo->ptr[j] != NULL )
				(nodoNuevo->ptr[j])->padre = nodoNuevo;
		 }

         dato = listDatos[cte];
         ptrHijo1 = corriente;
         ptrHijo2 = nodoNuevo;
         corriente = padre;

      }else{
			armarNodo(corriente ,dato,ptrHijo1,ptrHijo2);
            salir = true;
      }

   } while(!salir);

   salvarSubArbol(raiz);

   delete[] listPtrs;
   delete[] listDatos;
   delete[] listPtrsNro;
}

/*----------------------------------------------------------------------------*/
template < class CONT, class ELEM>
void cArbol< CONT, ELEM >::salvarSubArbol(cNodo<CONT,ELEM> *nodo, t_uint alt){

	for(t_uint j=0 ; j <= nodo->cantClavesUsadas; j++){
		if(nodo->ptr[j]!=NULL){
			salvarSubArbol(nodo->ptr[j],alt+1);
			if(alt > limitLevel){
				salvarNodo(nodo->ptr[j]);
				delete nodo->ptr[j];
				nodo->ptr[j]=NULL;
			}
		}
	}
}
/*---------------------------------------------------------------------------*/
/*
Libera la memoria asignada a los nodos del arbol de forma recursiva.
 Parametros:
	     cNodo *ptr : nodo a liberar
*/
template < class CONT, class ELEM>
void cArbol< CONT, ELEM >::borrarNodo(cNodo<CONT,ELEM> *ptr){

	if(ptr != NULL){
		for(t_uint i = 0; i <= ptr->cantClavesUsadas; i++)
			borrarNodo(ptr->ptr[i]);
		delete ptr;
	}
}

/*---------------------------------------------------------------------------*/
/*
Carga los nodos almacenados en disco en memoria de forma recursiva
 Parametros:
	     std::ifstream &inputFile: archivo que contiene los nodos
	     cNodo *padre : nodo padre que se le debe asignar al nodo
 Retorno:
 	     cNodo*: padre del nodo.
*/
template < class CONT, class ELEM>
cNodo<CONT,ELEM>* cArbol< CONT, ELEM >::cargarRec(t_uint nroNodo,cNodo<CONT,ELEM> *padre, t_uint limit){
	t_uint cont=0;
	cNodo<CONT,ELEM> *nodo=cargarNodo(nroNodo,padre);

	while((cont<=nodo->cantClavesUsadas) && (nodo->ptrNroHijo[cont]!=NULL_BL)
	                                                   && (limit!=limitLevel)){
		  nodo->ptr[cont]=cargarRec(nodo->ptrNroHijo[cont], nodo,limit+1);
		  cont++;
	}
	return nodo;
}

/*---------------------------------------------------------------------------*/
/*
Carga los nodos almacenados en disco
 Parametros:
	     std::string nameFile: nombre del archivo que contiene los nodos
*/
template < class CONT, class ELEM>
void cArbol< CONT, ELEM >::loadArbol(std::string nameFile){

	std::ifstream inputFile;

	inputFile.open(nameFile.c_str(), std::ios::in | std::ios::binary);

	if(!inputFile.good())
		throw ExceptionTree(ERROR_FILE_NOT_FOUND);

	this->nameFile=nameFile;
	inputFile >> header;
	inputFile.close();

	raiz=cargarRec(header.nroRaiz,NULL,0);

	isCreado=true;
}
/*---------------------------------------------------------------------------*/
/*
Crea un arbol
   Parametros:
		std::string nameFile: nombre del archivo donde se almacenara
		                      el arbol si existen datos los borra.
*/
template < class CONT, class ELEM>
void cArbol< CONT, ELEM >::crearArbol(std::string nameFile){

	std::ofstream outputFile(nameFile.c_str(),std::ios::out | std::ios::binary);

	if(!outputFile.good())
		throw ExceptionTree(ERROR_NEA);

	outputFile << header;
	outputFile.close();
	isCreado=true;
	this->nameFile=nameFile;

}
/*---------------------------------------------------------------------------*/
/* Borra el arbol del disco.
*/
template < class CONT, class ELEM>
void cArbol< CONT, ELEM >::destroy(){
	if(isCreado){
	   if(raiz!=NULL){
		isCreado=false;
		borrarNodo(raiz);
		raiz=NULL;
		corriente=NULL;
		remove(nameFile.c_str());
   		nameFile="";
   		limitLevel=0; 
	   }
	
	}else throw ExceptionTree(ERROR_DT);
}
/*---------------------------------------------------------------------------*/
/*
Guarda en disco los nodos del arbol en forma recursiva
 Parametros:
		std::ofstream &outputFile: archivo donde se guardan los nodos
	        cNodo *nodo: nodo que se va almacenar
*/
template < class CONT, class ELEM>
void cArbol< CONT, ELEM >::guardarRec(std::ofstream &outputFile, cNodo<CONT,ELEM> *nodo){

	cNodoDisco<ELEM> nodoDisk;

	nodoDisk=nodo->getNodoDisco();

	for(t_uint i=0 ; i< nodo->cantClavesUsadas ;i++){
		/*Por si el nodo tiene sus hijos en disco*/
		if(nodo->ptr[i]!=NULL)
			guardarRec(outputFile,nodo->ptr[i]);
	}

	if(nodo->ptr[nodo->cantClavesUsadas]!=NULL){
		guardarRec(outputFile,nodo->ptr[nodo->cantClavesUsadas]);
	}

	outputFile.seekp((nodo->nroNodo-1) *cNodoDisco<ELEM>::sizeofNodoDisco() + cHeaderArbol::sizeofHeader());
	outputFile << nodoDisk;

}

/*---------------------------------------------------------------------------*/
/*
 Guarda en disco los nodos del arbol en forma recursiva
*/
template < class CONT, class ELEM>
void cArbol< CONT, ELEM >::saveArbol(){

	if(!isCreado)
	   throw ExceptionTree(ERROR_NEA);

	std::ofstream outputFile;

	outputFile.open(nameFile.c_str(), std::ios::in | std::ios::out | std::ios::binary);

	if(!outputFile.good())
		throw ExceptionTree(ERROR_SAVE_TREE);

	header.nroRaiz=raiz->nroNodo;
	outputFile << header;	
	guardarRec(outputFile,raiz);
	outputFile.close();	
	
}

/*---------------------------------------------------------------------------*/
/*
   Salva y borra el arbol.
*/
template < class CONT, class ELEM>
void cArbol< CONT, ELEM >::closeArbol(){

	if(isCreado && raiz!=NULL){
		saveArbol();
		isCreado=false;
		borrarNodo(raiz);
		raiz=NULL;
		corriente=NULL;
   		nameFile="";
   		limitLevel=0; 
	}
}

/*---------------------------------------------------------------------------*/
/*
template < class CONT, class ELEM>
void cArbol< CONT, ELEM >::mostrar(){

	if(!isCreado)
	   throw ExceptionTree(ERROR_NEA);

	std::cout << "------------------------------------------------------------------------" << std::endl;
	std::cout << "| Lo que esta entre () es el nodo y lo que esta entre [] es el padre   |" << std::endl;
	std::cout << "| El segundo [] indica el numero del padre del nodo en disco.          |" << std::endl;
	std::cout << "| El tercero [] es el numero de nodo en disco.                         |" << std::endl;
	std::cout << "| Si dise [-NRO] significa que el nodo es el hijo Izq del elemento NRO |" << std::endl;
	std::cout << "| Si dise [+NRO] significa que el nodo es el hijo Der del elemento NRO |" << std::endl;
	std::cout << "------------------------------------------------------------------------" << std::endl;
	std::cout << "---------" << std::endl;
	std::cout << "| Arbol |" << std::endl;
	std::cout << "---------" << std::endl << std::endl;
    ver(raiz);
	std::cout << "------------------------------------------------------------------------" << std::endl;

}
*/
/*------------------------------------------------------------------------------*/
/*
template < class CONT, class ELEM>
void cArbol< CONT, ELEM >::ver(cNodo<CONT,ELEM> *nodo){
   t_uint i;
   t_uint aux=0;

   if(!nodo) return;

   std::cout << "[";

   if(nodo->padre!=NULL){
	   t_uint j=0;
	   while(j<(nodo->padre)->cantClavesUsadas && (nodo->padre)->dato[j] < nodo->dato[nodo->cantClavesUsadas-1]){
		   j++;
	   }
	   if(j==nodo->padre->cantClavesUsadas){
	       if(j>0)
  		      std::cout << "+" << (nodo->padre)->dato[j-1];
	   }else{
		    std::cout << "-" << (nodo->padre)->dato[j];
	   }

   }else std::cout << "RR";

   std::cout << "] ";

   if(nodo->padre != NULL){

	   if(aux>9){
		   if(nodo->padre->nroNodo > 9)
		      std::cout << "[" << nodo->padre->nroNodo << "] ";
		   else std::cout << "[" << nodo->padre->nroNodo << "]  ";
	   }else {
		      if(nodo->padre->nroNodo > 9)
		         std::cout << " [" << nodo->padre->nroNodo << "] ";
			  else std::cout << " [" << nodo->padre->nroNodo << "]  ";
	   }

   }else {
		  if(aux>9)
	         std::cout << " [RR] ";
		  else std::cout << "  [RR] ";
   }

   if(nodo != NULL){
		if(nodo->nroNodo > 9)
			std::cout << " [" << nodo->nroNodo << "] ";
		else std::cout << " [" << nodo->nroNodo << "]  ";

   }


   std::cout << "(";
   for(i = 0; i < nodo->cantClavesUsadas-1; i++)
	   std::cout << nodo->dato[i] << "-";

   if(nodo->cantClavesUsadas)
	   std::cout << nodo->dato[i] << ")" << std::endl;
*/
   /*El = es xq hay un ptr mas que datos*/
/*
   for(i = 0; i <= nodo->cantClavesUsadas; i++){
	   if(nodo->ptr[i]!=NULL)
			ver(nodo->ptr[i]);
   }

}
*/
/*------------------------------------------------------------------------------*/

#endif



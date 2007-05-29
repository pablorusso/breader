#include <iostream>
#include "cArbol.h"
#include "cPalabra.h"



bool inicializado=false;

/*------------------------------------------------------------------------------------------*/
void cargarArbol(){

   cArbol<cPalabra,cElemento> arbol(8.0625);

/*PRUEBO LA CLASE CPALABRA*/
  cPalabra elem1("0123456789");
 /*Guardo un par de palabras en disco*/
  try{
	   elem1.persistirPalabra();
  }catch(ExceptionPalabra pp){ std::cout << pp.what() << std::endl;}
   elem1.setNroBloque(55);
 /*En aux voy a tener el offset de la palabra en vez de la palabra*/
   cElemento aux1 = elem1.getDato();
 /*En el constructor de palabraLarga se va a recuperar la palabra del disco*/
   cPalabra palabraLarga(aux1);

   arbol.crearArbol("arbol.dat");
   cElemento aux;
   aux.nroBlock=99;
   aux.tipo=IS_STRING;

   char *vec[]={"33","20","57","90","49","30","17","35","10","21","40","80","56","08","39","43","60","04","31","99","01","02","11","12","13","14","15","22","23","24","25","05","06","07","61","62","63","64","65","66","81","82","83","84","44","45","46","47","67","85","86","87","88","89","26","18","075","101","58","59","060","061","062","063","064","065","066","067","068","069"};
   //             0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   35   36   37   38   39   40   41   42   43   44   45   46   47   48   49   50   51   52   53  54    55    56    57   58   59    60    61    62    63    64    65    66    67    68    69

   for(int k=0;k<70 ;k++){
	   aux.palabra[0]=vec[k][0];
	   aux.palabra[1]=vec[k][1];
	   aux.palabra[2]=vec[k][2];
	   aux.palabra[3]=vec[k][3];

	   cPalabra elem(aux);
	   try{
			arbol.insertar(elem);
	   }catch(ExceptionTree ee){ std::cout << ee.what() << std::endl;  }
   }

   arbol.insertar(palabraLarga);
   arbol.mostrar();
   arbol.saveArbol();

   if(arbol.buscar(palabraLarga))
	   std::cout << "Se Busco palabra Larga: Encontrada" << std::endl;
   else std::cout << "Se Busco palabra Larga: NO Encontrado" << std::endl;

   cPalabra noEstoy("no estoy");
   if(arbol.buscar(noEstoy))
	   std::cout << "Se Busco palabra 'no estoy': Encontrada" << std::endl;
   else std::cout << "Se Busco palabra 'no estoy': NO Encontrado" << std::endl;

   inicializado=true;
}

/*------------------------------------------------------------------------------*/

int main(){

	remove(NAME_FILE_PALABRAS);
    remove("arbol.dat");
	bool salir=false;
	int op=0;

	std::cout << "1 - Crear y generar el arbol (hasta nivel 1 en memoria) e " << std::endl;
	std::cout << "    insertar palabra larga y buscarla" << std::endl << std::endl;
	std::cout << "2 - Crear y cargar el arbol hasta el nivel 3 en memoria" << std::endl << std::endl;
	std::cout << "3 - Salir" << std::endl << std::endl;

	while(!salir){
		std::cout << "op=";
		std::cin >> op;

		switch(op){

		case 1: cargarArbol();
				break;

		case 2:{
				if(inicializado){
					cArbol<cPalabra,cElemento> arbol(48.60);
					arbol.loadArbol("arbol.dat");
					arbol.mostrar();
				}else {std::cout << "Primero debe generarse el Arbol con 1" << std::endl;}
				break;
			   }

		case 3:
				salir=true;
				break;
		}

		op=0;

	}

//	int tamano=CANT_ELEM_X_NODO * sizeof(cPalabra) + (CANT_ELEM_X_NODO+1)*(sizeof(tNodo*) + sizeof(int))+ sizeof(tNodo) ;
//		std::cout << "Tam Nodo: " << tamano << "tam no "<< sizeof(tNodo)<< std::endl;

   return 0;
}
/*------------------------------------------------------------------------------*/

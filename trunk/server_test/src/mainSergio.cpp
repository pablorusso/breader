
#include <iostream>
#include <string>
#include "cFileManager.h"


/*
SALIDA CORRECTA:

PRUEBA SERGIO
Categoria ID: 50 offset: 4 frecuencia: 0
Categoria ID: 51 offset: 5 frecuencia: 1
Categoria ID: 51 offset: 4 frecuencia: 2
Categoria ID: 51 offset: 4 frecuencia: 4
Categoria ID: 54 offset: 4 frecuencia: 4
Categoria ID: 53 offset: 4 frecuencia: 3
-------------------------------------------
Categoria ID: 53 offset: 5 frecuencia: 1
Categoria ID: 53 offset: 6 frecuencia: 2
Categoria ID: 53 offset: 7 frecuencia: 3
Categoria ID: 51 offset: 5 frecuencia: 2
Categoria ID: 53 offset: 8 frecuencia: 4
Categoria ID: 51 offset: 4 frecuencia: 2
Borro cat 3
Categoria ID: 54 offset: 4 frecuencia: 4
Busco cat 3 -> Deberia producirce un error ->
Error no se ha podido encontrar el registro.


*/

/*------------------------------------------------------------*/
void func(){
	Archivo4 aa;


	try{	
	//	cFileManager pp(&aa);
 		cFileManager pp;
		pp.crearFileManager("datosFM.dat");

		t_idcat cat0 = aa.addCategory("Cat0");
		t_idcat cat1 = aa.addCategory("Cat1");
		t_idcat cat2 = aa.addCategory("Cat2");
		t_idcat cat3 = aa.addCategory("Cat3");
		t_idcat cat4 = aa.addCategory("Cat4");
	
		/*Agrego Palabras*/
		t_offset off0 = pp.addPalabra();	
		t_offset off1 = pp.addPalabra();
		t_offset off2 = pp.addPalabra();
		t_offset off3 = pp.addPalabra();
		t_offset off4 = pp.addPalabra();
		


		/*Agrego a una palabra una caegoria*/
		t_diferencias ff = {0,0};
		pp.setPalabra(cat0 , off0 , ff);

	    ff.cantFalse = 1; ff.cantTrue = 1;
		pp.setPalabra(cat1 , off1 , ff);

	    ff.cantFalse = 2; ff.cantTrue = 2;
		pp.setPalabra(cat1 , off0 , ff);

		tFrecuencias frec;
		/*----------------------------------------------------------*/
		frec = pp.getPalabra(cat0, off0);
		std::cout << "Categoria ID: " << cat0 << " offset: " << off0 << " frecuencia: " << frec.cantFalse << std::endl;

		frec = pp.getPalabra(cat1, off1);
		std::cout << "Categoria ID: " << cat1 << " offset: " << off1 << " frecuencia: " << frec.cantFalse << std::endl;
  
		frec = pp.getPalabra(cat1, off0);
		std::cout << "Categoria ID: " << cat1 << " offset: " << off0 << " frecuencia: " << frec.cantFalse << std::endl;
		/*----------------------------------------------------------*/

		ff.cantFalse = 2; ff.cantTrue = 2;
		pp.setPalabra(cat1 , off0 , ff);

		/*----------------------------------------------------------*/
		frec = pp.getPalabra(cat1, off0);
		std::cout << "Categoria ID: " << cat1 << " offset: " << off0 << " frecuencia: " << frec.cantFalse << std::endl;

		/*----------------------------------------------------------*/

		ff.cantFalse = 4; ff.cantTrue = 4;
		pp.setPalabra(cat4 , off0 , ff);
		
		ff.cantFalse = 3; ff.cantTrue = 3;
		pp.setPalabra(cat3 , off0 , ff);

/*----------------------------------------------------------*/
		frec = pp.getPalabra(cat4, off0);
		std::cout << "Categoria ID: " << cat4 << " offset: " << off0 << " frecuencia: " << frec.cantFalse << std::endl;
		
		frec = pp.getPalabra(cat3, off0);
		std::cout << "Categoria ID: " << cat3 << " offset: " << off0 << " frecuencia: " << frec.cantFalse << std::endl;
			std::cout << "-------------------------------------------" << std::endl;
/*----------------------------------------------------------*/

		ff.cantFalse = 1; ff.cantTrue = 1;
		pp.setPalabra(cat3 , off1 , ff);
		
		ff.cantFalse = 2; ff.cantTrue = 2;
		pp.setPalabra(cat3 , off2 , ff);
		
		ff.cantFalse = 3; ff.cantTrue = 3;
		pp.setPalabra(cat3 , off3 , ff);

		ff.cantFalse = 1; ff.cantTrue = 1;
		pp.setPalabra(cat1 , off1 , ff);

/*----------------------------------------------------------*/
		frec = pp.getPalabra(cat3, off1);
		std::cout << "Categoria ID: " << cat3 << " offset: " << off1 << " frecuencia: " << frec.cantFalse << std::endl;

		frec = pp.getPalabra(cat3, off2);
		std::cout << "Categoria ID: " << cat3 << " offset: " << off2 << " frecuencia: " << frec.cantFalse << std::endl;

		frec = pp.getPalabra(cat3, off3);
		std::cout << "Categoria ID: " << cat3 << " offset: " << off3 << " frecuencia: " << frec.cantFalse << std::endl;

		frec = pp.getPalabra(cat1, off1);
		std::cout << "Categoria ID: " << cat1 << " offset: " << off1 << " frecuencia: " << frec.cantFalse << std::endl;
		
/*----------------------------------------------------------*/

		ff.cantFalse = 4; ff.cantTrue = 4;
		pp.setPalabra(cat3 , off4 , ff);

		ff.cantFalse = 2; ff.cantTrue = 2;
		pp.setPalabra(cat2 , off0 , ff);


/*----------------------------------------------------------*/

		frec = pp.getPalabra(cat3, off4);
		std::cout << "Categoria ID: " << cat3 << " offset: " << off4 << " frecuencia: " << frec.cantFalse << std::endl;

		frec = pp.getPalabra(cat2, off0);
		std::cout << "Categoria ID: " << cat1 << " offset: " << off0 << " frecuencia: " << frec.cantFalse << std::endl;


/*----------------------------------------------------------*/

	std::cout << "Borro cat 3" << std::endl;
	pp.deleteCategoria(cat3);
	pp.restructurar();

	frec = pp.getPalabra(cat4, off0);
	std::cout << "Categoria ID: " << cat4 << " offset: " << off0 << " frecuencia: " << frec.cantFalse << std::endl;

	std::cout << "Busco cat 3 -> Deberia producirce un error ->" << std::endl;
	frec = pp.getPalabra(cat3, off0);
	std::cout << "Categoria ID: " << cat3 << " offset: " << off0 << " frecuencia: " << frec.cantFalse << std::endl;
	
	std::cout << "-------------------------------------------" << std::endl;

	pp.cerrarFileManager();	
	}catch(ExceptionFileManager &e){ 			
			std::cout << e.what() << std::endl;
	}
	

}
/*------------------------------------------------------------*/
int mainX(int argc, char** argv){
	std::cout << "PRUEBA SERGIO" << std::endl;
	func();

	remove(A4_FILENAME);

	return 0;
}
/*------------------------------------------------------------*/

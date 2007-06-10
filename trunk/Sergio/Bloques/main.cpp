
#include "cFileManager.h"
#include <iostream>

cFileManager pp;


/*----------------------------------------------------------------------------*/
void add(){

	tFrecuencias ff={0,0};

	std::cout << "-----------------------------------------------" << std::endl;	

	try{


	ff.cantFalse=0;  ff.cantTrue=0;
	std::cout << "ADD01: FREC:0  OFFSET:  " << pp.addPalabra(0,ff) << std::endl;
	
	ff.cantFalse=1;  ff.cantTrue=1;
	std::cout << "ADD02: FREC:1  OFFSET:  " << pp.addPalabra(1,ff) << std::endl;
	
	ff.cantFalse=2;  ff.cantTrue=2;
	std::cout << "ADD03: FREC:2  OFFSET:  " << pp.addPalabra(0,ff) << std::endl;
	
	ff.cantFalse=3;  ff.cantTrue=3;
	std::cout << "ADD04: FREC:3  OFFSET:  " << pp.addPalabra(1,ff) << std::endl;
	
	ff.cantFalse=4;  ff.cantTrue=4;
	std::cout << "ADD05: FREC:4  OFFSET:  " << pp.addPalabra(1,ff) << std::endl;
	
	ff.cantFalse=5;  ff.cantTrue=5;
	std::cout << "ADD06: FREC:5  OFFSET:  " << pp.addPalabra(0,ff) << std::endl;

	ff.cantFalse=6;  ff.cantTrue=6;
	std::cout << "ADD07: FREC:6  OFFSET:  " << pp.addPalabra(0,ff) << std::endl;
	
	ff.cantFalse=7;  ff.cantTrue=7;
	std::cout << "ADD08: FREC:7  OFFSET:  " << pp.addPalabra(1,ff) << std::endl;
	
	ff.cantFalse=8;  ff.cantTrue=8;
	std::cout << "ADD09: FREC:8  OFFSET:  " << pp.addPalabra(0,ff) << std::endl;
	
	ff.cantFalse=9;  ff.cantTrue=9;
	std::cout << "ADD10: FREC:9  OFFSET:  " << pp.addPalabra(1,ff) << std::endl;
	
	ff.cantFalse=10;  ff.cantTrue=10;
	std::cout << "ADD11: FREC:10 OFFSET:  " << pp.addPalabra(1,ff) << std::endl;
	
	ff.cantFalse=11;  ff.cantTrue=11;
    std::cout << "ADD12: FREC:11 OFFSET:  " << pp.addPalabra(0,ff) << std::endl;

	ff.cantFalse=12;  ff.cantTrue=12;
	std::cout << "ADD13: FREC:12  OFFSET:  " << pp.addPalabra(2,ff) << std::endl;
	
	ff.cantFalse=13;  ff.cantTrue=13;
	std::cout << "ADD14: FREC:13  OFFSET:  " << pp.addPalabra(2,ff) << std::endl;
	
	ff.cantFalse=14;  ff.cantTrue=14;	
	std::cout << "ADD15: FREC:14  OFFSET:  " << pp.addPalabra(2,ff) << std::endl;

	ff.cantFalse=15;  ff.cantTrue=15;
	std::cout << "ADD16: FREC:15  OFFSET:  " << pp.addPalabra(2,ff) << std::endl;
	
	ff.cantFalse=16;  ff.cantTrue=16;
	std::cout << "ADD17: FREC:16  OFFSET:  " << pp.addPalabra(2,ff) << std::endl;
	
	ff.cantFalse=17;  ff.cantTrue=17;
	std::cout << "ADD18: FREC:17  OFFSET:  " << pp.addPalabra(2,ff) << std::endl;

	ff.cantFalse=18;  ff.cantTrue=18;
	std::cout << "ADD19: FREC:18  OFFSET:  " << pp.addPalabra(2,ff) << std::endl;
	
	ff.cantFalse=19;  ff.cantTrue=19;
	std::cout << "ADD20: FREC:19  OFFSET:  " << pp.addPalabra(3,ff) << std::endl;
	
	ff.cantFalse=20;  ff.cantTrue=20;
	std::cout << "ADD21: FREC:20  OFFSET:  " << pp.addPalabra(3,ff) << std::endl;

	ff.cantFalse=21;  ff.cantTrue=21;
	std::cout << "ADD22: FREC:21  OFFSET:  " << pp.addPalabra(2,ff) << std::endl;

	ff.cantFalse=22;  ff.cantTrue=22;
	std::cout << "ADD23: FREC:22 OFFSET:  " << pp.addPalabra(3,ff) << std::endl;

	}catch(ExceptionFileManager e){ 
		std::cout << "error: " << e.what() << std::endl;	
	}


}
/*----------------------------------------------------------------------------*/
void get(){


	tFrecuencias ff={0,0};

	std::cout << "-----------------------------------------------" << std::endl;	

	try{
	


	try{

		/*--------------------------------------------------------------------*/
		ff = pp.getPalabra(1,19);		
		std::cout << "GET08: FREC:" << ff.cantFalse <<"  OFFSET: 19  " << std::endl;

		ff = pp.getPalabra(2,33);		
		std::cout << "GET14: FREC:" << ff.cantFalse <<"  OFFSET: 33  " << std::endl;
		
		ff = pp.getPalabra(2,53);		
		std::cout << "GET22: FREC:" << ff.cantFalse <<" OFFSET: 53  " << std::endl;

		ff = pp.getPalabra(0,20);		
		std::cout << "GET09: FREC:" << ff.cantFalse <<"  OFFSET: 20  " << std::endl;

		ff = pp.getPalabra(3,52);		
		std::cout << "GET21: FREC:" << ff.cantFalse <<"  OFFSET: 52  " << std::endl;

		ff = pp.getPalabra(0,23);		
		std::cout << "GET12: FREC:" << ff.cantFalse <<" OFFSET: 23  " << std::endl;

		ff = pp.getPalabra(3,54);		
		std::cout << "GET23: FREC:" << ff.cantFalse <<" OFFSET: 54  " << std::endl;

		ff = pp.getPalabra(1,16);		
		std::cout << "GET05: FREC:" << ff.cantFalse <<"  OFFSET: 16  " << std::endl;
	

		/*--------frecuencia de una palabra una que no existe-------*/
		ff = pp.getPalabra(2,87);
		/*--------------------------------------------------------------------*/

	}catch(ExceptionFileManager e){
		std::cout << "No existe-> "<< e.what() << std::endl;
	
	}	

	}catch(ExceptionFileManager e){ 
			std::cout <<"error: " << e.what();	
	}


}
/*----------------------------------------------------------------------------*/
void get2(){

		tFrecuencias ff={1,1};
	std::cout << "-----------------------------------------------" << std::endl;	
	try{


		ff = pp.getPalabra(2,19);		
		std::cout << "GET08: FREC:" << ff.cantFalse <<"  OFFSET: 19  " << std::endl;

		ff = pp.getPalabra(3,33);	
		std::cout << "GET14: FREC:" << ff.cantFalse <<"  OFFSET: 33 " << std::endl;
		
		ff = pp.getPalabra(3,53);
		std::cout << "GET22: FREC:" << ff.cantFalse <<"  OFFSET: 53" << std::endl;

		ff = pp.getPalabra(1,20);		
		std::cout << "GET09: FREC:" << ff.cantFalse <<"  OFFSET: 20 " << std::endl;

		ff = pp.getPalabra(2,52);		
		std::cout << "GET21: FREC:" << ff.cantFalse <<"  OFFSET: 52 " << std::endl;

		ff = pp.getPalabra(1,23);		
		std::cout << "GET12: FREC:" << ff.cantFalse <<" OFFSET: 23 " << std::endl;

		ff = pp.getPalabra(4,54);		
		std::cout << "GET23: FREC:" << ff.cantFalse <<" OFFSET: 54  " << std::endl;

		ff = pp.getPalabra(2,16);		
		std::cout << "GET05: FREC:" << ff.cantFalse <<"  OFFSET: 16  " << std::endl;


	}catch(ExceptionFileManager e){ 
		std::cout <<"GET error: " << e.what() << std::endl;
	}


}
/*----------------------------------------------------------------------------*/
void set(){

	tFrecuencias ff={0,1};

	try{
	

		

	//setPalabra(const unsigned int &idCat,const unsigned int &nro, const tFrecuencias &frec);

	try{

		/*----------------------Agrega frecuencia a las q ya existen-----------------------*/
		pp.setPalabra(1,19,ff);		
		pp.setPalabra(2,33,ff);				
		pp.setPalabra(2,53,ff);		
		pp.setPalabra(0,20,ff);	
		
		pp.setPalabra(2,52,ff);		
		pp.setPalabra(0,23,ff);		
		pp.setPalabra(3,54,ff);		
		pp.setPalabra(1,16,ff);		
		
/*---------Agrego frecuencia a una nueva categoria de una palabra q existe-*/

		pp.setPalabra(2,19,ff);		
		pp.setPalabra(3,33,ff);				
		pp.setPalabra(3,53,ff);		
		pp.setPalabra(1,20,ff);		
		pp.setPalabra(3,52,ff);	
		pp.setPalabra(1,23,ff);		
    	pp.setPalabra(4,54,ff);		
		pp.setPalabra(2,16,ff);					

/*----------------------Agrega frecuencia a las q ya existen------------------*/
	    		
		pp.setPalabra(2,19,ff);
		pp.setPalabra(1,20,ff);
		pp.setPalabra(3,33,ff);				
		pp.setPalabra(3,53,ff);		
		pp.setPalabra(3,33,ff);				
		pp.setPalabra(2,52,ff);		
		pp.setPalabra(4,54,ff);		
		pp.setPalabra(4,54,ff);				    	
		pp.setPalabra(2,16,ff);	
		pp.setPalabra(1,23,ff);		

/*--------Intento agregar frecuencia a una palabra una que no existe-------*/
		pp.setPalabra(2,87,ff);
/*-------------------------------------------------------------------------*/


	}catch(ExceptionFileManager e){
		std::cout << "-------------------------------------------" << std::endl;	
		std::cout << "error Set no existe ->: " << e.what()	<< std::endl;		
	
	}



	}catch(ExceptionFileManager e){ 
				std::cout <<"error: " << e.what();	
	}


}
/*----------------------------------------------------------------------------*/
void borrar(unsigned int i){


	tFrecuencias ff={0,1};

	try{	
std::cout << "---------------------DELETE CAT------------------------" << std::endl;
	
		pp.deleteCategoria(i);


		ff = pp.getPalabra(2,19);		
		std::cout << "GET08: FREC:" << ff.cantFalse <<"  OFFSET: 19  " << std::endl;

//		ff = pp.getPalabra(3,33);	
//		std::cout << "GET14: FREC:" << ff.cantFalse <<"  OFFSET: 33 " << std::endl;
		
//		ff = pp.getPalabra(3,53);
//  	std::cout << "GET22: FREC:" << ff.cantFalse <<"  OFFSET: 53" << std::endl;

		ff = pp.getPalabra(1,20);		
		std::cout << "GET09: FREC:" << ff.cantFalse <<"  OFFSET: 20 " << std::endl;

		ff = pp.getPalabra(2,52);		
		std::cout << "GET21: FREC:" << ff.cantFalse <<"  OFFSET: 52 " << std::endl;

		ff = pp.getPalabra(1,23);		
		std::cout << "GET12: FREC:" << ff.cantFalse <<" OFFSET: 23 " << std::endl;

		ff = pp.getPalabra(4,54);		
		std::cout << "GET23: FREC:" << ff.cantFalse <<" OFFSET: 54  " << std::endl;

		ff = pp.getPalabra(2,16);		
		std::cout << "GET05: FREC:" << ff.cantFalse <<"  OFFSET: 16  " << std::endl;

		ff = pp.getPalabra(2,33);		
		std::cout << "GET14: FREC:" << ff.cantFalse <<"  OFFSET: 33  " << std::endl;

		ff = pp.getPalabra(2,53);		
		std::cout << "GET22: FREC:" << ff.cantFalse <<"  OFFSET: 53  " << std::endl;
	

	}catch(ExceptionFileManager &e){
		std::cout <<"error: " << e.what() << std::endl;	
	}

}
/*----------------------------------------------------------------------------*/
void restructurar(){

	tFrecuencias ff={0,1};

	try{	
std::cout << "---------------------Restructurar------------------" << std::endl;
	
		pp.restructurar();


		ff = pp.getPalabra(2,19);		
		std::cout << "GET08: FREC:" << ff.cantFalse <<"  OFFSET: 19  " << std::endl;

		ff = pp.getPalabra(1,20);		
		std::cout << "GET09: FREC:" << ff.cantFalse <<"  OFFSET: 20 " << std::endl;

		ff = pp.getPalabra(2,52);		
		std::cout << "GET21: FREC:" << ff.cantFalse <<"  OFFSET: 52 " << std::endl;

		ff = pp.getPalabra(1,23);		
		std::cout << "GET12: FREC:" << ff.cantFalse <<" OFFSET: 23 " << std::endl;

		ff = pp.getPalabra(4,54);		
		std::cout << "GET23: FREC:" << ff.cantFalse <<" OFFSET: 54  " << std::endl;

		ff = pp.getPalabra(2,16);		
		std::cout << "GET05: FREC:" << ff.cantFalse <<"  OFFSET: 16  " << std::endl;

		ff = pp.getPalabra(2,33);		
		std::cout << "GET14: FREC:" << ff.cantFalse <<"  OFFSET: 33  " << std::endl;

		ff = pp.getPalabra(2,53);		
		std::cout << "GET22: FREC:" << ff.cantFalse <<"  OFFSET: 53  " << std::endl;


	}catch(ExceptionFileManager e){ 
		std::cout <<"error: " << e.what() << std::endl;	
	}

}

/*----------------------------------------------------------------------------*/

int main(int argc, char** argv){

	std::cout << "Header block: " << sizeof(cHeaderBlock) << std::endl;
	std::cout << "bloque: " << sizeof(cBloque) << std::endl;
	std::cout << "Registro: " << sizeof(cRegistroBlock) << std::endl;	
	std::cout << "cHeaderFile: " << sizeof(cHeaderFile) << std::endl;	
std::cout << "---------------------------------------------------" << std::endl;	
	pp.crearFileManager("bloques.dat");

	add();
	get();
	set();
	get();
	get2();
	
	borrar(3);	
	restructurar();
	


//	pp.loadFileManager("bloques.dat");

	pp.cerrarFileManager();

return 0;
}

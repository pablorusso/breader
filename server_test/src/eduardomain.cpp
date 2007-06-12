#include <iostream>
#include <fstream>

#include "Archivo4.h"
////////////////////
// PARA EDUARDO ///
///////////////////
// TODO general:
// * No guardar basura en nextFreeReg (igual ese campo ni va)
//////////////// * La longitud de las categorias es fija igual a 20, no hace falta guardar
////////////////   +1 bits en el caso de long=20
// * Al borrar una categoria, no hace falta borrar su contenido. Y si lo haces,
//   que no sea con basura
//////////////// * Te arregle que no verificabas el idcat que te pasaban a remCat
// * Fijate addCategory y modifyCategoryInfo que no se le puede pedir que te
//   pasen tooodo... son cosas que tenes que inicializar vos. El addCategory
//   deberia tener solo el string del nombre, y nada mas, el resto es cosa tuya,
//   interna a tu implementacion.
//   El modifyCategoryInfo no se, hay muchas cosas que se pueden modificar.
//   en principio seria: modifyCategoryInfo(idcat, nuevo_nombre),
//   modifyCategoryInfo(idcat, nuevo_artPositive),
//   modifyCategoryInfo(idcat, nuevo_artNegative)... y asi... aunque tambien
//   supongo seria mas comodo poder incrementar/decrementar esos valores
//   directamente... tipo incArtPositive(idcat, cant_a_incrementar), y asi
//   Podes hacerlo de varias formas, igual... fijate que seria lo mas optimo.
////////////// * Te puse todo en el dir raiz, porque a pablo le gusta asi =P

using namespace std;
int eduardomain(int argc, char** argv) {
	cout << "Bienvenido al breader" << endl;

	try {
		Archivo4 a4;

		t_queue_idcat idSet;
		
		idSet = a4.getCategoriesId();

/*	AHORA ES UNA QUEUE	
		t_queue_idcat::iterator iter;
		
		iter = idSet.begin();
		
		if(!idSet.empty())
			cout << "Tiene Algo!!" << endl;
		else
			cout << "Vacio" << endl;
		
		while(iter != idSet.end())
		{
		      cout << "id_Category: " << *iter << endl;
			iter++;	
		}
*/

		cout << "id<Category0: " << a4.addCategory("Futbol")  << endl;
		cout << "id<Category1: " << a4.addCategory("Tenis")  << endl;
		cout << "id<Category2: " << a4.addCategory("Deportes")  << endl;

	try { // Para que arroje la excepcion si el catName es muy grande
		cout << "id<Category??: " << a4.addCategory("012345678901234567890")  << endl;
	} catch (eArchivo4) {}


		if(a4.findCategory(2))
		{	cout << "Categoria 2 encontrada" << endl;
			a4.modifyCategoryName(2,"FFFutbol");
			cout << "Categoria 2 modificada" << endl;
		}else
		{
			cout << "Category 2 NO encontrada" << endl;
		}
			
		a4.incCategoryArt(2,2);
		
		a4.decCategoryArt(2, 1);

		a4.incCategoryWord(2, 10);
		
		a4.decCategoryWord(2,5);

		//modifyCategoryName(const t_idcat &idCategory, string catName);


		if (a4.findCategory(0))  cout << "Categoria 1 encontrada" << endl;
		else cout << "Categoria 1 NO encontrada" << endl;

		if (a4.deleteCategory(1)) cout << "Categoria 1 borrada" << endl;
		else cout << "Categoria 1 NO borrada" << endl;

		if (a4.findCategory(1))  cout << "Categoria 1 encontrada" << endl;
		else cout << "Categoria 1 NO encontrada" << endl;


		if (a4.deleteCategory(3))  cout << "Categoria 3 borrada" << endl;
		else cout << "Categoria 3 NO borrada" << endl;



        	/*cout << "id<Category3: " << a4.addCategory("2Futbol")  << endl;
		cout << "id<Category4: " << a4.addCategory("2Tenis")  << endl;
		cout << "id<Category4: " << a4.addCategory("2Deportes")  << endl;*/


		cout << a4 << endl;


	}
	catch (eArchivo4 e){
		cout << e.getErrorMensaje() << endl;
	}
	catch (eRegArchivo4 e){
		cout << e.getErrorMensaje() << endl;
	}
	catch (fstream::failure e) {
		cout << "ERROR: " << e.what() << endl;

	}
	cout << "Adios" << endl;
	return 0;
}

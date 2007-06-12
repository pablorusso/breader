#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "EntitiesManager.h"
#include "General.h"

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

int Damianmain(int argc, char** argv)
{
	// crea el directorio para los archivos si no existe
	string rmFiles("rm -fR ");
	rmFiles.append( General::getDataPath() );
	system(rmFiles.c_str());

	string makeDir("mkdir -p ");
	makeDir.append( General::getDataPath() );
	system(makeDir.c_str());

	cout << "Bienvenido al test del breader server" << endl << endl;

	cout << endl;
	cout << "[action] FeedCreate - ";
	cout << EntitiesManager::getInstance()->FeedCreate( "testFeed", "http://feed.url.com" );
	cout << endl;

	cout << endl;
	cout << "[action] FeedGetAll - ";
	cout << EntitiesManager::getInstance()->FeedGetAll();
	cout << endl;

	cout << endl;
	cout << "[action] ArticleCreate1 - ";
	cout << EntitiesManager::getInstance()->ArticleCreate( 0, "titulo1", "summary1", "link1", "author1", 0 );
	cout << endl;
	cout << "[action] ArticleCreate2 - ";
	cout << EntitiesManager::getInstance()->ArticleCreate( 0, "titulo2", "summary2", "link2", "author2", 0 );
	cout << endl;
	cout << "[action] ArticleCreate3 - ";
	cout << EntitiesManager::getInstance()->ArticleCreate( 0, "titulo3", "summary3", "link3", "author3", 0 );
	cout << endl;
	cout << "[action] ArticleCreate4 - ";
	cout << EntitiesManager::getInstance()->ArticleCreate( 0, "titulo4", "summary4", "link4", "author4", 0 );
	cout << endl;
	cout << "[action] ArticleCreate5 - ";
	cout << EntitiesManager::getInstance()->ArticleCreate( 0, "titulo5", "summary5", "link5", "author5", 0 );
	cout << endl;
	cout << "[action] ArticleCreate6 - ";
	cout << EntitiesManager::getInstance()->ArticleCreate( 0, "titulo6", "summary6", "link6", "author6", 0 );
	cout << endl;
	cout << "[action] ArticleCreate7 - ";
	cout << EntitiesManager::getInstance()->ArticleCreate( 0, "titulo7", "summary7", "link7", "author7", 0 );
	cout << endl;
	cout << "[action] ArticleCreate8 - ";
	cout << EntitiesManager::getInstance()->ArticleCreate( 0, "titulo8", "summary8", "link8", "author8", 0 );
	cout << endl;

	cout << endl;
	cout << "[action] ArticleGetByFeed - ";
	cout << EntitiesManager::getInstance()->ArticleGetByFeed( 0, 5 );
	cout << endl;

	cout << endl;
	cout << "[action] ArticleGetByFeedNext - ";
	cout << EntitiesManager::getInstance()->ArticleGetByFeedNext( 5 );
	cout << endl;

	cout << endl;
	cout << "[action] FeedDelete - ";
	cout << EntitiesManager::getInstance()->FeedDelete( 0 );
	cout << endl;

	/*
	// Con categorias todavia no puedo probar nada

	cout << "[action] ArticleChangeReadState - ";
	cout << EntitiesManager::getInstance()->ArticleChangeReadState( 1, 1 );
	cout << endl;

	cout << "[action] ArticleChangeFavState - ";
	cout << EntitiesManager::getInstance()->ArticleChangeFavState( 1, 1 );
	cout << endl;

	cout << "[action] ArticleApproveTag - ";
	cout << EntitiesManager::getInstance()->ArticleApproveTag( t_idfeed feedId, t_idart artId, t_idcat tagId );
	cout << endl;

	cout << "[action] ArticleGetByTags - ";
	cout << EntitiesManager::getInstance()->ArticleGetByTags( vector< t_idcat > tagIds, vector< bool > state, t_idart quantity );
	cout << endl;

	cout << "[action] ArticleGetByTagsNext - ";
	cout << EntitiesManager::getInstance()->ArticleGetByTagsNext( t_idart quantity );
	cout << endl;

	cout << "[action] ArticleGetUnclassified - ";
	cout << EntitiesManager::getInstance()->ArticleGetUnclassified( t_idart quantity );
	cout << endl;

	cout << "[action] ArticleGetUnclassifiedNext - ";
	cout << EntitiesManager::getInstance()->ArticleGetUnclassifiedNext( t_idart quantity );
	cout << endl;

	cout << "[action] ArticleGetUnread - ";
	cout << EntitiesManager::getInstance()->ArticleGetUnread( t_idart quantity );
	cout << endl;

	cout << "[action] ArticleGetUnreadNext - ";
	cout << EntitiesManager::getInstance()->ArticleGetUnreadNext( t_idart quantity );
	cout << endl;

	cout << "[action] ArticleGetFavourites - ";
	cout << EntitiesManager::getInstance()->ArticleGetFavourites( t_idart quantity );
	cout << endl;

	cout << "[action] ArticleGetFavouritesNext - ";
	cout << EntitiesManager::getInstance()->ArticleGetFavouritesNext( t_idart quantity );
	cout << endl;

	cout << "[action] ArticleLinkTag - ";
	cout << EntitiesManager::getInstance()->ArticleLinkTag( t_idfeed feedId, t_idart artId, t_idcat tagId );
	cout << endl;

	cout << "[action] ArticleUnLinkTag - ";
	cout << EntitiesManager::getInstance()->ArticleUnLinkTag( t_idfeed feedId, t_idart artId, t_idcat tagId );
	cout << endl;
	*/

	cout << "Adios" << endl;
	return 0;
}

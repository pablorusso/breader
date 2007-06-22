#ifndef FEEDHANDLER
#define FEEDHANDLER

#include "eFeedHandler.h"
#include "Archivo2.h"
#include "Archivo6.h"
#include "Articulo.h"
#include <queue>
#include <map>
#include <cstdio>

// BORRAR
#include <iostream>
// BORRAR

typedef std::queue<Articulo> t_cola_art; //!< el tipo COLA_ARTICULOS
typedef std::pair<t_idart, t_timestamp> t_emap_ultCat; 
  //!< el tipo "elemento" del map de getUltimosArticulosCat()
typedef std::map<t_idfeed, t_emap_ultCat> t_map_ultCat;
  //!< un mapa para utilizar en getUltimosArticulosCat() y en
  //!< getgetUltimosArticulosNoLeidos()

/**
 * Esta clase se encarga de manejar los feeds, con sus articulos y sus
 * categorias, de manera abstracta. Trata de encapsular a los siguientes
 * archivos: Archivo1_f, Archivo2_f, Archivo_5, Archivo_6
 */
using namespace std;
class feedHandler {
	public:
		/**
		 * Constructor. Inicializa los archivos.
		 * @param MAX_CAT la maxima cantidad de categorias de los feeds
		 */
		feedHandler(const t_idcat &MAX_CAT);

		/**
		 * Destructor
		 */
		~feedHandler();

		/**
		 * Da de alta a un feed. Lo agrega en el Archivo6, y crea sus Archivo1,
		 * Archivo2 y Archivo5 correspondientes
		 * @param uri la uri del feed
		 * @param name el nombre del feed
		 * @return el id del feed asignado
		 * @throw eFeedHandler si el Archivo6 esta corrupto
		 * @throw eFeedHandler si los parametros del feed no son integros
		 */
		t_idfeed altaFeed(const string &uri, const string &name);

		/**
		 * Da de baja a un feed. Lo borra del Archivo6, y borra sus Archivo1,
		 * Archivo2 y Archivo5 correspondientes
		 * @param idfeed el id del feed
		 * @return true si fue dado de baja, false de lo contrario (el feed no
		 *         existia)
		 * @throw eFeedHandler si el Archivo6 esta corrupto
		 */
		bool bajaFeed(const t_idfeed &idfeed);

		/**
		 * Devuelve el feed indicado por idfeed
		 * @return el feed indicado por idfeed
		 * @param idfeed el idfeed a obtener, debe estar en rango
		 * @throw eFeedHandler si el Archivo6 esta corrupto
		 * @throw eFeedHandler si el idfeed esta fuera de rango, o el feed
		 *                  estaba borrado
		 */
		Feed getFeed(const t_idfeed &idfeed);

		/**
		 * Da de alta un articulo en un feed. Se lo agrega en el Archivo1 y en
		 * el Archivo2 correspondientes
		 * @param idfeed el id del feed
		 * @param art el articulo a dar de alta
		 * @return el id del articulo asignado
		 * @throw eFeedHandler si el idfeed no existia
		 * @throw eFeedHandler si el Archivo2 esta corrupto
		 */
 		t_idart altaArticulo(const t_idfeed &idfeed, const Articulo &art);

		/**
		 * Da de alta a todos los articulos de la cola en un feed. Se los
		 *  agrega en el Archivo1 y en el Archivo2 correspondientes
		 * @param idfeed el id del feed
		 * @param c_art la cola de articulos a dar de alta
		 * @return una cola con los ids asignados a los articulos
		 * @throw eFeedHandler si el idfeed no existia
		 * @throw eFeedHandler si el Archivo2 esta corrupto
		 */
		t_cola_idart altaArticulo(const t_idfeed &idfeed,
		  const t_cola_art &c_art);

		/**
		 * Marca un articulo como leido o no.
		 * @param idfeed el id del feed al que pertenece el articulo
		 * @param idart el id del articulo a modificar
		 * @param si_no si se quiere marcarlo como leido o como no leido
		 *              (0=no leido, 1=leido)
		 * @throw eFeedHandler si el idfeed no existia
		 * @throw eFeedHandler si el Archivo2 esta corrupto
		 * @throw eFeedHandler si el idart esta fuera de rango
		 */
		void leerArticulo(const t_idfeed &idfeed, const t_idart &idart,
		  const bool si_no);

		/**
		 * Invierte el valor de lectura, es decir, si estaba leido,
		 * pasa no leido y viceversa.
		 * @param idfeed el id del feed cuyo articulo hay que leer/desleer
		 * @param idart el id del articulo que hay que leer/desleer
		 * @return el articulo con la inversion hecha
		 * @throw eFeedHandler si el idfeed no existia
		 * @throw eFeedHandler si el Archivo2 esta corrupto
		 * @throw eFeedHandler si el idart esta fuera de rango
		 */
		Articulo invertirLecturaArticulo(const t_idfeed &idfeed,
		  const t_idart &idart);

		/**
		 * Invierte el valor del bit de favorito, es decir, si estaba como
		 * favorito deja de estarlo y viceversa.
		 * @param idfeed el id del feed cuyo articulo hay que invertir
		 * @param idart el id del articulo que hay que invertir
		 * @return el articulo con la inversion hecha
		 * @throw eFeedHandler si el idfeed no existia
		 * @throw eFeedHandler si el Archivo2 esta corrupto
		 * @throw eFeedHandler si el idart esta fuera de rango
		 */
		Articulo invertirFavorito(const t_idfeed &idfeed, const t_idart &idart);

		/**
		 * Devuelve el timestamp del ultimo articulo del feed indicado
		 * Nota: devuelve cero si no tenia articulos, como la fecha mas reciente
		 * @param idfeed el id del feed
		 * @return el timestamp del ultimo articulo del feed indicado
		 * @throw eFeedHandler si el idfeed no existia
		 * @throw eFeedHandler si el Archivo2 esta corrupto
		 */
		t_timestamp getUltimaFecha(const t_idfeed &idfeed);

		/**
		 * Devuelve la cantidad de articulos pertenecientes a un feed
		 * @param idfeed el id del feed sobre el cual contar articulos
		 * @return la cantidad de articulos
		 * @throw eFeedHandler si el idfeed no existia
		 */
		t_idart cantidadArticulos(const t_idfeed &idfeed);

		/**
		 * Devuelve una cola de articulos con cant_art articulos no leidos
		 * Nota: trabaja en conjunto con getProximosArticulosNoLeidos()
		 * @param cant_art la cantidad de articulos maxima que contendra la cola
		 * @return una cola con los cant_art mas recientes. Si no hay articulos,
		 *         devuelve una cola vacia
		 * @throw eFeedHandler si el Archivo2 esta corrupto
		 * @throw eFeedHandler si el idfeed no existia
		 */
		t_cola_art getUltimosArticulosNoLeidos(const t_idart &cant_art);

		/**
		 * Devuelve una cola de articulos con los cant_art articulos no leidos
		 * despues del ultimo articulo pedido
		 * Nota: trabaja en conjunto con getUltimosArticulosNoLeidos()
		 * @param cant_art la cantidad de articulos maxima que contendra la cola
		 * @return una cola con los cant_art mas recientes, despues del ultimo
		 *         articulo pedido. Si no hay articulos, devuelve una cola vacia
		 * @throw eFeedHandler si no se habia llamado antes a
		 *                     getUltimosArticulosNoLeidos()
		 * @throw eFeedHandler si el Archivo2 esta corrupto
		 */
		t_cola_art getProximosArticulosNoLeidos(const t_idart &cant_art);

		/**
		 * Devuelve una cola de articulos con los cant_art mas recientes de un
		 * feed
		 * Nota: trabaja en conjunto con getProximosArticulos()
		 * @param idfeed el id del feed cuyos articulos seran devueltos
		 * @param cant_art la cantidad de articulos maxima que contendra la cola
		 * @return una cola con los cant_art mas recientes. Si no hay articulos,
		 *         devuelve una cola vacia
		 * @throw eFeedHandler si el idfeed no existia
		 * @throw eFeedHandler si el Archivo2 esta corrupto
		 */
		t_cola_art getUltimosArticulos(const t_idfeed &idfeed,
		  const t_idart &cant_art);

		/**
		 * Devuelve una cola de articulos con los cant_art mas recientes de un
		 * feed, despues del ultimo articulo pedido, correspondientes al ultimo
		 * feed pedido.
		 * Nota: trabaja en conjunto con getUltimosArticulos()
		 * @param cant_art la cantidad de articulos maxima que contendra la cola
		 * @return una cola con los cant_art mas recientes, despues del ultimo
		 *         articulo pedido. Si no hay articulos, devuelve una cola vacia
		 * @throw eFeedHandler si no se habia llamado antes a
		 *                     getUltimosArticulos()
		 * @throw eFeedHandler si el Archivo2 esta corrupto
		 */
		t_cola_art getProximosArticulos(const t_idart &cant_art);

		/**
		 * Devuelve una cola de articulos con los cant_art mas recientes de una
		 * categoria
		 * Nota: trabaja en conjunto con getProximosArticulos()
		 * @param idcat el id de la categoria cuyos articulos seran devueltos
		 * @param cant_art la cantidad de articulos maxima que contendra la cola
		 * @return una cola con los cant_art mas recientes. Si no hay articulos,
		 *         devuelve una cola vacia
		 * @throw eFeedHandler si el idcat esta fuera de rango
		 * @throw eFeedHandler si el Archivo2 esta corrupto
		 */
		t_cola_art getUltimosArticulosCat(const t_idcat &idcat,
		  const t_idart &cant_art);

		/**
		 * Devuelve una cola de articulos con los cant_art mas recientes de un
		 * feed, despues del ultimo articulo pedido, correspondientes al ultimo
		 * feed pedido.
		 * Nota: trabaja en conjunto con getUltimosArticulos()
		 * @param cant_art la cantidad de articulos maxima que contendra la cola
		 * @return una cola con los cant_art mas recientes, despues del ultimo
		 *         articulo pedido. Si no hay articulos, devuelve una cola vacia
		 * @throw eFeedHandler si no se habia llamado antes a
		 *                     getUltimosArticulos()
		 * @throw eFeedHandler si el Archivo2 esta corrupto
		 */
		t_cola_art getProximosArticulosCat(const t_idart &cant_art);

		/**
		 * Devuelve una cola de articulos con los cant_art mas recientes que
		 * satisfacen la "consulta booleana" (ej: CAT1 AND ~CAT12 AND ~CAT15)
		 * Entonces los parametros indican que categorias participan en los AND
		 * y si participan como SI o como NO (~)
		 * Nota: trabaja en conjunto con getProximosArticulosBool()
		 * @param c_cat un contenedor con los ids de las categorias que
		 *              participan en la "consulta booleana"
		 * @param c_si_no un contenedor con la opcion de "si o no" para cada
		 *                una de las categorias
		 * @param cant_art la cantidad de articulos maxima que contendra la cola
		 * @return una cola con los cant_art que matchean la consulta.
		 *         Si no hay articulos, devuelve una cola vacia.
		 * @throw eFeedHandler si el Archivo2 esta corrupto
		 */
		t_cola_art getUltimosArticulosBool(ContenedorIdCat &c_cat,
		  ContenedorIdCat &c_si_no, const t_idart &cant_art);

		/**
		 * Devuelve una cola de articulos con los cant_art mas recientes de un
		 * feed, que matchean la consulta, despues del ultimo articulo pedido.
		 * Nota: trabaja en conjunto con getUltimosArticulosbool()
		 * @param cant_art la cantidad de articulos maxima que contendra la cola
		 * @return una cola con los cant_art que matchean la consulta.
		 *         Si no hay articulos, devuelve una cola vacia.
		 * @throw eFeedHandler si no se habia llamado antes a
		 *                     getUltimosArticulosBool()
		 * @throw eFeedHandler si el Archivo2 esta corrupto
		 */
		t_cola_art getProximosArticulosBool(const t_idart &cant_art);

		/**
		 * Devuelve una cola de articulos con los cant_art mas recientes que
		 * no estan categorizados (es decir, que no tienen ninguna categoria
		 * manual o que tienen al menos una categoria automatica
		 * Nota: trabaja en conjunto con getProximosArticulosNoCat()
		 * @param cant_art la cantidad de articulos maxima que contendra la cola
		 * @return una cola con los cant_art que no estan categorizados.
		 *         Si no hay articulos, devuelve una cola vacia.
		 * @throw eFeedHandler si el Archivo2 esta corrupto
		 */
		t_cola_art getUltimosArticulosNoCat(const t_idart &cant_art);

		/**
		 * Devuelve una cola de articulos con los cant_art mas recientes de un
		 * feed, que no estan categorizados, despues del ultimo articulo pedido.
		 * Nota: trabaja en conjunto con getUltimosArticulosNoCat()
		 * @param cant_art la cantidad de articulos maxima que contendra la cola
		 * @return una cola con los cant_art que matchean la consulta.
		 *         Si no hay articulos, devuelve una cola vacia.
		 * @throw eFeedHandler si no se habia llamado antes a
		 *                     getUltimosArticulosNoCat()
		 * @throw eFeedHandler si el Archivo2 esta corrupto
		 */
		t_cola_art getProximosArticulosNoCat(const t_idart &cant_art);

		/**
		 * Devuelve una cola con los idfeed del Archivo6
		 * @return una cola con los idfeed del Archivo6
		 * @throw eFeedHandler si el Archivo6 esta corrupto
		 */
		t_cola_idfeeds getColaIdFeeds();

		/**
		 * Clasifica un articulo
		 * @param idfeed el id del feed a clasificar
		 * @param idcat el id de la categoria a clasificar
		 * @param idart el id de la articulo a clasificar
		 * @param si_no si se clasifica o des clasifica
		 * @param usu_pc si es el usuario o la pc el que clasifica
		 * Si usu_pc = 1 -> clasificado por la pc
		 * Si usu_pc = 0 -> clasificado por el usuario
		 * @return el articulo clasificado
		 * @throw eFeedHandler si el idfeed es inexistente
		 * @throw eFeedHandler si el idcat esta fuera de rango
		 * @throw eFeedHandler si el idart esta fuera de rango
		 */
		Articulo clasificarArticulo(const t_idfeed &idfeed, const t_idcat &idcat,
		  const t_idart &idart, const bool si_no, const bool usu_pc);

		/**
		 * Se le da de baja a una categoria, borrando su contenido en el
		 * Archivo5 y en el Archivo2_f
		 * TODO: al "borrarlo" le estoy diciendo que no esta clasificado
		 *         con ese idcat, esta bien???
		 * @param idcat el id de la categoria a borrar
		 * @throw eFeedHandler si el idcat esta fuera de rango
		 * @throw eFeedHandler si el Archivo6 esta corrupto
		 * @throw eFeedHandler si el Archivo2 esta corrupto
		 */
		void bajaCategoria(const t_idcat &idcat);

		/**
		 * Devuelve el factor de registros ocupados de Archivo_5 y Archivo_6
		 * sobre registros totales  (se usara para decidir cuando reestructurar)
		 * @return el factor de registros ocupados sobre registros totales
		 */
		t_usedFactor getUsedFactor() const {
			return this->a6.getUsedFactor();
		}

		/**
		 * Reestructura el Archivo5 y el Acrhivo6, para que no tengan espacio
		 * libre innecesario. Esto cambia los idfeeds y, por lo tanto, los
		 * nombres del Archivo_2_f y del Archivo_1_f
		 * @throw eFeedHandler si el Archivo6 esta corrupto
		 * @throw eFeedHandler si el Archivo2 esta corrupto
		 */
		void reestructurar();

		/**
		 * Reestructura el Archivo5 y el Acrhivo6, para que tengan MAX_CAT
		 * cantidad de ids de categorias. Ademas, tiene el mismo resultado que
		 * reestructurar()
		 * Nota: si NEW_MAX_CAT es menor que MAX_CAT este metodono tiene
		 * efecto. Si NEW_MAX_CAT no es multiplo de 8 se toma el siguiente
		 * multiplo de 8.
		 * @param NEW_MAX_CAT la maxima cantidad de categorias nueva.
		 * @throw eFeedHandler si el Archivo6 esta corrupto
		 * @throw eFeedHandler si el Archivo2 esta corrupto
		 */
		void set_MAX_CAT(const t_idcat &NEW_MAX_CAT);

		/**
		 * Lee una clasificacion de un articulo para un feed en particular
		 * Nota: este metodo lee por quien fue clasificado, para saber si esta
		 *       clasificado o no ir a readCat()
		 * @param idfeed el id del feed a leer
		 * @param idart el id del articulo a leer
		 * @param idcat el idcat a leer
		 * @return por quien fue clasificado (usu_pc)
		 * Si usu_pc = 1 -> clasificado por la pc
		 * Si usu_pc = 0 -> clasificado por el usuario
		 * @throw eFeedHandler si el archivo esta corrupto
		 * @throw eFeedHandler si el idfeed esta fuera de rango
		 */
		bool readUsu_Pc(const t_idfeed &idfeed,const t_idart &idart,
		  const t_idcat &idcat);

	private:
		Archivo6 a6; //!< el Archivo6
		t_idart ultArt; //!< el ultimo articulo devuelto (para usar con
		                //!< getUltimosArticulos)
		t_idfeed feed_ultArt; //!< el idfeed del ultimo articulo devuelto
		                      //!< (para usar con getUltimosArticulos)
		bool ultArt_pedido; //!< para saber si ya se utilizo el metodo
		                    //!< getUltimosArticulos()
		t_idcat idcat_ultCat; //!< el id de la categoria que se consulto en
		                      //!< getUltimosArticulosCat()
		bool ultArtCat_pedido; //!< para saber si ya se utilizo el metodo
		                       //!< getUltimosArticulosCat()
		bool bool_ultArt_pedido; //!< para saber si ya se utilizo el metodo
		                         //!< getUltimosArticulosBool()
		bool no_leido_ultArt_pedido; //!< para saber si ya se utilizo el metodo
		                             //!< getUltimosArticulosNoLeidos()
		bool no_cat_ultArt_pedido; //!< para saber si ya se utilizo el metodo
		                           //!< getUltimosArticulosNoLeidos()
		t_map_ultCat map_ultCat; //!< utilizado en getUltimosArticulosCat()
		                         //!< y en getgetUltimosArticulosNoLeidos()

		ContenedorIdCat bool_c_cat; //!< contenedor de los id_cat utilizado en
		                             //!< getUltimosArticulosBool();
		ContenedorIdCat bool_c_si_no; //!< contenedor de los si_no utilizado en
		                               //!< getUltimosArticulosBool();
		/**
		 * Renombra el Archivo_1_f y el Archivo_2_f
		 * @param idfeed_old el id feed viejo
		 * @param idfeed_old el id feed nuevo
		 */
		static void renameFeed(const t_idfeed &idfeed_old,
		  const t_idfeed &idfeed_new);

		/**
		 * Itera a traves de los id art de un feed buscando uno que no este
		 * leido
		 * @param idfeed el idfeed sobre el cual buscar articulos
		 * @param idart el id del articulo sobre el que se iterara. Este
		 *              parametro es modificado por el metodo, dejandolo en
		 *              un idart que matchee, o en -1 si no hay ningun articulo
		 *              que matchee
		 * @return el nuevo timestamp del articulo (ignorar si idart==-1)
 		 * @throw eFeedHandler si el Archivo2 esta corrupto
		 */
		t_timestamp iterateMapNoLeidos(const t_idfeed &idfeed, t_idart &idart);

		/**
		 * Itera a traves de los id art de un feed buscando uno que este
		 * clasificado con idcat
		 * @param idcat el idcat a buscar
		 * @param idfeed el idfeed sobre el cual buscar articulos
		 * @param idart el id del articulo sobre el que se iterara. Este
		 *              parametro es modificado por el metodo, dejandolo en
		 *              un idart que este clasificado con idcat, o en -1 si
		 *              no hay ningun articulo anterior clasificado con idcat
		 * @return el nuevo timestamp del articulo (ignorar si idart==-1)
 		 * @throw eFeedHandler si el Archivo2 esta corrupto
		 */
		t_timestamp iterateMapCat(const t_idcat &idcat, const t_idfeed &idfeed,
		  t_idart &idart);

		/**
		 * Itera a traves de los id art de un feed buscando uno que matchee
		 * con bool_c_cat y bool_c_si_no
		 * @param idfeed el idfeed sobre el cual buscar articulos
		 * @param idart el id del articulo sobre el que se iterara. Este
		 *              parametro es modificado por el metodo, dejandolo en
		 *              un idart que matchee, o en -1 si no hay ningun articulo
		 *              que matchee
		 * @return el nuevo timestamp del articulo (ignorar si idart==-1)
 		 * @throw eFeedHandler si el Archivo2 esta corrupto
		 */
		t_timestamp iterateMapBool(const t_idfeed &idfeed, t_idart &idart);

		/**
		 * Itera a traves de los id art de un feed buscando uno que no este
		 * categorizado
		 * @param idfeed el idfeed sobre el cual buscar articulos
		 * @param idart el id del articulo sobre el que se iterara. Este
		 *              parametro es modificado por el metodo, dejandolo en
		 *              un idart que matchee, o en -1 si no hay ningun articulo
		 *              que matchee
		 * @return el nuevo timestamp del articulo (ignorar si idart==-1)
 		 * @throw eFeedHandler si el Archivo2 esta corrupto
		 */
		t_timestamp iterateMapNoCat(const t_idfeed &idfeed, t_idart &idart);

		/**
		 * Constructor copia, privado, para prevenir descuidos
		 */
		feedHandler(const feedHandler &);

		/**
		 * Operador= sobrecargado, privado, para prevenir descuidos
		 */
		feedHandler &operator=(const feedHandler &);
};

#endif

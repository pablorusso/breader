#include "feedHandler.h"

feedHandler::feedHandler(const t_idcat &MAX_CAT):a6(MAX_CAT), ultArt(0),
  feed_ultArt(0), idcat_ultCat(0), ultArt_pedido(false) {}

feedHandler::~feedHandler() {}

t_idfeed feedHandler::altaFeed(const string &uri, const string &name) {
	t_idfeed ret;
	// Doy de alta al feed en el Archivo6
	ret = this->a6.addFeed(uri, name);
	// Creo un Archivo2_f vacio, con ese idfeed
	Archivo2 a2(this->a6.get_MAX_CAT(), ret);
	return ret;
}

bool feedHandler::bajaFeed(const t_idfeed &idfeed) {
	bool ret;
	// Lo borro del Archivo6
	if ((ret = this->a6.remFeed(idfeed)) == true) {
		// Borro el Archivo_1_f correspondiente
		Archivo1::del(idfeed);
		Archivo2::del(idfeed);
	}
}

t_idart feedHandler::altaArticulo(const t_idfeed &idfeed, const Articulo &art) {
	t_idart ret;
	if (this->a6.findFeed(idfeed) == true) {
		Archivo2 a2(this->a6.get_MAX_CAT(), idfeed);
		ret = a2.writeArticulo(art);
	} else THROW(eFeedHandler, FH_IDFEED_INEXISTENTE);
	return ret;
}

t_cola_idart feedHandler::altaArticulo(const t_idfeed &idfeed,
  const t_cola_art &c_art) {
	t_cola_idart mic_idart;
	t_cola_art mic_art = c_art;
	if (this->a6.findFeed(idfeed) == true) {
		Archivo2 a2(this->a6.get_MAX_CAT(), idfeed);
		while (!mic_art.empty()) {
			mic_idart.push(a2.writeArticulo(mic_art.front()));
			mic_art.pop();
		}
	} else THROW(eFeedHandler, FH_IDFEED_INEXISTENTE);
	return mic_idart;
}


void feedHandler::leerArticulo(const t_idfeed &idfeed, const t_idart &idart,
  const bool si_no) {
	if (this->a6.findFeed(idfeed) == true) {
		Archivo2 a2(this->a6.get_MAX_CAT(), idfeed);
		a2.writeLeido(idart, si_no);
	} else THROW(eFeedHandler, FH_IDFEED_INEXISTENTE);
}

void feedHandler::invertirLecturaArticulo(const t_idfeed &idfeed,
  const t_idart &idart) {
	if (this->a6.findFeed(idfeed) == true) {
		Archivo2 a2(this->a6.get_MAX_CAT(), idfeed);
		a2.invertirLecturaArticulo(idart);
	} else THROW(eFeedHandler, FH_IDFEED_INEXISTENTE);
}

void feedHandler::invertirFavorito(const t_idfeed &idfeed,
  const t_idart &idart) {
	if (this->a6.findFeed(idfeed) == true) {
		Archivo2 a2(this->a6.get_MAX_CAT(), idfeed);
		a2.invertirFavorito(idart);
	} else THROW(eFeedHandler, FH_IDFEED_INEXISTENTE);
}

t_idart feedHandler::cantidadArticulos(const t_idfeed &idfeed) {
	t_idart ret;
	if (this->a6.findFeed(idfeed) == true) {
		Archivo2 a2(this->a6.get_MAX_CAT(), idfeed);
		ret = a2.cantidadArticulos();
	} else THROW(eFeedHandler, FH_IDFEED_INEXISTENTE);
	return ret;
}

t_cola_art feedHandler::getUltimosArticulos(const t_idfeed &idfeed,
  const t_idart &cant_art) {
	t_cola_art c_art;
	if (this->a6.findFeed(idfeed) == true) {
		this->ultArt_pedido = true;
		// Seteo el idfeed
		this->feed_ultArt = idfeed;
		Archivo2 a2(this->a6.get_MAX_CAT(), idfeed);
		t_idart idart = a2.cantidadArticulos();
		t_idart count=0;
		while ((count < cant_art) && (idart > 0)) {
			++count;
			c_art.push(a2.readArticulo(--idart));
		}
		this->ultArt = idart;
	} else THROW(eFeedHandler, FH_IDFEED_INEXISTENTE);
	return c_art;
}

t_cola_art feedHandler::getProximosArticulos(const t_idart &cant_art) {
	t_cola_art c_art;
	if (this->a6.findFeed(this->feed_ultArt) == true) {
		if (this->ultArt_pedido) {
			Archivo2 a2(this->a6.get_MAX_CAT(), this->feed_ultArt);
			t_idart idart=this->ultArt;
			if (idart > 0) {
				t_idart count=0;
				while ((count < cant_art) && (idart > 0)) {
					++count;
					c_art.push(a2.readArticulo(--idart));
				}
				this->ultArt = idart;
			}
		} else THROW(eFeedHandler, FH_PROXIMOS_ART_SIN_ULTIMOS);
	} else THROW(eFeedHandler, FH_IDFEED_INEXISTENTE);
	return c_art;
}


void feedHandler::iterateMapCat(const t_idcat &idcat, const t_idfeed &idfeed,
  t_idart &idart) {
	Archivo2 a2(this->a6.get_MAX_CAT(), idfeed);
	bool cat = false;
	while ((!cat) && (idart!=-1))
		cat = a2.readCat(idart--, idcat);
	if (cat) ++idart;
}

t_cola_art feedHandler::getUltimosArticulosCat(const t_idcat &idcat,
  const t_idart &cant_art) {
	t_cola_art c_art;
	if (idcat < this->a6.get_MAX_CAT()) {
		// Seteo los atributos para que se puedan utilizar en
		// getProximosArticulosCat()
		this->ultArtCat_pedido = true;
		this->idcat_ultCat = idcat;
		// Obtengo los idfeeds y creo un map con (idfeed, idart, timestamp)
		// donde idart es el id del ultimo articulo del feed correspondiente, y
		// si no hay articulos ni se agrega el feed (lo borro despues)
		// Tambien inicializo el timestamp
		t_cola_idfeeds c_idfeeds = this->a6.getColaIdFeeds();
		while (!c_idfeeds.empty()) {
			Archivo2 a2(this->a6.get_MAX_CAT(), c_idfeeds.front());
			t_idart numArt = a2.cantidadArticulos();
			if (numArt > 0)
				this->map_ultCat[c_idfeeds.front()] = t_emap_ultCat(numArt-1,
				   a2.readTimestamp(numArt-1));
			c_idfeeds.pop();
		}
		// Recorro una vez el map, para apuntar los idart a articulos que esten
		// clasificados con idcat. Si un feed no tiene articulos clasificados
		// con idcat, lo elimino del map
		t_map_ultCat::iterator it = this->map_ultCat.begin();
		while (it!=this->map_ultCat.end()) {
			iterateMapCat(idcat, it->first, it->second.first);
			if (it->second.first == -1) {
				// Tengo que tener cuidado al borrar un elemento
				t_map_ultCat::iterator it2 = it++;
				this->map_ultCat.erase(it2);
			}
			else ++it;
		}
		// Hacer hasta que hayan encontrado todos los cant_art necesarios, o
		// hasta que el map este vacio (no tengo suficientes matches para
		// llenar la cola)
		while ((c_art.size() < cant_art) && (!this->map_ultCat.empty())) {
			// Recorro el map para buscar el menor timestamp
			t_timestamp max_timestamp= 0;
			t_idfeed max_idfeed; // el idfeed correspondiente al max timestamp
			for (it=this->map_ultCat.begin(); it!=this->map_ultCat.end(); ++it){
				if (it->second.second > max_timestamp) {
					max_timestamp = it->second.second;
					max_idfeed = it->first;
				}
			}
			// Agrego el art con mayor timestamp a la cola
			Archivo2 a2(this->a6.get_MAX_CAT(), max_idfeed);
			c_art.push(a2.readArticulo(this->map_ultCat[max_idfeed].first));
			// Disminuyo el idart correspondiente, y busco el proximo idart
			// que matchee con idcat
			this->iterateMapCat(idcat, max_idfeed,
			  --this->map_ultCat[max_idfeed].first);
			if (this->map_ultCat[max_idfeed].first == -1)
			  this->map_ultCat.erase(max_idfeed);
		}
		
	} else THROW(eFeedHandler, FH_IDCAT_FUERA_DE_RANGO);
	return c_art;
}

t_cola_art feedHandler::getProximosArticulosCat(const t_idart &cant_art) {
	t_cola_art c_art;
	if (this->ultArtCat_pedido) {
		// Hacer hasta que haya encontrado todos los cant_art necesarios, o
		// hasta que el map este vacio (no tengo suficientes matches para
		// llenar la cola)
		while ((c_art.size() < cant_art) && (!this->map_ultCat.empty())) {
			// Recorro el map para buscar el menor timestamp
			t_timestamp max_timestamp= 0;
			t_idfeed max_idfeed; // el idfeed correspondiente al max timestamp
			t_map_ultCat::iterator it;
			for (it=this->map_ultCat.begin(); it!=this->map_ultCat.end(); ++it){
				if (it->second.second > max_timestamp) {
					max_timestamp = it->second.second;
					max_idfeed = it->first;
				}
			}
			// Agrego el art con mayor timestamp a la cola
			Archivo2 a2(this->a6.get_MAX_CAT(), max_idfeed);
			c_art.push(a2.readArticulo(this->map_ultCat[max_idfeed].first));
			// Disminuyo el idart correspondiente, y busco el proximo idart
			// que matchee con idcat
			this->iterateMapCat(this->idcat_ultCat, max_idfeed,
				--this->map_ultCat[max_idfeed].first);
			if (this->map_ultCat[max_idfeed].first == -1)
				this->map_ultCat.erase(max_idfeed);
		}
	} else THROW(eFeedHandler, FH_PROXIMAS_CAT_SIN_ULTIMAS);
	return c_art;
}

void feedHandler::bajaCategoria(const t_idcat &idcat) {
	if (idcat < this->a6.get_MAX_CAT()) {
		this->a6.bajaCategoria(idcat);
		t_cola_idfeeds c_idfeeds = this->getColaIdFeeds();
		while (!c_idfeeds.empty()) {
			Archivo2 a2(this->a6.get_MAX_CAT(), c_idfeeds.front());
			a2.bajaCategoria(idcat);
			c_idfeeds.pop();
		}
	} else THROW(eFeedHandler, FH_IDCAT_FUERA_DE_RANGO);
}

void feedHandler::clasificarArticulo(const t_idfeed &idfeed, const t_idcat
  &idcat, const t_idart &idart, const bool si_no, const bool usu_pc) {
	if (idcat < this->a6.get_MAX_CAT()) {
		if (idfeed < this->a6.findFeed(idfeed)) {
			Archivo2 a2(this->a6.get_MAX_CAT(), idfeed);
			if (idart < a2.cantidadArticulos()) {
				a2.writeCat(idart, idcat, si_no, usu_pc);
				this->a6.catFeed(idfeed, idcat, si_no);
			} else THROW(eFeedHandler, FH_IDART_FUERA_DE_RANGO);
		} else THROW(eFeedHandler, FH_IDFEED_INEXISTENTE);
	} else THROW(eFeedHandler, FH_IDCAT_FUERA_DE_RANGO);
}

#include "feedHandler.h"

feedHandler::feedHandler(const t_idcat &MAX_CAT): a6(MAX_CAT),
  ultArt(0), feed_ultArt(0), idcat_ultCat(0), ultArt_pedido(false) {}

feedHandler::~feedHandler() {}

t_idfeed feedHandler::altaFeed(const string &uri, const string &name) {
	t_idfeed ret;
	try {
		// Doy de alta al feed en el Archivo6
		ret = this->a6.addFeed(uri, name);
		// Creo un Archivo2_f vacio, con ese idfeed
		Archivo2 a2(this->a6.get_MAX_CAT(), ret);
	}
	catch (IException &e) {
		eFeedHandler mie(e.getErrorMensaje());
		throw(mie);
	}
	return ret;
}

bool feedHandler::bajaFeed(const t_idfeed &idfeed) {
	bool ret;
	try {
		// Lo borro del Archivo6
		if ((ret = this->a6.remFeed(idfeed)) == true) {
			// Borro el Archivo_1_f correspondiente
			Archivo1::del(idfeed);
			Archivo2::del(idfeed);
		}
	}
	catch (IException &e) {
		eFeedHandler mie(e.getErrorMensaje());
		throw(mie);
	}

	return ret;
}

t_idart feedHandler::altaArticulo(const t_idfeed &idfeed, const Articulo &art) {
	t_idart ret;
	try {
		Archivo2 a2(this->a6.get_MAX_CAT(), idfeed);
		ret = a2.writeArticulo(art);
	}
	catch (IException &e) {
		eFeedHandler mie(e.getErrorMensaje());
		throw(mie);
	}
	return ret;
}

t_cola_idart feedHandler::altaArticulo(const t_idfeed &idfeed,
  const t_cola_art &c_art) {
	t_cola_idart mic_idart;
	try {
		t_cola_art mic_art = c_art;
		Archivo2 a2(this->a6.get_MAX_CAT(), idfeed);
		while (!mic_art.empty()) {
			mic_idart.push(a2.writeArticulo(mic_art.front()));
			ContenedorIdCat c = mic_art.front().get_cont_idcat();
			this->a6.catFeed(idfeed, c);
			mic_art.pop();
		}
	}
	catch (IException &e) {
		eFeedHandler mie(e.getErrorMensaje());
		throw(mie);
	}
	return mic_idart;
}


void feedHandler::leerArticulo(const t_idfeed &idfeed, const t_idart &idart,
  const bool si_no) {
	try {
		Archivo2 a2(this->a6.get_MAX_CAT(), idfeed);
		a2.writeLeido(idart, si_no);
	}
	catch (IException &e) {
		eFeedHandler mie(e.getErrorMensaje());
		throw(mie);
	}
}

void feedHandler::invertirLecturaArticulo(const t_idfeed &idfeed,
  const t_idart &idart) {
	try {
		Archivo2 a2(this->a6.get_MAX_CAT(), idfeed);
		a2.invertirLecturaArticulo(idart);
	}
	catch (IException &e) {
		eFeedHandler mie(e.getErrorMensaje());
		throw(mie);
	}
}

void feedHandler::invertirFavorito(const t_idfeed &idfeed,
  const t_idart &idart) {
	try {
		Archivo2 a2(this->a6.get_MAX_CAT(), idfeed);
		a2.invertirFavorito(idart);
		//this->a6.invertirFavorito(idfeed); //TODO
	}
	catch (IException &e) {
		eFeedHandler mie(e.getErrorMensaje());
		throw(mie);
	}
}

t_idart feedHandler::cantidadArticulos(const t_idfeed &idfeed) {
	t_idart ret;
	try {
		Archivo2 a2(this->a6.get_MAX_CAT(), idfeed);
		ret = a2.cantidadArticulos();
	}
	catch (IException &e) {
		eFeedHandler mie(e.getErrorMensaje());
		throw(mie);
	}
	return ret;
}

t_cola_art feedHandler::getUltimosArticulos(const t_idfeed &idfeed,
  const t_idart &cant_art) {
	t_cola_art c_art;
	try {
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
	}
	catch (IException &e) {
		eFeedHandler mie(e.getErrorMensaje());
		throw(mie);
	}
	return c_art;
}

t_cola_art feedHandler::getProximosArticulos(const t_idart &cant_art) {
	t_cola_art c_art;
	if (this->ultArt_pedido) {
		try {
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
		}
		catch (IException &e) {
			eFeedHandler mie(e.getErrorMensaje());
			throw(mie);
		}
	} else {
		eFeedHandler mie("Se pidieron los proximos articulos sin pedir los ultimos");
		throw(mie);
	}
	return c_art;
}

void feedHandler::iterateMapCat(const t_idcat &idcat, const t_idfeed &idfeed,
  t_idart &idart) {
	try {
		Archivo2 a2(this->a6.get_MAX_CAT(), idfeed);
		bool cat = false;
		while ((!cat) && (idart!=-1))
			cat = a2.readCat(idart--, idcat);
		if (cat) ++idart;
	}
	catch (IException &e) {
		eFeedHandler mie(e.getErrorMensaje());
		throw(mie);
	}
}

t_cola_art feedHandler::getUltimosArticulosCat(const t_idcat &idcat,
  const t_idart &cant_art) {
	t_cola_art c_art;
	try {
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
	}
	catch (IException &e) {
		eFeedHandler mie(e.getErrorMensaje());
		throw(mie);
	}
	return c_art;
}

t_cola_art feedHandler::getProximosArticulosCat(const t_idart &cant_art) {
	t_cola_art c_art;
	if (this->ultArtCat_pedido) {
		try {
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
		}
		catch (IException &e) {
			eFeedHandler mie(e.getErrorMensaje());
			throw(mie);
		}
	} else {
		eFeedHandler mie("Se pidieron los proximos articulos sin pedir los ultimos");
		throw(mie);
	}

	return c_art;
}

void feedHandler::clasificarArticulo(const t_idfeed &idfeed, const t_idcat
  &idcat, const t_idart &idart, const bool si_no, const bool usu_pc) {
	try {
		Archivo2 a2(this->a6.get_MAX_CAT(), idfeed);
		a2.writeCat(idart, idcat, si_no, usu_pc);
		if (si_no == 1) this->a6.catFeed(idfeed, idcat, 1);
		else {} //TODO
	}
	catch (IException &e) {
		eFeedHandler mie(e.getErrorMensaje());
		throw(mie);
	}	
}

void feedHandler::bajaCategoria(const t_idcat &idcat) {
	try {
		this->a6.bajaCategoria(idcat);
		t_cola_idfeeds c_idfeeds = this->getColaIdFeeds();
		while (!c_idfeeds.empty()) {
			Archivo2 a2(this->a6.get_MAX_CAT(), c_idfeeds.front());
			a2.bajaCategoria(idcat);
			c_idfeeds.pop();
		}
	}
	catch (IException &e) {
		eFeedHandler mie(e.getErrorMensaje());
		throw(mie);
	}	
}

void feedHandler::reestructurar() {
	try {
		{ // Este scope es para que se cierre el a6_bis
			// Creo un Archivo6 y un Archivo5 nuevos
			Archivo6 a6_bis(this->a6.get_MAX_CAT(), 1);
			t_cola_idfeeds c_idfeed = this->a6.getColaIdFeeds();
			while (!c_idfeed.empty()) {
				Feed feed = this->a6.getFeed(c_idfeed.front());
				t_idfeed idfeed_old = c_idfeed.front();
				t_idfeed idfeed_new = a6_bis.addFeed(feed);
				feedHandler::renameFeed(idfeed_old, idfeed_new);
				c_idfeed.pop();
			}
		}
		// Borro los archivos viejos y renombro los nuevos
		::remove(A6_PATH);
		::rename(A6_PATH_BIS, A6_PATH);
		::remove(A5_PATH);
		::rename(A5_PATH_BIS, A5_PATH);
		//Reabro el archivo6 (y el archivo5)
		this->a6.reopen();
	}
	catch (IException &e) {
		eFeedHandler mie(e.getErrorMensaje());
		throw(mie);
	}	

}

void feedHandler::set_MAX_CAT(const t_idcat &NEW_MAX_CAT) {
	try {
		if (NEW_MAX_CAT > this->a6.get_MAX_CAT()) {
			t_idcat MY_NEW_MAX_CAT = NEW_MAX_CAT;
			t_idcat m = NEW_MAX_CAT % 8;
			if (m!=0) MY_NEW_MAX_CAT += (8-m);
			{ // Este scope es para que se cierre el a6_bis
				// Creo un Archivo6 y un Archivo5 nuevos, con capacidad para
				// NEW_MAX_CAT
				Archivo6 a6_bis(MY_NEW_MAX_CAT, 1);
				t_cola_idfeeds c_idfeed = this->a6.getColaIdFeeds();
				while (!c_idfeed.empty()) {
					Feed feed = this->a6.getFeed(c_idfeed.front());
					feed.set_MAX_CAT(MY_NEW_MAX_CAT);
					t_idfeed idfeed_old = c_idfeed.front();
					t_idfeed idfeed_new = a6_bis.addFeed(feed);
					feedHandler::renameFeed(idfeed_old, idfeed_new);
	
				{ // Scope para que se cierren los Archivos2
					Archivo2 a2_old(this->a6.get_MAX_CAT(), idfeed_old);
					Archivo2 a2_new(MY_NEW_MAX_CAT, idfeed_new, 1);
	
					t_idart count=0;
					t_idart max = a2_old.cantidadArticulos();
					while (count < max) {
						Articulo art = a2_old.readArticulo(count);
						art.set_MAX_CAT(MY_NEW_MAX_CAT);
						a2_new.writeArticulo(art);
						++count;
					}
				}
	
					string fileNameOld = Archivo2::genFileName(idfeed_old);
					string fileNameNewBis = Archivo2::genFileName(idfeed_new, 1);
	
					::remove(fileNameOld.c_str());
					::rename(fileNameNewBis.c_str(), fileNameOld.c_str());
	
					fileNameOld = Archivo1::genFileName(idfeed_old);
					fileNameNewBis = Archivo1::genFileName(idfeed_new, 1);
					::remove(fileNameOld.c_str());
					::rename(fileNameNewBis.c_str(), fileNameOld.c_str());
	
					c_idfeed.pop();
				}
			}
	/*
	cout << "STOP" << endl;
	string tmp;
	cin >> tmp;
	*/
	
			// Borro los archivos viejos y renombro los nuevos
			::remove(A6_PATH);
			::rename(A6_PATH_BIS, A6_PATH);
			::remove(A5_PATH);
			::rename(A5_PATH_BIS, A5_PATH);
			//Reabro el archivo6 (y el archivo5)
			this->a6.reopen();
			this->a6.set_MAX_CAT(MY_NEW_MAX_CAT); // TODO mejorar parchecito
		}
	}
	catch (IException &e) {
		eFeedHandler mie(e.getErrorMensaje());
		throw(mie);
	}	
}

void feedHandler::renameFeed(const t_idfeed &idfeed_old,
  const t_idfeed &idfeed_new) {
	try {
		// Primero renombro el Archivo_2
		string fn_old = Archivo2::genFileName(idfeed_old);
		string fn_new = Archivo2::genFileName(idfeed_new);
		::rename(fn_old.c_str(), fn_new.c_str());
		// Ahora renombro el Archivo_1
		fn_old = Archivo1::genFileName(idfeed_old);
		fn_new = Archivo1::genFileName(idfeed_new);
		::rename(fn_old.c_str(), fn_new.c_str());
	}
	catch (IException &e) {
		eFeedHandler mie(e.getErrorMensaje());
		throw(mie);
	}	
}

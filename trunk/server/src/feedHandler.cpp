#include "feedHandler.h"

feedHandler::feedHandler(const t_idcat &MAX_CAT): a6(MAX_CAT),
  ultArt(0), feed_ultArt(0), idcat_ultCat(0), ultArt_pedido(false),
  bool_ultArt_pedido(false), bool_c_cat(MAX_CAT), bool_c_si_no(MAX_CAT) {}

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
		Articulo miart = art;
		miart.set_MAX_CAT(this->a6.get_MAX_CAT());
		Archivo2 a2(this->a6.get_MAX_CAT(), idfeed);
		ret = a2.writeArticulo(miart);

		ContenedorIdCat catArt = miart.get_cont_idcat();
		ContenedorIdCat catFeed = (this->a6.getFeed(idfeed)).getContIdCat();
		catFeed.catOR(catArt);
		this->a6.catFeed(idfeed, catArt);
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
		ContenedorIdCat catArt(this->a6.get_MAX_CAT());
		t_cola_art mic_art = c_art;
		Archivo2 a2(this->a6.get_MAX_CAT(), idfeed);
		while (!mic_art.empty()) {
			Articulo miart = mic_art.front();
			miart.set_MAX_CAT(this->a6.get_MAX_CAT());
			mic_idart.push(a2.writeArticulo(miart));
			ContenedorIdCat catArt_tmp = miart.get_cont_idcat();
			catArt.catOR(catArt_tmp);
			mic_art.pop();
		}
		ContenedorIdCat catFeed = (this->a6.getFeed(idfeed)).getContIdCat();
		catFeed.catOR(catArt);
		this->a6.catFeed(idfeed, catFeed);
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

t_timestamp feedHandler::getUltimaFecha(const t_idfeed &idfeed) {
	t_timestamp ret = 0;
	try {
		Archivo2 a2(this->a6.get_MAX_CAT(), idfeed);
		t_idart count = a2.cantidadArticulos();
		if (count > 0) ret = a2.readTimestamp(count-1);
	}
	catch (IException &e) {
		eFeedHandler mie(e.getErrorMensaje());
		throw(mie);
	}
	return ret;
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
		eFeedHandler mie("Se pidieron los proximos articulos sin pedir los \
		                  ultimos");
		throw(mie);
	}
	return c_art;
}

t_timestamp feedHandler::iterateMapCat(const t_idcat &idcat,
  const t_idfeed &idfeed, t_idart &idart) {
	t_timestamp timestamp;
	try {
		Archivo2 a2(this->a6.get_MAX_CAT(), idfeed);
		bool cat = false;
		bool end = false;
		while ( !cat && !end )
		{
			cat = a2.readCat( idart, idcat );
			end = idart == 0;
			if ( !end ) idart--;
		}
		if (cat) timestamp = a2.readTimestamp(++idart);
	}
	catch (IException &e) {
		eFeedHandler mie(e.getErrorMensaje());
		throw(mie);
	}
	return timestamp;
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
			it->second.second=iterateMapCat(idcat,it->first,it->second.first);
			if ( it->second.first == (t_idart)-1 )
			{
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
			it->second.second = this->iterateMapCat(idcat, max_idfeed,
				--this->map_ultCat[max_idfeed].first);
			if (this->map_ultCat[max_idfeed].first == (t_idart)-1)
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
			// Hacer hasta que hayan encontrado todos los cant_art necesarios, o
			// hasta que el map este vacio (no tengo suficientes matches para
			// llenar la cola)
			while ((c_art.size() < cant_art) && (!this->map_ultCat.empty())) {
				// Recorro el map para buscar el menor timestamp
				t_timestamp max_timestamp= 0;
				t_idfeed max_idfeed; // el idfeed del max timestamp
				t_map_ultCat::iterator it;
				for (it=this->map_ultCat.begin(); it!=this->map_ultCat.end();
				  ++it){
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
				it->second.second = this->iterateMapCat(this->idcat_ultCat,
				  max_idfeed, --this->map_ultCat[max_idfeed].first);
				if (this->map_ultCat[max_idfeed].first == (t_idart)-1)
					this->map_ultCat.erase(max_idfeed);
			}
		}
		catch (IException &e) {
			eFeedHandler mie(e.getErrorMensaje());
			throw(mie);
		}
	} else {
		eFeedHandler mie("Se pidieron los proximos articulos sin pedir los \
		                  ultimos");
		throw(mie);
	}

	return c_art;
}

t_timestamp feedHandler::iterateMapBool(const t_idfeed &idfeed, t_idart &idart){
	t_timestamp timestamp;
	try {
		Archivo2 a2(this->a6.get_MAX_CAT(), idfeed);
		bool match = false;
		bool end = false;
		while ( !match && !end )
		{
			ContenedorIdCat c = a2.readCat(idart);
			match = c.match(this->bool_c_cat, this->bool_c_si_no);
			end = idart == 0;
			if ( !end ) idart--;
		}
		if (match) timestamp = a2.readTimestamp(++idart);
	}
	catch (IException &e) {
		eFeedHandler mie(e.getErrorMensaje());
		throw(mie);
	}
	return timestamp;
}

t_cola_art feedHandler::getUltimosArticulosBool(ContenedorIdCat &c_cat,
  ContenedorIdCat &c_si_no, const t_idart &cant_art) {
	t_cola_art c_art;
	try {
		// Seteo los atributos para que se puedan utilizar en
		// getProximosArticulosBool()
		this->bool_ultArt_pedido = true;
		this->bool_c_cat = c_cat;
		this->bool_c_si_no = c_si_no;
		this->bool_c_cat.set_MAX_CAT(this->a6.get_MAX_CAT());
		this->bool_c_si_no.set_MAX_CAT(this->a6.get_MAX_CAT());


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

		// Recorro una vez el map, para apuntar los idart a articulos que
		// matcheen la consulta. Si un feed no tiene articulos que lo hagan,
		// con idcat, lo elimino del map
		t_map_ultCat::iterator it = this->map_ultCat.begin();
		while (it!=this->map_ultCat.end()) {
			it->second.second = this->iterateMapBool(it->first,
			  it->second.first);
			if (it->second.first == (t_idart)-1) {
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
			it->second.second = this->iterateMapBool(max_idfeed,
			  --this->map_ultCat[max_idfeed].first);
			if (this->map_ultCat[max_idfeed].first == (t_idart)-1)
				this->map_ultCat.erase(max_idfeed);
		}
	}
	catch (IException &e) {
		eFeedHandler mie(e.getErrorMensaje());
		throw(mie);
	}
	return c_art;
}

t_cola_art feedHandler::getProximosArticulosBool(const t_idart &cant_art) {
	t_cola_art c_art;
	if (this->ultArtCat_pedido) {
		try {
			// Hacer hasta que haya encontrado todos los cant_art necesarios, o
			// hasta que el map este vacio (no tengo suficientes matches para
			// llenar la cola)
			while ((c_art.size() < cant_art) && (!this->map_ultCat.empty())) {
				// Recorro el map para buscar el menor timestamp
				t_timestamp max_timestamp= 0;
				t_idfeed max_idfeed; // el idfeed del max timestamp
				t_map_ultCat::iterator it;
				for (it=this->map_ultCat.begin(); it!=this->map_ultCat.end();
				  ++it){
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
				it->second.second = this->iterateMapCat(this->idcat_ultCat,
				  max_idfeed, --this->map_ultCat[max_idfeed].first);
				if (this->map_ultCat[max_idfeed].first == (t_idart)-1)
					this->map_ultCat.erase(max_idfeed);
			}
		}
		catch (IException &e) {
			eFeedHandler mie(e.getErrorMensaje());
			throw(mie);
		}
	} else {
		eFeedHandler mie("Se pidieron los proximos articulos sin pedir los \
		                  ultimos");
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
		else {
			// Tengo que recorrer el Archivo2 correspondiente, para saber si
			// este articulo que se descategorizo era el ultimo que contenia
			// la categoria, para asi borrarla del Archivo5
			t_idart max = a2.cantidadArticulos();
			t_idart i = 0;
			bool cat=false;
			while ((i<max) && (!cat)) {
				cat = a2.readCat(i, idcat);
			}
			if (!cat) this->a6.catFeed(idfeed, idcat, 0);
		}
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
			this->bool_c_cat.set_MAX_CAT(MY_NEW_MAX_CAT);
			this->bool_c_si_no.set_MAX_CAT(MY_NEW_MAX_CAT);
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

					string fileNameOld=Archivo2::genFileName(idfeed_old);
					string fileNameNewBis=Archivo2::genFileName(idfeed_new,1);

					::remove(fileNameOld.c_str());
					::rename(fileNameNewBis.c_str(), fileNameOld.c_str());

					fileNameOld = Archivo1::genFileName(idfeed_old);
					fileNameNewBis = Archivo1::genFileName(idfeed_new, 1);
					::remove(fileNameOld.c_str());
					::rename(fileNameNewBis.c_str(), fileNameOld.c_str());

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

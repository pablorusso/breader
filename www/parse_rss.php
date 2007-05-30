<?
	require_once( 'magpierss/rss_fetch.inc' );


	if (!ini_get("register_globals"))
	{
    	import_request_variables('GPC');
	}
	// http://localhost/breader/parse_rss.php?feedUrl=http%3A//magpierss.sf.net/test.rss&mustCreate=0&lastUpdate=

	$rss = fetch_rss( $feedUrl );
	if ($rss)
	{
		$feedName = $rss->channel['title'];
		if ( $mustCreate == "1" ) //alta de feed
		{
			// dar de alta el feed usando $feedUrl y $feedName
			echo "ALTA DE FEED: ".$feedName;
		}

		foreach ($rss->items as $item)
		{
			$date_timestamp = $item['date_timestamp'];
			if ( $date_timestamp > $lastUpdate )
			{
				$title = $item['title'];
				$atom_content = $item['atom_content'];
				$link = $item['link'];
				$dc_creator = $item['dc']['creator'];
				// Dar de alta el articulo usando $feedName y los datos de articulo
				echo "ALTA DE ARTICULO: ".$title." ".$link." ".$date_timestamp;
			}
		}

		// retorno el xml del feed creado o actualizado
	}
	else
	{
		echo "Error: " . magpie_error();
	}

	//echo "<test>";
	//echo $feedUrl;
	//echo "</test>";
	//echo "<response><status error=\"0\" message=\"\"/>";
	//echo "<feed id=\"3\" name=\"".$feedUrl."\" lastUpdate=\"May 22, 2007 3:29 AM\" url=\"http://www.blogger.com/rss/atom.xml\"/>";
	//echo "</response>"
?>


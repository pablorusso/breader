<?
	require_once( 'magpierss/rss_fetch.inc' );
	require_once( 'magpierss/rss_parse.inc' );

	if (!ini_get("register_globals"))
	{
    	import_request_variables('GPC');
	}

	$artXML = "";
	$rss = fetch_rss( $feedUrl );
	if ($rss)
	{
		$feedName = $rss->channel['title'];
		foreach ($rss->items as $item)
		{
			$date_timestamp = $item['date_timestamp'];
			if ( $date_timestamp > $lastUpdate || $lastUpdate == "" )
			{
				$title = $item['title'];
				if ( $item['atom_content'] != "" )
					$atom_content = $item['atom_content']; // atom, no lo encodeo porque tiene que venir bien formateado
				else
					$atom_content = $item['summary']; // rss

				$link = $item['link'];

				if ( $item['dc']['creator'] != null )
					$dc_creator = $item['dc']['creator'];  // atom
				else
					$dc_creator = $item['author']; // rss

				$article = "<article feedId=\"".htmlspecialchars($feedId, ENT_QUOTES)."\" feedName=\"".htmlspecialchars($feedName, ENT_QUOTES)."\" title=\"".htmlspecialchars($title, ENT_QUOTES)."\" link=\"".htmlspecialchars($link, ENT_QUOTES)."\" author=\"".htmlspecialchars($dc_creator, ENT_QUOTES)."\" date=\"".htmlspecialchars($date_timestamp, ENT_QUOTES)."\">";
				$article = $article."<summary>".htmlspecialchars($atom_content, ENT_QUOTES)."</summary>";
				$article = $article."</article>";

				$artXML = $article.$artXML;

				//var_dump( $item );
			}
		}
		$errorN   = "0";
		$error    = "";
	}
	else
	{
		$errorN   = "1";
		$error    = magpie_error();
	}

	header('Content-Type: text/xml');
	echo "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>";
	echo "<response><status error=\"".htmlspecialchars($errorN, ENT_QUOTES)."\" message=\"".htmlspecialchars($error, ENT_QUOTES)."\"/>";
	if ( $errorN == "0" )
		if ( $artXML == "" )
			echo "<articles/>";
		else
			echo "<articles>".$artXML."</articles>";
	echo "</response>"
?>


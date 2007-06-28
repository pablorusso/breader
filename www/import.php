<?php
	require_once( 'magpierss/rss_fetch.inc' );
	if (!ini_get("register_globals"))
	{
    	import_request_variables('GPC');
	}

	$uploaddir = 'uploads/';
	$uploadfile = $uploaddir . basename($_FILES['upfile']['name']);

	$errorN = 0;
	if ( !move_uploaded_file( $_FILES['upfile']['tmp_name'], $uploadfile ) )
	{
		$errorN   = "2";
		$error    = "Error subiendo el archivo.";
	}

	function readFromSocket( $socket, $result )
	{
		$ack = "OK";
		$size = 0;
		$result = "";
		$out = "";

		while ( $out = socket_read( $socket, 10240 ) ) $size = $size.$out;
		socket_write( $socket, $ack, strlen( $ack ) );
		if ( $size > 0 )
		{
			while ( $out = socket_read( $socket, 10240 ) ) $result = $result.$out;
			while ( strlen( $result ) < $size )
			{
				while ( $out = socket_read( $socket, 10240 ) )
					$result = $result.$out;
			}
			socket_write( $socket, $ack, strlen( $ack ) );
		}
		return $result;
	}

	function writeToSocket( $socket, $data )
	{
		$size = strlen( $data );
		$ack  = "OK";

		//echo "SIZE:".$size;
		socket_write( $socket, $size, strlen( $size ) );
		while ( $out = socket_read( $socket, 10240 ) ) $ack = $ack.$out;
		if ( $size > 0 )
		{
			socket_write( $socket, $data, $size );
			while ( $out = socket_read( $socket, 10240 ) ) $ack = $ack.$out;
		}
	}

	function openSocket()
	{
		$service_port = 12000;
		$address = gethostbyname( 'localhost' );
		$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
		if ($socket === false)
		{
			echo "socket_create() failed: reason: " . socket_strerror(socket_last_error()) . "\n";
		}

		$result = socket_connect($socket, $address, $service_port);
		if ($result === false)
		{
			echo "socket_connect() failed.\nReason: ($result) " . socket_strerror(socket_last_error($socket)) . "\n";
		}

		return $socket;
	}

	if ( $errorN == 0 )
	{
		$feeds = '';
		$lines = file( $uploadfile );
		foreach ($lines as $line_num => $line)
		{
			$feedUrl = htmlspecialchars( $line );
			$rss = fetch_rss( $feedUrl );
			$feedName = "";
			if ($rss)
			{
				$feedName = $rss->channel['title'];

				// Informo el alta de feed
				$socket = openSocket();
				$actionCode = "F1";
				writeToSocket( $socket, $actionCode );
				$params = "feedUrl||#".$feedUrl."|||feedName||#".$feedName;
				// Aca se pierde cuelga, como que el server espera otra cosa, no se...
				writeToSocket( $socket, $params );
				$resultCode = "0";
				readFromSocket( $socket, $resultCode );
				$response = "";
				readFromSocket( $socket, $response );
				socket_close($socket);

				/*
				foreach ($rss->items as $item)
				{
					// Dar de alta el articulo para el feed.
					// Como no anduvo siquiera el alta de feed, no seguimos con esto pero es trivial
					// mandar el mensaje. El problema es de comunicacion con sockets.

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
				*/
				$errorN   = "0";
				$error    = "";
			}
			else
			{
				$errorN   = "1";
				$error    = magpie_error();
			}
		}
	}


	//echo.write "<script>document.location.reload();</script>";

	/*header('Content-Type: text/xml');
	echo "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>";
	echo "<response><status error=\"".htmlspecialchars($errorN, ENT_QUOTES)."\" message=\"".htmlspecialchars($error, ENT_QUOTES)."\"/>";
	if ( $errorN == "0" )
		echo "<feeds>".$feeds."</feeds>";
	echo "</response>"*/
?>
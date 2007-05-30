<?
	require_once( 'magpierss/rss_fetch.inc' );

	if (!ini_get("register_globals"))
	{
    	import_request_variables('GPC');
	}

	$rss = fetch_rss( $feedUrl );
	$feedName = "";
	if ($rss)
	{
		$feedName = $rss->channel['title'];
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
		echo "<feed id=\"\" name=\"".htmlspecialchars($feedName, ENT_QUOTES)."\" lastUpdate=\"\" url=\"".htmlspecialchars($feedUrl, ENT_QUOTES)."\"/>";
	echo "</response>"
?>


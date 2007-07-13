<?
	// Convierte un timestamp a una fecha legible,
	// lo hago en php porque desde el xslt no se puede
	if (!ini_get("register_globals"))
    	import_request_variables('GPC');

	echo "<?xml version=\"1.0\"?>";
	echo "<formatted>";
	echo strftime( "%B %e, %Y %R:%S", $timestamp );
	echo "</formatted>";
?>
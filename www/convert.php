<?
// May 18, 2007 9:42 AM
// date - F j, Y G
//        %B %e, %Y %R %Z  string strftime ( string $format [, int $timestamp] )

// 1180478460

	if (!ini_get("register_globals"))
    	import_request_variables('GPC');

	echo "<?xml version=\"1.0\"?>";
	echo "<formatted>";
	echo strftime( "%B %e, %Y %R", $timestamp );
	echo "</formatted>";
?>
<?php
   require_once ('magpierss/rss_utils.inc');
   $feedDate = "2007-06-23T18:56:53+00:00";
   echo "FECHA: ".@parse_w3cdtf( $feedDate );
?>
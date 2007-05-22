<?
require_once('magpierss/rss_fetch.inc');

$url = 'http://localhost/breader/rss/atom.xml';
$rss = fetch_rss( $url );

echo "Site: ", $rss->channel['title'], "<br>\n";

$item = $rss->items[0];

foreach( $item as $i )
	echo "<br>".$i;

echo "<p>----------</br>";
echo "<br>title: ".$item['title'];
echo "<br>link: ".$item['link'];
echo "<br>summary: ".$item['summary'];
echo "<br>description: ".$item['description'];
echo "<br>timestamp: ".$item['date_timestamp'];
echo "<br>content: ".$item['content'];
echo "<br>info: ".$item['info'];
echo "<br>tagline: ".$item['tag'];
echo "<br>copyright: ".$item['copyright'];
echo "<br>----------</p>";


/*foreach ($rss->items as $item ) {
	$title = $item[title];
	$url   = $item[link];
	$url   = $item[summary];

	echo "<p>----------</br>";
	echo "<br>title: ".$item[title];
	echo "<br>link: ".$item[link];
	echo "<br>summary: ".$item[summary];
	echo "<br>description: ".$item[description];
	echo "<br>timestamp: ".$item[date_timestamp];
	echo "<br>content: ".$item[content];
	echo "<br>info: ".$item[info];
	echo "<br>tagline: ".$item[tagline];
	echo "<br>copyright: ".$item[copyright];
	echo "<br>----------</p>";


	//echo "<a href=$url>$title</a></li><br>\n";
}*/
?>
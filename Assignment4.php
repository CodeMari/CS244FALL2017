<?php
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "esp8266asgn1";


$X = json_decode($_GET['X']);
$Y = json_decode($_GET['Y']);
$Z = json_decode($_GET['Z']);

if(!file_exists('HW4Vals.csv')) {
	
		$file = fopen('HW4Vals.csv', 'w');
		fputcsv($file, array('X', 'Y', 'Z'));
	
	} else {
	
$file = fopen('HW4Vals.csv', 'a');
	}
	$size = min(count($X), count($Y));
	
	for($i = 0; $i < $size; $i++){
			fputcsv($file, array($X[$i], $Y[$i],$Z[$i]));	
	}
	
	fclose($file);

?> 
<?php

$servername = "localhost";
$username = "root";
$password = "";
$dbname = "esp8266asgn1";

$IRArray = json_decode($_GET['IR']);
$REDArray = json_decode($_GET['RED']);

if(!file_exists('HW2Vals.csv')) {

	$file = fopen('HW2Vals.csv', 'w');
	fputcsv($file, array('IR', 'RED'));

} else {

	$file = fopen('HW2Vals.csv', 'a');
	
}

$size = min(count($REDArray), count($IRArray));

for($i = 0; $i < $size; $i++){
		fputcsv($file, array($IRArray[$i], $REDArray[$i]));	
}

fclose($file);

?> 

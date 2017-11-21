<?php
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "esp8266asgn1";


$IR = json_decode($_GET["IR"]);
$RED = json_decode($_GET["RED"]);


$X = json_decode($_GET['X']);
$Y = json_decode($_GET['Y']);
$Z = json_decode($_GET['Z']);


$file = fopen('HW6Vals.csv', 'a');

//fputcsv($file, array('IR', 'RED'));
$data = array( array( $IR , $RED, $X, $Y, $Z) );
if(!file_exists('HW6Vals.csv')) {
	
		$file = fopen('HW6Vals.csv', 'w');
		fputcsv($file, array("IR", "RED",'X', 'Y', 'Z'));
	
	} else {
	
$file = fopen('HW6Vals.csv', 'a');
	}
	$size = min(count($X), count($Y));
	
	for($i = 0; $i < $size; $i++){
			fputcsv($file, array($X[$i], $Y[$i],$Z[$i]));	
	}
	
	fclose($file);
foreach($data as $row)
{
	fputcsv($file, $row);
}

fclose($file);



?> 

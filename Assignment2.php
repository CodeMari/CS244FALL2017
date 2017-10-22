<?php
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "esp8266asgn1";

$IR = $_GET["IR"];
$RED = $_GET["RED"];

if(!file_exists('HW2Vals4.csv')) {

	$file = fopen('HW2Vals4.csv', 'w');
	fputcsv($file, array('IR', 'RED'));

} else {

	$file = fopen('HW2Vals4.csv', 'a');
	
}

$data = array( array( $IR , $RED) );

foreach($data as $row){
	fputcsv($file, $row);
}

fclose($file);

?> 

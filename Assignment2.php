 <?php
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "esp8266asgn1";

$IR = $_GET["IR"];
$RED = $_GET["RED"];

$file = fopen('HW2Vals.csv', 'a');

//fputcsv($file, array('IR', 'RED'));
$data = array( array( $IR , $RED) );

foreach($data as $row)
{
	fputcsv($file, $row);
}

fclose($file);

?> 
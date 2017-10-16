<?php
    $servername = "localhost";
    $username = "root";
    $password = "";
    $dbname = "esp8266asgn1";
    
    $val = $_GET["Transmission"];
    $data = array( array( $val ) );
    
    $file = fopen('HW1demo.csv', 'a');
    
    foreach($data as $row)
    {
        fputcsv($file, $row);
    }
    
    fclose($file);
    //echo "The data is " . $data . "<br>";
    //echo "Goodbye<br>"
    
    // Create connection
    /*$conn = new mysqli($servername, $username, $password, $dbname);
     // Check connection
     if ($conn->connect_error) {
     die("Connection failed: " . $conn->connect_error);
     }
     
     $sql = "INSERT INTO homeworkone (Name) VALUES ('$data')";
     
     if ($conn->query($sql) === TRUE) {
     echo "New record created successfully";
     } else {
     echo "Error: " . $sql . "<br>" . $conn->error;
     }
     
     $conn->close();*/
    ?> 

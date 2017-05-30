/**
 * @file bug729.cpp regression case for bug 729 ("PDO prepared statements bug introduced")
 *
 * - execute following PHP script
 * @verbatim
<?php
$options = [
    PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION,
    PDO::ATTR_EMULATE_PREPARES => false,
];

$host=$argv[1];
$port=$argv[2];
$user=$argv[3];
$pass=$argv[4];

$dsn = "mysql:host=".$host.";port=".$port.";dbname=information_schema";
$dbh = new PDO( $dsn, $user, $pass, $options );
$res = $dbh
    ->query( "SELECT COLLATION_NAME FROM COLLATIONS" )
    ->fetch( PDO::FETCH_COLUMN );

var_dump( $res );

 @endverbatim
 * - check log for "Can't route MYSQL_COM_STMT_PREPARE"
 */

#include <my_config.h>
#include <iostream>
#include "testconnections.h"

int main(int argc, char *argv[])
{
    TestConnections * Test = new TestConnections(argc, argv);
    Test->set_timeout(30);
    char str[1024];

    sprintf(str, "php %s/bug729.php %s %d %s %s", Test->test_dir, Test->maxscale_IP, Test->rwsplit_port, Test->maxscale_user, Test->maxscale_password);

    Test->tprintf("Executing PHP script: %s\n", str);
    Test->add_result(system(str), "PHP script FAILED!\n");

    Test->check_log_err((char *) "Can't route MYSQL_COM_STMT_PREPARE", FALSE);

    Test->copy_all_logs(); return(Test->global_result);
}
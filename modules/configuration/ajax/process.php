<?php
/**
 * This file is used by the Dashboard to get the data for
 * the recruitment bar chart via AJAX
 *
 * PHP version 5
 *
 * @category Main
 * @package  Loris
 * @author   Tara Campbell <tara.campbell@mail.mcgill.ca>
 * @license  Loris License
 * @link     https://github.com/aces/Loris
 */

header("content-type:application/json");

ini_set('default_charset', 'utf-8');

require_once "Database.class.inc";
require_once 'NDB_Client.class.inc';
require_once "Utility.class.inc";
$client = new NDB_Client();
$client->makeCommandLine();
$client->initialize();

$DB =& Database::singleton();
foreach ($values as $key => $value) {
    if (is_numeric($key)) { //update
        $this->DB->update(
            'Config', 
            array('Value' => $value), 
            array('ID' => $key)
        );
    } else { //add new or remove
        $id = split("-", $key);
        if ($id[0] === "add") {
            $this->DB->insert(
                'Config', 
                array('ConfigID' => $id[1], 'Value' => $value)
            );
        } elseif ($id[0] === "remove") {
            $this->DB->delete('Config', array('ID' => $id[1]));
        }
    } 
} 

exit();

?>
<?

$url = 'http://echo.jsontest.com/key/value/one/two';
$obj = json_decode(file_get_contents($url), true);
echo $obj['one'];

?>
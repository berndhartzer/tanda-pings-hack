<?hh // strict

namespace Pings\Handlers;

use Usox\Sharesta\RequestInterface;

class HomeHandler implements \JsonSerializable {

  public function jsonSerialize(): string {

    /* $myfile = fopen('/var/www/newfile.txt', 'a') or die('Unable to open file!'); */
    /* fwrite($myfile, print_r(get_loaded_extensions())); */
    /* fclose($myfile); */

    return 'Root route';
  }
}

class GetSomeIdHandler implements \JsonSerializable {

  private ?string $request_id;

  public function __construct(private RequestInterface $request): void {

  }

  public function doSomething(): void {
    $this->request_id = $this->request->getRouteParameters()->get('id');
  }

  public function jsonSerialize(): string {
    return 'Got id: '.(string) $this->request_id;
  }
}

class SomeNewHandler implements \JsonSerializable {

  private ?dict $some_dict;

  public function handle(): void {
    $this->some_dict = dict['a' => 'one', 'b' => 'two'];
  }

  public function jsonSerialize(): dict {
    return $this->some_dict;
  }

}

class DevicesHandler implements \JsonSerializable {

  private Set<string> $response;

  public function __construct(): void {
    $this->response = new Set();
  }

  public function handle(): void {

    $db = new \PDO('sqlite:/var/db/db.sqlite');

    $db->setAttribute(\PDO::ATTR_ERRMODE, \PDO::ERRMODE_EXCEPTION);

    $query = "SELECT DISTINCT device_id FROM pings";
    $stmt = $db->prepare($query);

    // Execute statement
    $stmt->execute();

    while ($row = $stmt->fetch(\PDO::FETCH_ASSOC)) {
      // var_dump($row);
      $this->response->add($row['device_id']);
    }

    // Close db file connection
    $db = null;

  }

  public function jsonSerialize(): Set<string> {
    return $this->response;
  }

}

class PingHandler implements \JsonSerializable {

  private string $device_id;
  private int $epoch_time;

  // private something $response;??

  public function __construct(private RequestInterface $request): void {

    $this->device_id = $request->getRouteParameters()->get('device_id');
    $this->epoch_time = $request->getRouteParameters()->get('epoch_time');
  }

  public function handle(): void {

    // Create (connect to) SQLite database in file
    $db = new \PDO('sqlite:/var/db/db.sqlite');

    // Set errormode to exceptions
    $db->setAttribute(\PDO::ATTR_ERRMODE, \PDO::ERRMODE_EXCEPTION);

    // Prepare INSERT statement to SQLite3 file db
    $query = "INSERT INTO pings (device_id, timestamp) VALUES (:device_id, :timestamp)";
    $stmt = $db->prepare($query);

    // TODO: check assoc array for binding variables e.g.g $stmt->execute([':device_id' => $this->device_id, ':timestamp' => $this->epoch_time]);
    // Bind parameters to statement variables
    $stmt->bindParam(':device_id', $this->device_id);
    $stmt->bindParam(':timestamp', $this->epoch_time);

      // Execute statement
    $stmt->execute();

    // Return value? maybe $db->lastInsertId();

    // Close db file connection
    $db = null;

  }

  public function jsonSerialize(): string {
    return 'ok';
  }

}

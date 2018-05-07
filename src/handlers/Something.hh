<?hh // strict

namespace Pings\Handlers;

use Usox\Sharesta\RequestInterface;
use Pings\Db\Db;

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

class DeviceHandler implements \JsonSerializable {

  private string $device_id;
  private Vector<int> $response;
  private int $time_from;
  private int $time_to;

  public function __construct(private RequestInterface $request): void {
    $this->response = new Vector();
    $this->device_id = $request->getRouteParameters()->get('device_id');

    if (isset($request->getRouteParameters()->get('date'))) {

      $this->time_from = strtotime($request->getRouteParameters()->get('date'));
      $this->time_to = strtotime('+1 day', $this->time_from);

    } else if (isset($request->getRouteParameters()->get('from'))) {

      $from = $request->getRouteParameters()->get('from');
      $to = $request->getRouteParameters()->get('to');

      $this->time_from = $this->dateIsTimestamp($from) ? $from : strtotime($from);
      $this->time_to = $this->dateIsTimestamp($to) ? $to : strtotime($to);

    }

  }

  public function handle(): void {

    $query_params = Map {
      ':device_id' => $this->device_id,
      ':time_from' => $this->time_from,
      ':time_to' => $this->time_to
    };

    $db = new Db();

    /* $stmt = $db->query('SELECT timestamp FROM pings WHERE device_id = :device_id AND timestamp BETWEEN :time_from AND :time_to', $query_params); */
    $stmt = $db->query('SELECT timestamp FROM pings WHERE device_id = :device_id AND timestamp >= :time_from AND timestamp < :time_to', $query_params);

    while ($row = $stmt->fetch(\PDO::FETCH_ASSOC)) {
      $this->response->add((int)$row['timestamp']);
    }

  }

  public function jsonSerialize(): Vector<int> {
    return $this->response;
  }

  private function dateIsTimestamp(string $date): Boolean {
    return is_numeric($date);
  }

}

class AllHandler implements \JsonSerializable {

  private int $time_from;
  private int $time_to;
  private Map<string, Vector> $response;

  public function __construct(private RequestInterface $request): void {

    $this->response = new Map();

    if (isset($request->getRouteParameters()->get('date'))) {

      $this->time_from = strtotime($request->getRouteParameters()->get('date'));
      $this->time_to = strtotime('+1 day', $this->time_from);

    } else if (isset($request->getRouteParameters()->get('from'))) {

      $from = $request->getRouteParameters()->get('from');
      $to = $request->getRouteParameters()->get('to');

      $this->time_from = $this->dateIsTimestamp($from) ? $from : strtotime($from);
      $this->time_to = $this->dateIsTimestamp($to) ? $to : strtotime($to);
    }

  }

  public function handle(): void {

    $query_params = Map {
      ':time_from' => $this->time_from,
      ':time_to' => $this->time_to
    };

    $db = new Db();

    $stmt = $db->query('SELECT device_id, timestamp FROM pings WHERE timestamp >= :time_from AND timestamp < :time_to', $query_params);

    while ($row = $stmt->fetch(\PDO::FETCH_ASSOC)) {

      if (!isset($this->response[$row['device_id']])) {
        $this->response[$row['device_id']] = new Vector();
      }

      $this->response[$row['device_id']]->add((int)$row['timestamp']);
    }

  }

  public function jsonSerialize(): Map<string, Vector> {
    return $this->response;
  }

  private function dateIsTimestamp(string $date): Boolean {
    return is_numeric($date);
  }

}

class DevicesHandler implements \JsonSerializable {

  private Set<string> $response;

  public function __construct(): void {
    $this->response = new Set();
  }

  public function handle(): void {

    $db = new Db();
    $stmt = $db->query('SELECT DISTINCT device_id FROM pings');

    while ($row = $stmt->fetch(\PDO::FETCH_ASSOC)) {
      $this->response->add($row['device_id']);
    }

  }

  public function jsonSerialize(): Set<string> {
    return $this->response;
  }

}

class PingHandler implements \JsonSerializable {

  private string $device_id;
  private int $epoch_time;

  // private something $response;?? maybe return inserted id?

  public function __construct(private RequestInterface $request): void {

    $this->device_id = $request->getRouteParameters()->get('device_id');
    $this->epoch_time = $request->getRouteParameters()->get('epoch_time');
  }

  public function handle(): void {

    $query_params = Map {
      ':device_id' => $this->device_id,
      ':timestamp' => $this->epoch_time
    };

    $db = new Db();
    $db->query('INSERT INTO pings (device_id, timestamp) VALUES (:device_id, :timestamp)', $query_params);

  }

  public function jsonSerialize(): string {
    return 'ok';
  }

}

class ClearHandler implements \JsonSerializable {

  public function handle(): void {

    $db = new Db();
    $db->query('DELETE FROM pings');
  }

  public function jsonSerialize(): string {
    return 'ok';
  }
}

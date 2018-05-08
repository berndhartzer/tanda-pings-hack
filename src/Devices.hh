<?hh // strict

namespace Pings\Devices;

use Pings\Db\Db;

class Devices {

  private Db $db;

  public function __construct(): void {
    $this->db = new Db();
  }

  public function getDevices(): Set<string> {

    $devices = new Set();

    $stmt = $this->db->query('SELECT DISTINCT device_id FROM pings');

    while ($row = $stmt->fetch(\PDO::FETCH_ASSOC)) {
      $devices->add($row['device_id']);
    }

    return $devices;
  }

  public function getDevicePingsOnDate($device_id, $date): Vector<int> {

    $time_from = strtotime($date);
    $time_to = strtotime('+1 day', $time_from);

    return $this->getDevicePings($device_id, $time_from, $time_to);
  }

  public function getDevicePingsInRange($device_id, $from, $to): void {

    $time_from = $this->dateIsTimestamp($from) ? $from : strtotime($from);
    $time_to = $this->dateIsTimestamp($to) ? $to : strtotime($to);

    return $this->getDevicePings($device_id, $time_from, $time_to);
  }

  public function getAllPingsOnDate($date): Map<string, Vector> {

    $time_from = strtotime($date);
    $time_to = strtotime('+1 day', $time_from);

    return $this->getAllPings($time_from, $time_to);
  }

  public function getAllPingsInRange($from, $to): Map<string, Vector> {

    $time_from = $this->dateIsTimestamp($from) ? $from : strtotime($from);
    $time_to = $this->dateIsTimestamp($to) ? $to : strtotime($to);

    return $this->getAllPings($time_from, $time_to);
  }

  public function addPing($device_id, $timestamp): Map<string, string> {

    $query_params = Map {
      ':device_id' => $device_id,
      ':timestamp' => $timestamp
    };

    $this->db->query('INSERT INTO pings (device_id, timestamp) VALUES (:device_id, :timestamp)', $query_params);

    return $query_params;
  }

  public function deleteAllPings(): void {
    $this->db->query('DELETE FROM pings');
  } 

  private function getDevicePings($device_id, $time_from, $time_to): Vector<int> {

    $pings = new Vector();

    $query_params = Map {
      ':device_id' => $device_id,
      ':time_from' => $time_from,
      ':time_to' => $time_to
    };

    $stmt = $this->db->query('SELECT timestamp FROM pings WHERE device_id = :device_id AND timestamp >= :time_from AND timestamp < :time_to', $query_params);

    while ($row = $stmt->fetch(\PDO::FETCH_ASSOC)) {
      $pings->add((int)$row['timestamp']);
    }

    return $pings;
  }

  private function getAllPings($time_from, $time_to): Map<string, Vector> {

    $pings = new Map();

    $query_params = Map {
      ':time_from' => $time_from,
      ':time_to' => $time_to
    };

    $stmt = $this->db->query('SELECT device_id, timestamp FROM pings WHERE timestamp >= :time_from AND timestamp < :time_to', $query_params);

    while ($row = $stmt->fetch(\PDO::FETCH_ASSOC)) {

      if (!isset($pings[$row['device_id']])) {
        $pings[$row['device_id']] = new Vector();
      }

      $pings[$row['device_id']]->add((int)$row['timestamp']);
    }

    return $pings;
  }

  private function dateIsTimestamp(string $date): Boolean {
    return is_numeric($date);
  }

}

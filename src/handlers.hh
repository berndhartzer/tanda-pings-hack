<?hh // strict

namespace Pings\Handlers;

use Usox\Sharesta\RequestInterface;
use Pings\Devices\Devices;

class DeviceHandler implements \JsonSerializable {

  private string $device_id;
  private Vector<int> $response;
  private string $from;
  private string $to;

  public function __construct(private RequestInterface $request): void {

    $this->response = new Vector();
    $this->device_id = $request->getRouteParameters()->get('device_id');

    if (isset($request->getRouteParameters()->get('date'))) {
      $this->from = $request->getRouteParameters()->get('date');
      $this->to = null;
    } else if (isset($request->getRouteParameters()->get('from'))) {
      $this->from = $request->getRouteParameters()->get('from');
      $this->to = $request->getRouteParameters()->get('to');
    }
  }

  public function handle(): void {

    $devices = new Devices();

    if ($this->to === null) {
      $device_pings = $devices->getDevicePingsOnDate($this->device_id, $this->from);
    } else {
      $device_pings = $devices->getDevicePingsInRange($this->device_id, $this->from, $this->to);
    }

    $this->response = $device_pings;
  }

  public function jsonSerialize(): Vector<int> {
    return $this->response;
  }

}

class AllHandler implements \JsonSerializable {

  private string $from;
  private string $to;
  private Map<string, Vector> $response;

  public function __construct(private RequestInterface $request): void {

    $this->response = new Map();

    if (isset($request->getRouteParameters()->get('date'))) {
      $this->from = $request->getRouteParameters()->get('date');
      $this->to = null;
    } else if (isset($request->getRouteParameters()->get('from'))) {
      $this->from = $request->getRouteParameters()->get('from');
      $this->to = $request->getRouteParameters()->get('to');
    }
  }

  public function handle(): void {

    $devices = new Devices();

    if ($this->to === null) {
      $all_pings = $devices->getAllPingsOnDate($this->from);
    } else {
      $all_pings = $devices->getAllPingsInRange($this->from, $this->to);
    }

    $this->response = $all_pings;
  }

  public function jsonSerialize(): Map<string, Vector> {
    return $this->response;
  }

}

class DevicesHandler implements \JsonSerializable {

  private Set<string> $response;

  public function __construct(): void {
    $this->response = new Set();
  }

  public function handle(): void {
    $devices = new Devices();
    $this->response = $devices->getDevices();
  }

  public function jsonSerialize(): Set<string> {
    return $this->response;
  }

}

class PingHandler implements \JsonSerializable {

  private string $device_id;
  private int $timestamp;
  private Map<string, string> $response;

  public function __construct(private RequestInterface $request): void {
    $this->device_id = $request->getRouteParameters()->get('device_id');
    $this->timestamp = $request->getRouteParameters()->get('epoch_time');
  }

  public function handle(): void {
    $devices = new Devices();
    $this->response = $devices->addPing($this->device_id, $this->timestamp);
  }

  public function jsonSerialize(): Map<string, string> {
    return $this->response;
  }
}

class ClearHandler implements \JsonSerializable {

  private string $response;

  public function handle(): void {
    $devices = new Devices();
    $devices->deleteAllPings();
    $this->response = 'ok';
  }

  public function jsonSerialize(): string {
    return $this->response;
  }
}

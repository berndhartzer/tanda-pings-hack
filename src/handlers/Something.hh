<?hh // strict

namespace Pings\Handlers;

use Usox\Sharesta\RequestInterface;

class HomeHandler implements \JsonSerializable {

  public function jsonSerialize(): string {
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

  public function handle(): void {
  }

  public function jsonSerialize(): Vector {
    return new Vector(null);
  }

}

class PingHandler implements \JsonSerializable {

  public function __construct(private RequestInterface $request): void {

  }

  public function handle(): void {
    $device_id = $this->request->getRouteParameters()->get('device_id');
    $epoch_time = $this->request->getRouteParameters()->get('epoch_time');

    $myfile = fopen('/var/www/newfile.txt', 'a') or die('Unable to open file!');
    $txt = $device_id . ' - ' . $epoch_time . '\n';
    fwrite($myfile, $txt);
    fclose($myfile);


  }

  public function jsonSerialize(): string {
    return 'ok';
  }

}

<?hh // strict

namespace Something\One;

use Usox\Sharesta\RequestInterface;

class HomeRoute implements \JsonSerializable {

  public function jsonSerialize(): string {
    return 'Root route';
  }
}

class GetSomeIdRoute implements \JsonSerializable {

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

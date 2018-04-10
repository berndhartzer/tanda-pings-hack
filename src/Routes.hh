<?hh // strict

namespace Pings\Routes;

use Usox\Sharesta\RequestInterface;
use Usox\Sharesta\RouterInterface;
use Usox\Sharesta\RoutesInterface;

class Routes implements RoutesInterface {

  public function registerRoutes(RouterInterface $router): void {

    $router->get('/', (RequestInterface $request): \JsonSerializable ==> {
      return new \Something\One\HomeRoute();
    });

    $router->get('/get_some/:id', (RequestInterface $request): \JsonSerializable ==> {
      $route = new \Something\One\GetSomeIdRoute($request);
      $route->doSomething();
      return $route;
    });

  }

}

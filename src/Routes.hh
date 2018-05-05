<?hh // strict

namespace Pings\Routes;

use Usox\Sharesta\RequestInterface;
use Usox\Sharesta\RouterInterface;
use Usox\Sharesta\RoutesInterface;

class Routes implements RoutesInterface {

  public function registerRoutes(RouterInterface $router): void {

    $router->get('/', (RequestInterface $request): \JsonSerializable ==> {
      return new \Pings\Handlers\HomeHandler();
    });

    $router->get('/get_some/:id', (RequestInterface $request): \JsonSerializable ==> {
      $route = new \Pings\Handlers\GetSomeIdHandler($request);
      $route->doSomething();
      return $route;
    });

    $router->get('/something', (RequestInterface $request): \JsonSerializable ==> {
      $route = new \Pings\Handlers\SomeNewHandler();
      $route->handle();
      return $route;
    });

    $router->get('/:device_id/:date', (RequestInterface $request): \JsonSerializable ==> {
      $response = new \Pings\Handlers\DeviceHandler($request);
      $response->handle();
      return $response;
    });

    $router->get('/:device_id/:from/:to', (RequestInterface $request): \JsonSerializable ==> {
      $response = new \Pings\Handlers\DeviceHandler($request);
      $response->handle();
      return $response;
    });

    $router->get('/all/:date', (RequestInterface $request): \JsonSerializable ==> {
      return new \Pings\Handlers\HomeHandler();
    });

    $router->get('/all/:from/:to', (RequestInterface $request): \JsonSerializable ==> {
      return new \Pings\Handlers\HomeHandler();
    });

    $router->get('/devices', (RequestInterface $request): \JsonSerializable ==> {
      $response = new \Pings\Handlers\DevicesHandler();
      $response->handle();
      return $response;
    });

    $router->post('/:device_id/:epoch_time', (RequestInterface $request): \JsonSerializable ==> {
      $response = new \Pings\Handlers\PingHandler($request);
      $response->handle();
      return $response;
    });

    $router->post('/clear_data', (RequestInterface $request): \JsonSerializable ==> {
      $response = new \Pings\Handlers\ClearHandler();
      $response->handle();
      return $response;
    });

  }

}

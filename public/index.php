<?hh // partial

require_once(__DIR__.'/../../vendor/hh_autoload.php');

use Usox\Sharesta\ApiFactory;

function app() {
  $factory = new ApiFactory();

  $router = $factory->createRouter(
    new ImmMap($_SERVER),
    new ImmMap($_GET)
  );

  $routes = new \Pings\Routes\Routes();

  $routes->registerRoutes($router);

  $router->route('index.php');
}

app();

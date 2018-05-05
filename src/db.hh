<?hh // strict

namespace Pings\Db;

class Db {

  private \PDO $db;

  public function __construct(): void {
    $this->db = new \PDO('sqlite:/var/db/db.sqlite');
    $this->db->setAttribute(\PDO::ATTR_ERRMODE, \PDO::ERRMODE_EXCEPTION);
  }

  public function query(string $query, Map $params = null): \PDOStatement {

    $stmt = $this->db->prepare($query);

    if ($params !== null) {

      foreach ($params as $key => $value) {
        $stmt->bindValue($key, $value);
      }
    }

    $stmt->execute();

    $this->closeConnection();

    return $stmt;
  }

  private function closeConnection(): void {
    $this->db = null;
  }

}

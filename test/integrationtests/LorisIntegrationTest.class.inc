<?php
/**
 * This contains an abstract class for Loris tests to extend.
 * It sets up the database handler, creates a user, creates a
 * webDriver instance, and logs in so that tests can focus on
 * the module being tested and not the overhead of logging in
 * to Loris.
 *
 * PHP Version 5
 *
 * @category Test
 * @package  Test
 * @author   Dave MacFarlane <david.macfarlane2@mcgill.ca>
 * @license  http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 * @link     https://www.github.com/aces/Loris/
 */

/**
 * Implementation of LorisIntegrationTest helper class.
 *
 * @category Test
 * @package  Test
 * @author   Dave MacFarlane <david.macfarlane2@mcgill.ca>
 * @license  http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 * @link     https://www.github.com/aces/Loris/
 */
abstract class LorisIntegrationTest extends PHPUnit_Framework_TestCase
{
    /**
     * A reference to the Selenium WebDriver object for the test
     * to use to script a web browser.
     */
    protected $webDriver;
    protected $config;
    protected $DB;
    protected $url;

    /**
     * Does basic setting up of Loris variables for this test, such as
     * instantiting the config and database objects, creating a user
     * to user for the tests, and logging in.
     *
     * @return none
     */
    public function setUp()
    {
        // Set up database wrapper and config
        $this->config = NDB_Config::singleton(__DIR__ . "/../../project/config.xml");
        $database     = $this->config->getSetting('database');

        $this->DB = Database::singleton(
            $database['database'],
            $database['username'],
            $database['password'],
            $database['host'],
            1
        );
        $this->url = $this->config->getSetting("url") . "/main.php";

        $this->DB->insert(
            "users",
            array(
             'ID'               => 999990,
             'UserID'           => 'UnitTester',
             'Real_name'        => 'Unit Tester',
             'First_name'       => 'Unit',
             'Last_name'        => 'Tester',
             'Email'            => 'tester@example.com',
             'CenterID'         => 1,
             'Privilege'        => 0,
             'PSCPI'            => 'N',
             'Active'           => 'Y',
             'Examiner'         => 'N',
             'Password_md5'     => 'a601e42ba82bb37a68ca3c8b7752f2e222',
             'Password_hash'    => null,
             'Password_expiry'  => '2099-12-31',
             'Pending_approval' => 'N',
            )
        );

        $this->DB->run(
            "INSERT INTO user_perm_rel SELECT 999990, PermID FROM permissions"
        );

        // Set up WebDriver implementation and login
        $capabilities = array(\WebDriverCapabilityType::BROWSER_NAME => 'firefox');

        $this->webDriver = RemoteWebDriver::create(
            'http://localhost:4444/wd/hub',
            $capabilities
        );

        $this->login("UnitTester", "4test4");

    }

    /**
     * Helper function to login to the loris instance which is being pointed to by
     * this test.
     *
     * @param string $username The username to log in as
     * @param string $password The (plain text) password to login as.
     *
     * @return none, side-effect logs in active webDriver
     */
    protected function login($username, $password)
    {
        $this->webDriver->get($this->url);

        $usernameEl = $this->webDriver->findElement(WebDriverBy::Name("username"));
        $passwordEl = $this->webDriver->findElement(WebDriverBy::Name("password"));

        $usernameEl->sendKeys($username);
        $passwordEl->sendKeys($password);

        $login = $this->webDriver->findElement(WebDriverBy::Name("login"));
        $login->click();
    }

    /**
     * Cleans up this test by deleting the temporary user that was created and all
     * its permissions. user_login_history also must be purged as it contains a
     * foreign key to users
     *
     * @return none
     */
    public function tearDown()
    {

        // Delete the temporary user.
        $this->DB->delete("user_login_history", array('userID' => 'UnitTester'));
        $this->DB->delete("user_perm_rel", array("UserID" => '999990'));
        $this->DB->delete("users", array("UserID" => 'UnitTester'));
        // Close the browser and end the session
        if ($this->webDriver) {
            $this->webDriver->quit();
        }
    }
}
?>
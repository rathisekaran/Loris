<?php

/**
 * Candidate_Dashboard class file
 *
 * PHP version 5
 * 
 * @category Behavioural
 * @package  Main 
 * @author   Rathi Gnanasekaran <sekaranrathi@@gmail.com> 
 * @license  http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 * @link     https://github.com/aces/Loris-Trunk
*/

require_once 'NDB_Form.class.inc';

/**
 * Candidate_Dashboard Class
 *
 * This class is to display candidate information
 * 
 * @category Behavioural
 * @package  Main 
 * @author   Rathi Gnanasekaran <sekaranrathi@gmail.com> 
 * @license  http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 * @link     https://github.com/aces/Loris-Trunk
*/
class NDB_Form_candidate_dashboard extends NDB_Form
{

    /**
     * _has_access returns true
     * if the user has the specific permission
     *
     * @return boolean
     */
    function _hasAccess()
    {
      // create user object
        $user =& User::singleton();
        if (Utility::isErrorX($user)) {
            return PEAR::raiseError("User Error: ".$user->getMessage());
        }

        // check that the $candID is passed b/c the profile needs to be opened
        if ($this->identifier != $_REQUEST['candID']) {
            return PEAR::raiseError("NDB_Form_candidate_dashboard::_access:
                       Candidate Profile Error (".$_REQUEST['candID']."): ");
        }

        $candidate =& Candidate::singleton($this->identifier);
        if (Utility::isErrorX($candidate)) {
            return PEAR::raiseError("Candidate Error
                           ($this->identifier): ".$candidate->getMessage());
        }

        // check user permissions
        return ($user->hasPermission('candidate_parameter_edit')
                && $user->getData('CenterID') == $candidate->getData('CenterID'));
    }

    /**
     * Constructor
     *
     * @return null
     */

    function candidate_dashboard()
    {

    }

}
?>

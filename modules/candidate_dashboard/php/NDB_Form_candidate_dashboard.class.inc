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
      * Returns default values for all candidate parameters
      *
      * @return default array
      */
    function _getDefaults()
    {
        $DB     =& Database::singleton();
        $config =& NDB_Config::singleton();
        //Get the default values
        $value_results = $DB->pselect("SELECT ParameterTypeID, Value FROM 
                         parameter_candidate WHERE CandID=:CaID", 
                         array('CaID' => $this->identifier));
        $candidate_dob = $DB->pselectOne("SELECT DoB FROM candidate 
                                         WHERE CandID=:CandidateID", 
                                         array('CandidateID' => $this->identifier));

        if ($config->getSetting('useProband') === 'true') {
            $proband_info = $DB->pselectRow("SELECT ProbandDoB,ProbandGender FROM
                    candidate WHERE CandID=:CandidateID",
                    array('CandidateID' => $this->identifier));
            if ($proband_info['ProbandDoB']=== '0000-00-00') {
                $proband_info['ProbandDoB'] = '';
            }
            if (!empty($proband_info['ProbandDoB']) & !empty($candidate_dob) ) {
                $age = Utility::calculateAge($proband_info['ProbandDoB'],
                                             $candidate_dob);
                $defaults['Age_Difference'] = $age['year']*12 + $age['mon'] + 
                        round($age['day']/30, 2);
            } else {
                $defaults['Age_Difference'] = 'Could not calculate age';
            }
            $this->tpl_data['proband_info'] = $proband_info;
            foreach ($proband_info as $key=>$val) {
                $defaults[$key] = $val;
                if ($key == 'ProbandDoB') {
                    $defaults[$key."2"] = $proband_info[$key];
                }
            }
        }

        $familyid = $DB->pselectOne("SELECT FamilyID from family WHERE 
                                    CandID=:CandidateID",
                                    array('CandidateID'=>$this->identifier));
        if (Utility::isErrorX($familyid)) {
            return PEAR::raiseError("Error: ".$familyid->getMessage());
        } else {
            $this->tpl_data['familyID'] = $familyid;
            $relations                  = $DB->pselect("SELECT CandID, Relationship_type 
                                          FROM family WHERE FamilyID=:fam_id AND 
                                          CandID <> '$this->identifier'",
                                          array('fam_id'=> $familyid));

        }
        $this->tpl_data['sibling_list'] = array();
        foreach ($relations as $relation) {
            $relation['Relationship_type']    = $this->relationType[$relation['Relationship_type']];
            $this->tpl_data['sibling_list'][] = $relation;
        }
       
        $emptor_info = $DB->pselectRow("SELECT flagged_caveatemptor,flagged_reason,flagged_other
                                        FROM candidate WHERE CandID=:CandidateID",
                                        array('CandidateID' => $this->identifier));
        foreach ($emptor_info as $key=>$val) {
            if ($key == 'flagged_reason') {
                $reason = $DB->pselectOne("SELECT Description AS Reason FROM candidate c 
                                           JOIN caveat_options o ON (o.ID = c.flagged_reason) 
                                           WHERE CandID=:CandidateID",
                                           array('CandidateID' => $this->identifier));
                if (!empty($reason)) {
                   $cand_info['flagged_reason'] = $reason; 
                }
            } else {
                $cand_info[$key]= $val;
            }
         
         }
 
        $DB->select("SELECT ParameterTypeID, Value FROM parameter_candidate 
                     WHERE CandID='".$this->identifier."'", $value_results);
        foreach ($value_results AS $vresult) {
            if (!empty($vresult['Value'])) {
                $cand_info["PTID_".$vresult['ParameterTypeID']] = $vresult['Value'];
            }
        }
        $this->tpl_data['params_list'] = $cand_info;
        foreach ($cand_info as $key=>$val) {
            if ($key == 'flagged_reason') {
                $defaults[$key] = $emptor_info[$key];
            } else {
                $defaults[$key] = $val;
            }
        }
 
       // Getting participant status default values
        $ps_info = $DB->pselectRow("SELECT study_consent,study_consent_date,study_consent_withdrawal,
                                    reason_specify,participant_status,participant_suboptions
                                    FROM participant_status
                                    WHERE CandID = :cid", array('cid'=>$this->identifier));
        if (empty ($ps_info['participant_status'])) {
            //set candidate to active by default
          $this->tpl_data['pstat'] = $DB->pselectOne("SELECT ID FROM participant_status_options WHERE
                                                        Description=:desc", array('desc'=>'Active'));

        } else {
           $this->tpl_data['pstat']     = $ps_info['participant_status'];
           $this->tpl_data['pstat_sub'] = $ps_info['participant_suboptions'];
        }
        foreach ($ps_info as $key => &$val) {

            if ($val === '0000-00-00') {
                $val = '';
            }
            if (!empty($ps_info[$key])) {
                $consent_info[$key] = $val;
            }
        }
 
    foreach ($consent_info as $key=>$val) {
        $defaults[$key] = $val;
        if(strpos($key,"_date") !== FALSE || strpos($key,"_withdrawal") !== FALSE) {
            $defaults[$key."2"] = $consent_info[$key];
        }
    }

        $consent                           = $config->getSetting('ConsentModule');
        $this->tpl_data['display_consent'] = false;
        if ($consent['useConsent'] === 'true') {
            $this->tpl_data['display_consent'] = true;
        }
        $this->tpl_data['display_proband'] = false;
        if ($config->getSetting('useProband') === 'true') {
            $this->tpl_data['display_proband'] = true;
        }
        $this->tpl_data['display_family'] = false;
        if ($config->getSetting('useFamilyID') === 'true') {
            $this->tpl_data['display_family'] = true;
        }
        $this->tpl_data['consent_info'] = $consent_info;
        $result                         = $DB->pselect("SELECT participant_status, entry_staff, 
                                          data_entry_date FROM participant_status_history
                                          WHERE CandID=:cid", array('cid'=>$this->identifier));
        for ($i=0; $i < sizeof($result); $i++) {
            $row                                       = $result[$i];
            $desc                                      = $DB->pselectOne("SELECT Description FROM participant_status_options 
                                                         WHERE ID=:pid", array('pid'=>$row['participant_status']));
            $this->participant_history["status".$i]    = $desc;
            $this->participant_history["staff".$i]     = $row['entry_staff'];
            $this->participant_history["timestamp".$i] = $row['data_entry_date'];
        }
        return $defaults;
    }

    /**
     * Constructor
     *
     * @return null
     */

    function candidate_dashboard()
    {
        $DB =& Database::singleton();
        $pscid = $DB->pselectone(
                'SELECT PSCID FROM candidate where CandID = :candid',
                array('candid'=>$this->identifier)
                );
        $this->addScoreColumn('pscid', 'PSCID');
        $this->tpl_data['candID'] = $this->identifier;

        $this->tpl_data['pscid'] = $pscid;

        //Proband Info section
        $config             =& NDB_Config::singleton();
        $probandDateOptions = array(
            'language' => 'en',
            'format'   => 'YMd',
            //allows for proband siblings up to 20 years older than candidate
            'minYear'  => $config->getSetting('startYear') - 20,
            'maxYear'  => $config->getSetting('endYear'),
            'addEmptyOption' => true,
            'emptyOptionValue' => null
        );
        $this->addBasicDate('ProbandDoB', 'DoB Proband:', $probandDateOptions);
        $this->addSelect('ProbandGender', 'Proband gender:',
               array('' => '','Male' => 'Male', 'Female' => 'Female') );
        $this->addScoreColumn('Age_Difference','Age Difference between candidate<BR> and proband (in months):');

        // Candidate info section
        $caveat_options = array(''=>null);
        $options = $DB->pselect("SELECT ID, Description FROM caveat_options");
        foreach($options as $row) {
            $caveat_options[$row['ID']] = $row['Description'];
        }
        $this->addSelect('flagged_caveatemptor','Caveat Emptor flag for candidate', 
                                 array(''=>'','true'=>'True','false'=>'False'));
        $this->addSelect('flagged_reason',$this->indent.'Reason for Caveat Emptor flag',
                                $caveat_options);
        $this->addTextAreaGroup('flagged_other',$this->indent.'If Other, please specify',
                                          array(null=>'','not_answered'=>'Not Answered'));
        //Extract the fields from the parameter_type table
        $DB->select("SELECT pt.ParameterTypeID, pt.Name, pt.Type, Description FROM parameter_type as pt, 
                     parameter_type_category as ptc, parameter_type_category_rel as ptcr 
                     WHERE pt.ParameterTypeID=ptcr.ParameterTypeID AND ptcr.ParameterTypeCategoryID=ptc.ParameterTypeCategoryID 
                     AND ptc.Name='Candidate Parameters' ORDER BY pt.ParameterTypeID, pt.name ASC", $field_results);
        //If there are no defined fields pritn a message to that effect
        if (empty($field_results)) {
            $this->addScoreColumn("static", "No candidate parameters have been defined for this study");
            return;
        }
        
        //Loop through the fields and add them appropriately
        foreach ($field_results AS $fresult) {
            switch(substr($fresult['Type'],0,3)) {
                case "enu":
                    $options = substr($fresult['Type'],4);
                    $options = trim($options,"()");
                    $options = Utility::explodeEncapsulated(",", "'", $options);
                    $elOptions = array('' => '');
                    foreach ($options as $option) {
                        $elOptions[$option] = $option;
                    }
                    $this->addSelect("PTID_".$fresult['ParameterTypeID'], $fresult['Description'], $elOptions);
                break;
                
                case "dat":
                    $this->addBasicDate("PTID_".$fresult['ParameterTypeID'], $fresult['Description'], $dateOptions);
                break;
                
                case "int":
                case "var":
                case "tex":
            default:
                    $this->addBasicTextArea("PTID_".$fresult['ParameterTypeID'], $fresult['Description']);
                
            }
            $this->tpl_data['elements_list'][]="PTID_".$fresult['ParameterTypeID'];
        }

        //Family Info section
        $this->addBasicText('FamilyMemberID','Family Member ID:<BR/> (Enter DCCID)' );
        $this->addSelect('relation_type','Relation Type', $this->relationType);

        $this->form->addFormRule(array(&$this,'_validate'));


    }

}
?>

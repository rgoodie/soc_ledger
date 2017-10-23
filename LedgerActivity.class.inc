<?php

/**
 * Created by PhpStorm.
 * User: carbon
 * Date: 4/19/17
 * Time: 3:34 PM
 */
class LedgerActivity {
  
  
  static function getSpecificActivityAndRate($selected) {
    $all_act = LedgerActivity::getAllCurrentActivityOptions();
    list($name, $rate) = explode(',', $all_act[$selected]);
    return array(
      'rate' => $rate,
      'name' => $name,
    );
  }
  
  static function getAllCurrentActivityOptions() {
    return explode(PHP_EOL, trim(variable_get('soc_ledger_activities')));
  }
  
  static function getSingleWithdrawalCategory($index) {
    $categories = self::getAllWithdrawalCategories();
    return trim($categories[$index]);
  }
  
  static function getAllWithdrawalCategories() {
    return explode(PHP_EOL, trim(variable_get('soc_ledger_withdrawal_categories')));
  }
  
  
  static public function recordIndividualActivity($uid, $name, $rate, $quantity, $assertion) {
    
    // Create record. Combine the name as name x quantity. Rate is multiplied
    // by quantity.
    $record = array(
      'uid' => $uid,
      'name' => sprintf("%s x %d", $name, $quantity),
      'rate' => $rate * $quantity,
      'datetime' => time(),
      'assertion' => $assertion,
    );
    
    watchdog(__FUNCTION__, print_r($assertion));
    
    // Write record
    $result = drupal_write_record('soc_ledger_activities', $record);
    
    // log everything
    watchdog(SOC_LEDGER_NAME, "Recording :uid, :name, :rate, :assertion", array(
      ':uid' => $uid,
      ':name' => $record['name'],
      ':rate' => $record['rate'],
      ':assertion' => $record['assertion'],
    ));
    
    // Return a result
    return $result;
    
  }
  
  
  static public function deleteIndividualActivity($id) {
    watchdog(SOC_LEDGER_NAME, 'About to delete entry :id', array(':id' => $id));
    return db_delete('soc_ledger_activities', 'a')
      ->condition('activity_id', $id)
      ->execute();
  }
  
  
  static function getActiveUsersInLedger() {
    // select distinct(uid) from soc_ledger_activities;
    $results = db_select('soc_ledger_activities','l')
      ->fields('l', array('uid'))
      ->distinct()
      ->execute()
      ->fetchAll();
    
    
    foreach ($results  as $result) {
      $rows[] = $result->uid;
    }
    
    return $rows;
    
  }
  
  
  static public function getSumfromUID($uid) {
    
    // check numberic
    if (is_numeric($uid)) {
      $query = db_query("select sum(rate) from {soc_ledger_activities} where uid = :uid", array(':uid' => $uid))->fetchCol(0);
      $final = array_pop($query);
      return $final;
    }
    else {
      return FALSE;
    }
  }
  
  
  static public function getActivityForIndividual($uid = 0, $limit = 10) {
    
    
    // Grab userid/names into $user_names array
    $user_names = array();
    $user_q = db_select('users', 'u')
      ->fields('u', array('uid', 'name'))
      ->execute();
    while ($user_r = $user_q->fetch()) {
      $user_names[$user_r->uid] = $user_r->name;
    }
    
    
    // define basic query
    $q = db_select('soc_ledger_activities', 'a')
      ->fields('a');
    
    // if single user
    if ($uid > 0) {
      // get user name
      $u = user_load($uid);
      $q->condition('uid', $uid);
    }
    
    
    // if there is a limit
    if ($limit > 0) {
      $q->range(0, $limit);
    }
    
    
    // do final execute
    $query_execute = $q->orderBy('datetime', 'DESC')->execute();
    
    $to_return = array();
    while ($row = $query_execute->fetchAssoc()) {
      
      $user_link = l($user_names[$row['uid']], 'soc/ledger/report/' . $row['uid']);
      
      $to_return[] = array(
        
        'Transaction ID' => $row['activity_id'],
        'Person' => $user_link,
        'Activity Name' => $row['name'],
        'Rate' => number_format($row['rate'], 2),
        'Date/Time' => date('D Y-m-d h:i:s a', $row['datetime']),
        'Assertion' => $row['assertion'],
        'Delete' => (user_access('administer ledger')) ? l('Detete', "soc/ledger/delete/" . $row['activity_id']) : '',
      );
    }
    
    // Table Array
    $output = array(
      '#theme' => 'table',
      '#header' => @array_keys($to_return[0]),
      '#rows' => $to_return,
      '#attributes' => array(
        'class' => array(
          'table-class',
          'table-responsive',
        ),
      ),
      '#empty' => t('Your table is empty'),
    );
    
    // Add sum for single user.
    if ($uid > 0) {
      $sum = db_query("select sum(rate) from {soc_ledger_activities} where uid=:uid", array(':uid' => $uid))->fetchField(0);
      $output['#rows'][] = array(
        "",
        t('<b>Current Total:</b>'),
        number_format($sum, 2),
        '',
        '',
        '',
        '',
      );
    }
    
    
    // build outer render array (field set). Only used if limits have been called.
    if ($limit > 0) {
      $fs_output = array(
        '#type' => 'fieldset',
        '#attributes' => array('class' => array('soc_ledger_max_height_20')),
        'content' => $output,
      );
      return $fs_output;
    } // Otherwise, just return $output
    else {
      return $output;
    }
    
    return 'No data';
  }
}


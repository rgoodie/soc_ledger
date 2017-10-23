<?php
/**
 * Creates a view that shows balances via NetID.
 * User: rg
 * Date: 10/23/17
 * Time: 11:31 AM
 */


function soc_ledger_totals_by_user() {
  
  
  $uids = LedgerActivity::getActiveUsersInLedger();
  
  foreach ($uids as $uid) {
    
    
    $balance[] = array(
      'Name' => user_load($uid)->name,
      'Balance' => '$' . number_format(LedgerActivity::getSumfromUID($uid), 2),
    );
  }
  
  
  return theme_table(array(
      'header' => array_keys($balance[0]),
      'rows' => $balance,
      'attributes' => array('class'=>'table table-bordered'),
      'caption' => '',
      'colgroups' => array(),
      'sticky' => '',
      'empty' => '',
    
    
    )
  
  );
}
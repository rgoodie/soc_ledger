<?php

function soc_ledger_showLedger() {

    return LedgerActivity::getActivityForIndividual(0,0);


}


function soc_ledger_showLedger_for_user($uid) {

    global $user;

    drupal_set_title(t("Ledger for @user", array(
        '@user'=>$user->name
    )));

    return LedgerActivity::getActivityForIndividual($uid,0);
}
<?php


function soc_ledger_delete_form($form, $form_state, $entry) {

    $item =  db_select('soc_ledger_activities', 'a')
        ->fields('a')
        ->condition('activity_id', $entry)
        ->execute()
        ->fetch();
    ;

    $form[] = array(
        '#markup'=>sprintf("\"%s\" on <em>%s</em>. ", $item->name, date(SOC_LEDGER_DEFAULT_DATE, $item->datetime)),
    );
    $form['entry'] = array(
        '#type' => 'value',
        '#value' => $entry,
    );

    return confirm_form($form, t('Are you sure you want to delete?'), '/soc/ledger/deposit');
}


function soc_ledger_delete_form_submit($form, $form_state) {

    dpm($form_state['values']['entry']['#value']);
}
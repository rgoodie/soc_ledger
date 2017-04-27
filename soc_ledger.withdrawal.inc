<?php

function soc_ledger_withdrawal_form($form, &$form_state)
{

    global $user;

    // First viewing.
    if (!isset($form_state['storage']['confirm'])) {
        $form[] = LedgerActivity::getActivityForIndividual($user->uid, 10);


        $form['activity_fs'] = array(
            '#type' => 'fieldset',
            '#title' => 'Describe Activity'
        );
        $form['activity_fs']['wd_activity_name'] = array(
            '#type' => 'textfield',
            '#title' => t('Activity Name'),
            '#description' => t('Please give a descriptive '),
            '#maxlength' => 255,
            '#element_validate' => array('soc_ledger_validate_withdrawal_nane'),
        );
        $form['activity_fs']['wd_activity_value'] = array(
            '#type' => 'textfield',
            '#title' => t('Activity Value'),
            '#description' => t('Please give a descriptive '),
            '#maxlength' => 255,
            '#element_validate' => array('soc_ledger_validate_withdrawal_value'),
        );


        $form['wd_submit'] = array(
            '#type' => 'submit',
            '#value' => t('record'),
            //'#submit' => array('soc_ledger_withdrawal_form_confirm'),
        );

        return $form;
    } // Triggers confirmation
    else {

        $action = sprintf(
            "Please confirm you are reporting a withdrawing of <em>%d</em> for <em>%s</em>",
            number_format($form_state['storage']['original_form']['wd_activity_value'], 2),
            $form_state['storage']['original_form']['wd_activity_name']
        );
        return confirm_form(
            array(),
            'Please confirm form submission',
            'soc/ledger/withdrawl',
            $action,
            'Yes, this looks right',
            'No, discard my answers and start over');
    }


}

// Submit that triggers confirm.
// Attrib: https://blog.arvixe.com/drupal-providing-confirmation-to-a-form-submit/
function soc_ledger_withdrawal_form_submit($form, &$form_state)
{

    // This will cause the form to be rebuilt, entering the confirm part of the
    // form.
    if (!isset($form_state['storage']['confirm'])) {
        $form_state['storage']['confirm'] = TRUE;
        $form_state['storage']['original_form'] = $form_state['values'];
        $form_state['rebuild'] = TRUE;
    } else {
        global $user;
        $name = $form_state['storage']['original_form']['wd_activity_name'];
        $value = $form_state['storage']['original_form']['wd_activity_value'];
        $msg = sprintf(
            "User %s (%d) withdrew %d for %n",
            $user->name,
            $user->uid,
            $value,
            $name
        );
        watchdog(SOC_LEDGER_NAME, $msg);
        LedgerActivity::recordIndividualActivity($user->uid, $name, $value * -1);

    }


}


function soc_ledger_validate_withdrawal_nane($element, &$form_state, $form)
{
    if (empty($element['#value'])) {
        form_error($element, t($element['#title'] . ': This field is required.'));
    }
}

function soc_ledger_validate_withdrawal_value($element, &$form_state, $form)
{
    if (empty($element['#value'])) {
        form_error($element, t($element['#title'] . ': This field is required.'));
    }
    if (!is_numeric($element['#value'])) {
        form_error($element, t($element['#title'] . ': Must be numeric'));
    }
    // must be positive
    if ($element['#value'] < 1) {
        form_error($element, t($element['#title']. ': Can\'t be negative number ' ));
    }
}




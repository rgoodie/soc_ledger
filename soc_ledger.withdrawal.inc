<?php

function soc_ledger_withdrawal_form($form, &$form_state) {
	
	global $user;
	
	$categories = LedgerActivity::getAllWithdrawalCategories();
	
	
	$form[] = LedgerActivity::getActivityForIndividual($user->uid, 10);
	
	
	$form['activity_fs'] = array(
		'#type' => 'fieldset',
		'#title' => 'Describe Activity',
	);
	$form['activity_fs']['wd_category'] = array(
		'#type' => 'select',
		'#title' => t('Category'),
		'#description' => t('Please select one of these categories that most closely describes your withdrawal.'),
		'#options' => $categories,
	);
	$form['activity_fs']['wd_activity_name'] = array(
		'#type' => 'textfield',
		'#title' => t('Activity Name'),
		'#description' => t('Please give a descriptive '),
		'#maxlength' => 255,
		'#element_validate' => array('_soc_ledger_validate_withdrawal_notnull'),
	);
	$form['activity_fs']['wd_activity_value'] = array(
		'#type' => 'textfield',
		'#title' => t('Activity Value'),
		'#maxlength' => 255,
		'#element_validate' => array('soc_ledger_validate_withdrawal_value'),
	);
	$form['activity_fs']['assertion'] = array(
		'#type' => 'checkbox',
		'#title' => t('Yes, I assert that I have handed in all necessary paper work.'),
		'#element_validate' => array('_soc_ledger_validate_withdrawal_notnull'),
	);
	
	$form['wd_submit'] = array(
		'#type' => 'submit',
		'#value' => t('record'),
	);
	
	return $form;
}

/**
 *
 * Submit handler
 *
 * @param $form
 * @param $form_state
 */
function soc_ledger_withdrawal_form_submit($form, &$form_state) {
	
	global $user;
	$name = $form_state['values']['wd_activity_name'];
	$value = $form_state['values']['wd_activity_value'];
	$category = $form_state['values']['wd_category'];
	$assertion = $form_state['values']['assertion'];
	
	// combine category and name
	$combined_name = sprintf(
		"%s: %s",
		LedgerActivity::getSingleWithdrawalCategory($category),
		$name
	);
	
	$msg = sprintf(
		"User %s (uid: %d) withdrew $%d for %s",
		$user->name,
		$user->uid,
		$value,
		$combined_name
	);
	watchdog(SOC_LEDGER_NAME, $msg);
	LedgerActivity::recordIndividualActivity($user->uid, $combined_name, $value * -1, 1, $assertion);
	dpm('Withdrawal recorded' . $msg);
	
	
}


function _soc_ledger_validate_withdrawal_notnull($element, &$form_state, $form) {
	if (empty($element['#value'])) {
		form_error($element, t("This field is required."));
	}
}

function soc_ledger_validate_withdrawal_value($element, &$form_state, $form) {
	if (empty($element['#value'])) {
		form_error($element, t(' This field is required.'));
	}
	if (!is_numeric($element['#value'])) {
		form_error($element, t('Must be numeric'));
	}
	// must be positive
	if ($element['#value'] < 1) {
		form_error($element, t('Can\'t be negative number '));
	}
}





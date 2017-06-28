<?php

/**
 *
 * FOrm object.
 *
 * Confirmation was removed as requested June '17.
 *
 * @param $form
 * @param $form_state
 *
 * @return array
 */
function soc_ledger_deposit_form($form, &$form_state) {
	
	global $user;
	$uid = $user->uid;
	
	// ledger options
	$ledger_options = LedgerActivity::getAllCurrentActivityOptions();
	
	
	$form[] = LedgerActivity::getActivityForIndividual($uid, 10);
	
	
	$form['activity'] = array(
		'#type' => 'select',
		'#options' => $ledger_options,
		'#title' => t('Select activity'),
	);
	
	$form['quantity'] = array(
		'#title' => t(variable_get('soc_ledger_count_label', 'Quantity?')),
		'#description' => t('How many times did you complete this today?'),
		'#type' => 'textfield',
		'#default' => 1,
		'#element_validate' => array('_soc_ledger_validate_quantity'),
	);
	
	$form['assertion'] = array(
		'#type' => 'checkbox',
		'#title' => t('Yes, I assert that I have handed in all necessary paper work.'),
		'#element_validate' => array('_soc_ledger_validate_assertion'),
	);
	
	// submit
	$form['add'] = array(
		'#type' => 'submit',
		'#value' => 'Record',
		'#submit' => array('soc_ledger_deposit_form_submit'),
	);
	
	return $form;
	
	
}

/**
 * Validator
 * Does not allow non-numeric, negative, or beyond a maximum preset
 *
 * @param $element
 * @param $form_state
 */
function _soc_ledger_validate_quantity($element, &$form_state) {
	
	// numeric
	if (!is_numeric($element['#value'])) {
		form_error($element, t('Item must be numeric'));
	}
	
	// positive
	if ($element['#value'] < 1) {
		form_error($element, t('Number must greater than zero.'));
	}
	
	// Not beyond max number set
	if ($element['#value'] > variable_get('soc_ledger_max_number', 1)) {
		form_error($element, t("Maximum count should only be @number", array(
			'@number' => variable_get('soc_ledger_max_number', 1),
		)));
	}
	
	
}

/**
 *
 * validator for assertion
 *
 * @param $element
 * @param $form_state
 */
function _soc_ledger_validate_assertion($element, &$form_state) {
	if ($element['#value'] == 0) {
		form_error($element, 'You must assert that you\'ve handed in necessary materials');
	}
}


/**
 *
 * Submit handler
 *
 * @param $form
 * @param $form_state
 */
function soc_ledger_deposit_form_submit($form, &$form_state) {
	
	
	global $user;
	$uid = $user->uid;
	
	
	$quantity = $form_state['values']['quantity'];
	$selected = $form_state['values']['activity'];
	watchdog(SOC_LEDGER_NAME, 'Selected @i', array('@i' => $selected));
	$data = LedgerActivity::getSpecificActivityAndRate($selected);
	$assertion = $form_state['values']['assertion'];
	
	// craft log/confirmation message
	$msg = sprintf(
		"User %s (uid: %d) recording %s at the rate of $%d for %d of times.",
		$user->name,
		$uid,
		$data['name'],
		$data['rate'],
		$quantity
	);
	
	dpm($msg);
	watchdog(SOC_LEDGER_NAME, $msg);
	
	// Finally record it
	LedgerActivity::recordIndividualActivity($uid, $data['name'], $data['rate'], $quantity, $assertion);
	
	
}



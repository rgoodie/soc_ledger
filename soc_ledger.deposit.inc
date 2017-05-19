<?php


function soc_ledger_deposit_form($form, &$form_state) {

	global $user;
	$uid = $user->uid;

	// ledger options
	$ledger_options = LedgerActivity::getAllCurrentActivityOptions();

	// First viewing.
	if (!isset($form_state['storage']['confirm'])) {

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

		$form['add'] = array(
			'#type' => 'submit',
			'#value' => 'Record',
			'#submit' => array('soc_ledger_deposit_form_submit'),
		);

		return $form;
	}

	// Triggers confirmation
	else {

		$action = sprintf(
			"Please confirm the  of <em>%s</em> for <em>%d</em> times",
			$ledger_options[$form_state['storage']['original_form']['activity']],
			$form_state['storage']['original_form']['quantity']
		);
		return confirm_form(
			array(),
			'Please confirm form submission',
			'soc/ledger/deposit',
			$action,
			'Yes, this looks right',
			'No, discard my answers and start over');
	}

}


function _soc_ledger_validate_quantity($element, &$form_state) {

	// numeric
	if (!is_numeric($element['#value'])) {
		form_error($element, t('Item must be numeric'));
	}

	// positive
	if (!is_numeric($element['#value'])) {
		form_error($element, t('Number must be positive'));
	}

	// Not beyond max number set
	if ($element['#value'] > variable_get('soc_ledger_max_number', 1)) {
		form_error($element, t("Maximum count should only be @number", array(
			'@number' => variable_get('soc_ledger_max_number', 1),
		)));
	}
}

function soc_ledger_deposit_form_submit($form, &$form_state) {


	global $user;
	$uid = $user->uid;

	// This will cause the form to be rebuilt, entering the confirm part of the
	// form.
	if (!isset($form_state['storage']['confirm'])) {
		$form_state['storage']['confirm'] = TRUE;
		$form_state['storage']['original_form'] = $form_state['values'];
		$form_state['rebuild'] = TRUE;
	}
	else {

		$quantity = $form_state['storage']['original_form']['quantity'];
		$selected = $form_state['storage']['original_form']['activity'];
		watchdog(SOC_LEDGER_NAME, 'Selected @i', array('@i' => $selected));
		$data = LedgerActivity::getSpecificActivityAndRate($selected);
		LedgerActivity::recordIndividualActivity($uid, $data['name'], $data['rate'], $quantity);
	}


}


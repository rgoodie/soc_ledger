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
			"Please confirm the  of <em>%s</em>",
			$ledger_options[$form_state['storage']['original_form']['activity']]
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

		$selected = $form_state['storage']['original_form']['activity'];
		watchdog(SOC_LEDGER_NAME, 'Selected @i', array('@i' => $selected));
		$data = LedgerActivity::getSpecificActivityAndRate($selected);
		LedgerActivity::recordIndividualActivity($uid, $data['name'], $data['rate']);
	}


}


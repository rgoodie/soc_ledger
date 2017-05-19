<?php

define('SOC_LEDGER_EXAMPLE_ACTRATES',
	"First Activity, 40\n" .
	"Second Activity, 48\n" .
	"Third Activity, 15"
);

define('SOC_LEDGER_NAG_DESCRIPTION',
	"Please enter activities and rates in the following fashion. One " .
	"activity and rate per line. Separated by a new line. Separate Activity" .
	"name from current rate by a comma."
);

define('SOC_LEDGER_CATEGORIES',
	"First Category\n" .
	"Second Category\n"
);


function soc_ledger_admin_form($form, &$form_state) {


	$form[] = array(
		'#markup' => t("AA @In <pre>@ex</pre>", array(
			'@In' => SOC_LEDGER_NAG_DESCRIPTION,
			'@ex' => SOC_LEDGER_EXAMPLE_ACTRATES,
		)),

	);

	$form['soc_ledger_activities'] = array(
		'#type' => 'textarea',
		'#title' => t('Activities and Rates'),
		'#default_value' => variable_get('soc_ledger_activities', SOC_LEDGER_EXAMPLE_ACTRATES),
		'#size' => 60,
		'#maxlength' => 255,
		'#required' => TRUE,
		'#element_validate' => array('_soc_ledger_settings_no_dollar_sign')
	);

	$form['soc_ledger_withdrawal_categories'] = array(
		'#type' => 'textarea',
		'#title' => t('Withdrawal Categories'),
		'#default_value' => variable_get('soc_ledger_withdrawal_categories', SOC_LEDGER_CATEGORIES),
		'#size' => 60,
		'#maxlength' => 255,
		'#required' => TRUE,
		'#element_validate' => array('_soc_ledger_settings_no_dollar_sign')
	);


	$form['soc_ledger_count_label'] = array(
		'#type' => 'textfield',
		'#title' => t('Maximum Quantity description phrase'),
		'#description' => t("The label to describe the count field on the deposit UI. "),
		'#maxlength' => 255,
		'#required' => TRUE,
		'#default_value' => variable_get('soc_ledger_count_label', t('How many times did you do this activity?')),

	);

	$form['soc_ledger_max_number'] = array(
		'#type' => 'textfield',
		'#title' => t('Maximum quantity'),
		'#default_value' => variable_get('soc_ledger_max_number', 1),
		'#size' => 2,
		'#maxlength' => 3,
		'#required' => TRUE,

	);


	return system_settings_form($form);

}


/**
 *
 * Don't include a dollar sign
 *
 * @param $element
 * @param $form_state
 */
function _soc_ledger_settings_no_dollar_sign($element, &$form_state) {

	if (strpos($element['#value'], '$')) {
		form_error($element, t("Please don't include a dollar sign ($). This causes issues down the road. "));
	}
}




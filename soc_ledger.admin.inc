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


function soc_ledger_admin_form($form, &$form_state)
{


    $form[] = array(
        '#markup' => t("AA @In <pre>@ex</pre>", array(
            '@In' => SOC_LEDGER_NAG_DESCRIPTION,
            '@ex' => SOC_LEDGER_EXAMPLE_ACTRATES
        ))

    );

    $form['soc_ledger_activities'] = array(
        '#type' => 'textarea',
        '#title' => t('Activities and Rates'),
        '#default_value' => variable_get('soc_ledger_activities', SOC_LEDGER_EXAMPLE_ACTRATES),
        '#size' => 60,
        '#maxlength' => 255,
        '#required' => TRUE,

    );



    return system_settings_form($form);

}




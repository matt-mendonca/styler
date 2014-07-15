<?php
/**
 * Callback function that builds the config form
 * 
 * @todo make it so styler rules can be deleted individually.
 */
function styler_config_form($form, &$form_state) {
  $rules = variable_get('styler_rules');
  $rules = json_decode($rules);

  $style_sheet_options = array();
  $style_sheets_choices = file_scan_directory("sites/all/libraries/styler", '/.*\.css$/');

  foreach ($style_sheets_choices as $key => $style_sheet_file):
    $style_sheet_options[$style_sheet_file->uri] = $style_sheet_file->filename;
  endforeach;

  $form = array(
    '#action' => '#', 
    '#tree' => TRUE,
    'rules_fieldset' => array(
      '#type' => 'fieldset',
      '#title' => t('Styler Rule'),
      '#prefix' => '<div id="styler-rules-fieldset-wrapper">',
      '#suffix' => '</div>',
      'add_rule' => array(
        '#type' => 'submit',
        '#value' => t('Add a rule.'),
        '#submit' => array('styler_add_rule_add_one'),
        '#ajax' => array(
          'callback' => 'syler_add_rule_callback',
          'wrapper' => 'styler-rules-fieldset-wrapper',
        )
      )
    ),

    'submit' => array( 
      '#type' => 'submit',
      '#value' => t('Save')
    )
  );

  if(empty($form_state['number_of_rules'])):
    if(count($rules) > 1): 
      $form_state['number_of_rules'] = count($rules);
    else: 
      $form_state['number_of_rules'] = 1;
    endif;
  endif;

  if($form_state['number_of_rules'] > 1) :
    $form['rules_fieldset']['remove_rule'] = array(
      '#type' => 'submit',
      '#value' => t('Remove last rule.'),
      '#submit' => array('styler_add_rule_remove_one'),
      '#ajax' => array(
        'callback' => 'syler_add_rule_callback',
        'wrapper' => 'styler-rules-fieldset-wrapper',
      ),
    );
  endif;

  for ($i=0; $i < $form_state['number_of_rules']; $i++) :
    if(isset($rules[$i]) && !empty($rules[$i])):
      $rule_name = $rules[$i]->rule_name;
      $parameter_name = $rules[$i]->parameter_name;
      $parameter_value = $rules[$i]->parameter_value;
      $style_sheet = $rules[$i]->style_sheet;
    else:
      $rule_name = '';
      $parameter_name = '';
      $parameter_value = '';
      $style_sheet = '';
    endif;

    $form['rules_fieldset'][$i] = array(
      '#prefix' => '<div>',
      '#suffix' => '</div><br>',

      'rule_name' => array( 
        '#type' => 'textfield',
        '#title' => t('Rule Name'),
        '#description' => t('Enter the query string parameter name.'),
        '#default_value' => $rule_name
      ),

      'parameter_name' => array( 
        '#type' => 'textfield',
        '#title' => t('Parameter Name'),
        '#description' => t('Enter the query string parameter name.'),
        '#default_value' => $parameter_name
      ),

      'parameter_value' => array( 
        '#type' => 'textfield',
        '#title' => t('Parameter Value'),
        '#description' => t('Enter the query string parameter value.'),
        '#default_value' => $parameter_value
      ),

      'style_sheet' => array( 
        '#type' => 'select',
        '#title' => t('Style Sheet'),
        '#description' => t('Style Sheet to apply for render alter. Pulls from sites/all/libraries/styler'),
        '#options' => $style_sheet_options,
        '#default_value' => $style_sheet
      )
    );
  endfor;
  
  return $form;
}

/**
 * Callback for both ajax-enabled buttons.
 *
 * Selects and returns the fieldset with the names in it.
 */
function syler_add_rule_callback($form, $form_state) {
  return $form['rules_fieldset'];
}

/**
 * Submit handler for the "add-one-more" button.
 *
 * Increments the max counter and causes a rebuild.
 */
function styler_add_rule_add_one($form, &$form_state) {
  $form_state['number_of_rules']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for the "remove one" button.
 *
 * Decrements the max counter and causes a form rebuild.
 */
function styler_add_rule_remove_one($form, &$form_state) {
  if ($form_state['number_of_rules'] > 1) :
    $form_state['number_of_rules']--;
  endif;

  $form_state['rebuild'] = TRUE;
}

/**
 * Callback function that saves form data
 */
function styler_config_form_submit($form, &$form_state) {
  $rules = array();

  foreach ($form_state['values']['rules_fieldset'] as $key => $value) :
    if(! is_array($value)):
      continue;
    endif;

    $rules[$key] = $value;
  endforeach;

  $rules = json_encode($rules);

  variable_set('styler_rules', $rules);
  
  drupal_set_message(t('Your configuration has been saved.'));
}
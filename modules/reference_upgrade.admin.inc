<?php
/**
 * @file
 * Admin interface for upgrades.
 */

/**
 * Administrative UI for upgrading references.
 *
 * @return array
 *   Renderable for the admin UI.
 */
function reference_upgrade_admin() {
  $output = '';

  $all_field_info = field_info_fields();

  $reference_field_types = array(
    'taxonomy_term_reference',
    'node_reference',
    'user_reference',
    'entityreference',
  );
  // Make field types alterable.
  backdrop_alter('reference_upgrade_field_types', $reference_field_types);

  $reference_felds = array();
  foreach ($all_field_info as $machine => $field_info) {
    if (in_array($field_info['type'], $reference_field_types)) {
      $reference_felds[$field_info['type']][$machine] = $field_info;
    }
  }

  $header = array(t('Field type'), t('Field machine names'), t('Convert field data'));
  $attributes = array('class' => array('upgrade'));
  $rows = array();
  foreach ($reference_felds as $type => $fields) {
    $link_text = t('Convert all @type fields', array('@type' => $type));
    $link_url = 'admin/content/reference/upgrade/' . $type;
    $options = array('attributes' => array('class' => array('button')));
    $field_names = array();

    foreach ($fields as $machine => $field_info) {
      $field_names[$machine] = $machine;
      // @todo add individual field conversion option.
    }

    $field_cell = theme('item_list', array('items' => $field_names));

    $rows[] = array(
      'type' => array('data' => $type),
      'fields' => array('data' => $field_cell),
      'link' => array('data' => l($link_text, $link_url, $options)),
    );
  }

  $output .= theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => $attributes,
  ));

  return $output;
}

function reference_upgrade_fieldtype($type) {
  if (TRUE) {
    return backdrop_get_form('reference_upgrade_fieldtype_confirm', $type);
  }
  return MENU_NOT_FOUND;
}

/**
 * Confirm form for upgrading a field type.
 *
 * @param string $type
 *   Machine name of the field type.
 *
 * @return array
 *   Confirm form to render.
 */
function reference_upgrade_fieldtype_confirm($form, &$form_state, $type) {
   $form['type'] = array(
    '#type' => 'value',
    '#value' => $type,
  );
  $question = t('Are you sure you wish to convert all fields of type @type?', array('@type' => $type));
  $path = 'admin/content/reference/upgrade';
  $description = NULL;
  $yes = t('Convert fields');
  $no = NULL;
  $name = 'upgrade_fieldtype';

  return confirm_form($form, $question, $path, $description, $yes, $no, $name);
}

/**
 * Submit handler for field type confirm form.
 */
function reference_upgrade_fieldtype_confirm_submit(&$form, &$form_state) {
  if ($form_state['values']['type']) {
    // update all fields of this type.
    reference_upgrade_convert($form_state['values']['type']);
  }
  backdrop_goto('admin/content/reference/upgrade');
}

function reference_upgrade_fieldname($type, $name) {
  if (TRUE) {
    return backdrop_get_form('reference_upgrade_fieldname_confirm', $type, $name);
  }
  return MENU_NOT_FOUND;
}

/**
 * Confirm form for upgrading a field type.
 *
 * @param string $type
 *   Machine name of the field type.
 * @param string $name
 *   Machine name of the field name.
 *
 * @return array
 *   Confirm form to render.
 */
function reference_upgrade_fieldname_confirm($form, &$form_state, $type, $name) {
  $form = array(
    'type' => array(
      '#type' => 'value',
      '#value' => $type,
    ),
    'name' => array(
      '#type' => 'value',
      '#value' => $name,
    ),
  );

  $question = t('Are you sure you wish to convert the @name field?', array('@name' => $name));
  $path = 'admin/content/reference/upgrade';
  $description = NULL;
  $yes = t('Convert field');
  $no = NULL;
  $name = 'upgrade_fieldname';

  return confirm_form($form, $question, $path, $description, $yes, $no, $name);
}

/**
 * Submit handler for field name confirm form.
 */
function reference_upgrade_fieldname_confirm_submit(&$form, &$form_state) {
  if ($form_state['values']['type'] && $form_state['values']['name']) {
    // update all fields of this type.
    reference_upgrade_convert($form_state['values']['type'], $form_state['values']['name']);
  }
  backdrop_goto('admin/content/reference/upgrade');
}

/**
 * Converts one reference field type to another.
 *
 * @param string $type
 *   Machine name of the field type.
 * @param string $field_name
 *   Machine name of the field name.
 *
 * @return string
 *   Message to set letting the person know their field has been converted.
 */
function reference_upgrade_convert($type = NULL, $field_name = NULL) {
  if (!isset($type)) {
    // This should never happen.
    return;
  }

  switch ($type) {
    case 'entityreference':
      _reference_upgrade_convert_entityreference($field_name);
      break;
    case 'taxonomy_term_reference':
      _reference_upgrade_convert_taxonomy_term_reference($field_name);
      break;
    case 'node_reference':
      _reference_upgrade_convert_node_reference($field_name);
      break;
    case 'user_reference':
      _reference_upgrade_convert_user_reference($field_name);
      break;
  }
}

<?php

/**
 * @file
 * Contains the functions for the reference autocomplete widget.
 */

/**
 * Called from reference_field_widget_form() to provide an autocomplete form widget .
 */
function reference_autocomplete_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {

  $entity_bundle_options = reference_autocomplete_entity_bundle_options($instance);
  $entity_bundle_default = isset($items[$delta]['entity_bundle']) ? $items[$delta]['entity_bundle'] : key($entity_bundle_options);

  $element['entity_bundle'] = array(
    '#type' => 'select',
    '#description' => 'Type',
    '#options' => $entity_bundle_options,
    '#default_value' => $entity_bundle_default,
  );
  $element['target_id'] = array(
    '#type' => 'textfield',
    '#description' => 'Entity',
    '#default_value' => isset($items[$delta]['target_id']) ? $items[$delta]['target_id'] : '',
    '#autocomplete_path' => 'reference_autocomplete/' . $entity_bundle_default,
    '#element_validate' => array('reference_autocomplete_validate'),
    '#value_callback' => 'reference_autocomplete_value',
  );
  return $element;
}

/**
 * Helper to get the entities types allowed to be referenced by a field.
 */
function reference_autocomplete_entity_bundle_options($instance) {
  $options = array();
  $entity_info = entity_get_info();
  foreach ($instance['settings']['entity_bundles'] as $entity_bundle => $enabled) {
    if($enabled) {
      list($entity_type, $bundle) = explode('-', $entity_bundle);
      $options[$entity_bundle] = $entity_info[$entity_type]['bundles'][$bundle]['label'];
    }
  }
  return $options;
}

/**
 * Menu callback for providing autocomplete values to the reference fields.
 */
function reference_autocomplete($entity_bundle = NULL, $typed = '') {
  $matches = array();
  list($entity_type, $bundle) = explode('-', $entity_bundle);
  if ($entity_bundle && $entity_info = entity_get_info($entity_type)) {
    $base_table = $entity_info['base table'];
    $id_field = $entity_info['entity keys']['id'];
    $label_field = $entity_info['entity keys']['label'];

    $result = db_select($base_table, 'bt')
      ->fields('bt', array($id_field, $label_field))
      ->condition($label_field, '%' . db_like($typed) . '%', 'LIKE')
      ->condition('type', $bundle, '=')
      ->orderBy($label_field)
      ->range(0, 10)
      ->execute();

    foreach ($result as $entity) {
      $item = reference_autocomplete_item_format($entity_info['label'], $entity->{$id_field}, $entity->{$label_field});
      $matches[$item] = $item;
    }
  }
  backdrop_json_output($matches);

  // TO DO: Why is the echo in backdrop_json_output not disabling the theme?
  module_invoke_all('exit');
  exit();
}

/**
 * Form callback to validate the values inserted into the field after autocompleting.
 */
function reference_autocomplete_validate($element, &$form_state, $form) {
  if (!empty($element['#value'])) {
    if ($id = reference_autocomplete_item_extract_id($element['#value'])) {
      form_set_value($element, $id, $form_state);
    }
    else {
      form_set_value($element, "", $form_state);
      form_error($element, t('The reference field value must contain a numeric id.'));
    }
  }
}

/**
 * Form callback for turning a numeric field value into an autocomplete format.
 */
function reference_autocomplete_value($element, $input = FALSE, $form_state = array()) {
  if(isset($element['#default_target_type']) && isset($element['#default_value'])) {
    $target_type_info = entity_get_info($element['#default_target_type']);
    $target_label_property = $target_type_info['entity keys']['label'];
    $target_entity = entity_load($element['#default_target_type'], $element['#default_value']);
    return reference_autocomplete_item_format($element['#default_target_type'], $element['#default_value'], $target_entity->{$target_label_property});
  }
}

/**
 * Helper function to turn the label and value into an autocomplete item.
 */
function reference_autocomplete_item_format($type, $id, $label) {
  return $label . " (" . $id . ")";
}

/**
 * Helper function to extract the entity id from an autocompleted item.
 */
function reference_autocomplete_item_extract_id($value) {
  if (is_numeric($value)) {
    return $value;
  }
  preg_match('/.*\(([0-9]*)\)$/', $value, $matches);
  return isset($matches[1]) ? $matches[1] : false;
}
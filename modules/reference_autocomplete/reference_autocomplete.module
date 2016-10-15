<?php

/**
 * @file
 * Contains the functions for the reference autocomplete widget.
 */

/**
 * Called from reference_field_widget_form() to provide an autocomplete form widget .
 */
function reference_autocomplete_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $element['target_id'] = array(
    '#type' => 'textfield',
    '#title' => $instance['label'],
    '#description' => $instance['description'],
    '#required' => $instance['required'],
    '#default_value' => isset($items[$delta]['target_id']) ? $items[$delta]['target_id'] : '',
    '#autocomplete_path' => 'reference_autocomplete/' . $instance['field_name'] . '-' . $instance['entity_type'] . '-' . $instance['bundle'],
    '#element_validate' => array('reference_autocomplete_validate'),
    '#value_callback' => 'reference_autocomplete_value',
  );
  return $element;
}

/**
 * Menu callback for providing autocomplete values to the reference fields.
 */
function reference_autocomplete($field_instance = NULL, $typed = '') {

  // Get the info for the field and instance that is being autocompleted.
  list($field_name, $entity_type, $bundle) = explode('-', $field_instance);
  $field_info = field_info_field($field_name);
  $field_instance_info = field_info_instance($entity_type, $field_name, $bundle);

  // Determine our target entity parameters based on the field settings.
  $target_entity_type = $field_info['settings']['entity_type'];
  $target_entity_status = $field_instance_info['settings']['status'];
  $target_entity_bundles = array();
  foreach ($field_instance_info['settings']['bundles'] as $bundle => $active) {
    if ($active) {
      $target_entity_bundles[] = $bundle;
    }
  }

  // Get the database table info for the target entity type.
  $entity_info = entity_get_info($target_entity_type);
  $base_table = $entity_info['base table'];
  $keys = $entity_info['entity keys'];

  // Build the query of our base table using the text that has been typed in .
  $query = db_select($base_table, 'bt')
    ->fields('bt', array($keys['id'], $keys['label']))
    ->condition($keys['label'], '%' . db_like($typed) . '%', 'LIKE')
    ->orderBy($keys['label'])
    ->range(0, 10);

  // Add a condition if we are limiting the bundles returned.
  if ($target_entity_bundles) {
    $query->condition($keys['bundle'], $target_entity_bundles,'IN');
  }

  // Add a condition if we are limiting results to published content.
  if ($target_entity_status) {
    $query->condition($keys['status'], 1,'=');
  }

  // Execute the query, format the results, and output as JSON.
  $result = $query->execute();
  $matches = array();
  foreach ($result as $entity) {
    $item = reference_autocomplete_item_format($entity->{$keys['id']}, $entity->{$keys['label']});
    $matches[$item] = $item;
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
    return reference_autocomplete_item_format($element['#default_value'], $target_entity->{$target_label_property});
  }
}

/**
 * Helper function to turn the label and value into an autocomplete item.
 */
function reference_autocomplete_item_format($id, $label) {
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

<?php

/**
 * @file
 * Contains the functions for the reference link formatter.
 */

/**
 * Called from reference_field_formatter_view() to create the formatter view.
 */
function reference_link_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();
  foreach ($items as $delta => $target) {
    $target_type_info = entity_get_info($target['target_type']);
    $target_label_property = $target_type_info['entity keys']['label'];
    $target_entity = entity_load($target['target_type'], $target['target_id']);
    $element[] = array(
      '#markup' => l($target_entity->$target_label_property, $target_entity->path['source']),
    );
  }
  return $element;
}

<?php
/**
 * @file
 * Integration with the Feeds module.
 */

/**
 * Implements hook_feeds_processor_targets_alter().
 */
function reference_feeds_processor_targets_alter(&$targets, $entity_type, $bundle) {
  foreach (field_info_instances($entity_type, $bundle) as $name => $instance) {
    $info = field_info_field($name);
    if ($info['type'] == 'reference') {
      foreach ($info['columns'] as $sub_field => $schema_info) {
        $name_label = $instance['label'] . ': ' . drupal_ucfirst(str_replace('_', ' ', $sub_field));
        $targets[$name . ':' . $sub_field] = array(
          'name' => $name_label,
          'callback' => 'reference_set_target',
          'real_target' => $info['field_name'],
          'description' => $schema_info['description'],
        );
      }
    }
  }
}

/**
 * Callback for hook_feeds_processor_targets_alter().
 *
 * @param $source
 *   Field mapper source settings.
 * @param $entity
 *   An entity object, for instance a node object.
 * @param $target
 *   A string identifying the target on the node.
 * @param $values
 *   The value to populate the target with.
 */
function reference_set_target(FeedsSource $source, $entity, $target, array $values, array $mapping) {
  $language = $mapping['language'];
  list($field_name, $column) = explode(':', $target . ':target_id');

  // Convert the values into an array if it isn't one already to correspond to
  // Drupal's handling of field value arrays.
  if (!is_array($values)) {
    $values = array($values);
  }


  // Determine entity type and ID for entity_load() below.
  if (get_class($source->importer->processor) == 'FeedsNodeProcessor') {
    $entity_type = 'node';
  }

  // Reload current field values.
  $existing_entities = FALSE;
  if ($entity_id = $entity->nid) {
    $existing_entities = entity_load($entity_type, array($entity_id), NULL, TRUE);
  }

  if (!empty($existing_entities)) {
    $existing_entity = reset($existing_entities);
    // If the field is already set on the given entity, update the existing value
    // array. Otherwise start with a fresh field value array.
    $field = isset($existing_entity->{$field_name}) ? $existing_entity->{$field_name} : array();
  }
  else {
    // If the field is already set on the given entity, update the existing value
    // array. Otherwise start with a fresh field value array.
    $field = isset($entity->{$field_name}) ? $entity->{$field_name} : array();
  }

  $references = array();
  if (isset($field[$language])) {
    foreach ($field[$language] as $delta => $item) {
      if (isset($item['target_id'])) {
        $references[$delta] = $item['target_id'];
      }
    }
  }

  // Loop over the field values array...
  foreach ($values as $value) {
    $query = "SELECT nid FROM {node} WHERE title = :title";
    $nid = db_query($query, array(':title' => $value))->fetchField();

    if (!in_array($nid, $references)) {
      // Add additional mappings.
      $field[$language][] = array('target_id' => $nid);
    }
  }

  $entity->{$field_name} = $field;
}

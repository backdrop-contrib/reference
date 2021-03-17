<?php
/**
 * @file
 * Views integration for Reference.
 */

/**
 * Implements hook_field_views_data().
 */
function reference_field_views_data($field) {
  // Get info for the entity type this field references to add a relationship.
  $target_info = entity_get_info($field['settings']['entity_type']);
  // Get the default values of the views config data for this field.
  $data = field_views_field_default_views_data($field);

  $entity_type = $target_info['label'];
  if ($entity_type == t('Node')) {
    $entity_type = t('Content');
  }
  $parameters = array('@type' => $entity_type, '!field_name' => $field['field_name']);

  // Alter data for both the field_data and field_revision tables.
  foreach ($data as $table_name => $table_data) {
    // Add a relationship TO the target table from the [field_name]_target_id.
    $data[$table_name][$field['field_name'] . '_target_id']['relationship'] = array(
      'handler' => 'views_handler_relationship',
      'base' => $target_info['base table'],
      'base field' => $target_info['entity keys']['id'],
      'label' => t('@type referenced by !field_name', $parameters),
      'group' => t('Reference'),
      'title' => t('Reference to'),
      'help' => t('A bridge to the @type that is referenced in the field <em>!field_name</em>', $parameters),
    );
  }

  return $data;
}

/**
 * Implements hook_field_views_data_views_data_alter().
 *
 * Views integration to provide reverse relationships on reference fields.
 */
function reference_field_views_data_views_data_alter(&$data, $field) {
  foreach ($field['bundles'] as $entity_type => $bundles) {
    if (array_key_exists('entity_type', $field['settings'])) {
      $target_entity_info = entity_get_info($field['settings']['entity_type']);
      if (isset($target_entity_info['base table'])) {
        $entity_info = entity_get_info($entity_type);
        $entity = $entity_info['label'];
        if ($entity == t('Node')) {
          $entity = t('Content');
        }
        $target_entity = $target_entity_info['label'];
        if ($target_entity == t('Node')) {
          $target_entity = t('Content');
        }

        $pseudo_field_name = 'reverse_' . $field['field_name'] . '_' . $entity_type;
        $replacements = array('@entity' => $entity, '@target_entity' => $target_entity, '!field_name' => $field['field_name']);
        $data[$target_entity_info['base table']][$pseudo_field_name]['relationship'] = array(
          'handler' => 'views_handler_relationship_entity_reverse',
          'field_name' => $field['field_name'],
          'field table' => _field_sql_storage_tablename($field),
          'field field' => $field['field_name'] . '_target_id',
          'base' => $entity_info['base table'],
          'base field' => $entity_info['entity keys']['id'],
          'label' => t('@entity referencing @target_entity from !field_name', $replacements),
          'group' => t('Reference'),
          'title' => t('Reference from'),
          'help' => t('A bridge to the @entity that has @target_entity in its field <em>!field_name</em>', $replacements),
          'join_extra' => array(
            0 => array(
              'field' => 'entity_type',
              'value' => $entity_type,
            ),
            1 => array(
              'field' => 'deleted',
              'value' => 0,
              'numeric' => TRUE,
            ),
          ),
        );
      }
    }
  }
}

/**
 * Implements hook_views_plugins().
 */
function reference_views_plugins() {
  $plugins = array(
    'display' => array(
      'reference' => array(
        'title' => t('Reference'),
        'admin' => t('Reference Source'),
        'help' => t('Selects referenceable entities for a reference field'),
        'handler' => 'reference_plugin_display',
        'uses hook menu' => FALSE,
        'use ajax' => FALSE,
        'use pager' => FALSE,
        'accept attachments' => FALSE,
        // Custom property, used with views_get_applicable_views() to retrieve
        // all views with a 'Reference' display.
        'reference display' => TRUE,
      ),
    ),
    'style' => array(
      'reference_style' => array(
        'title' => t('Reference list'),
        'help' => t('Returns results as a PHP array of labels and rendered rows.'),
        'handler' => 'reference_plugin_style',
        'theme' => 'views_view_unformatted',
        'uses row plugin' => TRUE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'reference',
        'even empty' => TRUE,
      ),
    ),
    'row' => array(
      'reference_fields' => array(
        'title' => t('Inline fields'),
        'help' => t('Displays the fields with an optional template.'),
        'handler' => 'reference_plugin_row_fields',
        'theme' => 'views_view_fields',
        'theme path' => backdrop_get_path('module', 'views') . '/theme',
        'theme file' => 'theme.inc',
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'reference',
      ),
    ),
  );
  return $plugins;
}

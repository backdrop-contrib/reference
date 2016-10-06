# Reference
Defines a field type to let entities referencing other entites in Backdrop.

### Current status:

This module is currently under development and should not be used on any production sites.

### Completed:

* define the field in reference_field_info()
* define the field schema in reference_field_schema()
* alter node entity info to provide its label and status properties
* create global and instace settings forms for the field
* define reference_field_is_empty() to check whether the field is filled in or not
* define a reference_autocomplete widget in hook_field_widget_info
* define a widget form in reference_field_widget_form (though not actually autocomplete yet)
* define a simple link display formatter in reference_field_formatter_info
* render a link to the referenced entity in reference_field_formatter_view
* use a select list for the entity type in the widget that respect the type restriction
* field validation to make sure the referenced entity exists
* define automcomplete function to make autocomplete widget actually autocomplete
* function to turn the autocomplete value to a numberic target id on form submit
* switch to using entity type bundle combos that are allowed to be referenced
* add js file for widget
* switch autocomplete path for entity_id when entity_type changes
* clear entity id field if the entity type is changed
* update reference_entity_info_alter to provide info on all core content entities
* change autocomplete function to use new entity/bundle combo
* unbind first autocomplete event when create a new one

### In progress:

* Switch to allowing only a single entity type as the field target.

### Things to do:

* autocomplete callback failing for comment types
* check that entity exists in autocomplete validate
* get autocomplete function to respect status restrictions
* permissions on autocomplete callback to check that user has ability to edit the field
* add access check to the entity info so user can only select content they can access
* update field validation to test that the referenced entity meets restrictions
* behaviour for when an extisting target is no longer an allowed entity bundle
* hide or disable entity type selector if only one entity type can be selected
* hide the numeric id from the user after autocomplete select?
* autocomplete to get entity labels that start with the typed text before just containing
* error or watchdog when trying to reference an entity that hasn't defined its keys properly
* additional field formatter to display rendered entity
* views integration
* mutilingual support for label autocomplete
* and many more...

### To merge into core:

* write tests
* patch core modules to provide their label and status properties in their entity info
* remove then-unneeded reference_entity_info_alter() function

### Possible core changes:
* add bundles for file entities
* replace references provided by other core modules, such as taxonomy tags and other term refs
* upgrade path from taxonomy reference modules
* upgrade path from contrib references to reference
* standardized autocomplete widget
* file key for includes in hook_field_widget_info and hook_field_formatter_info
* file key for includes in hook_field_info

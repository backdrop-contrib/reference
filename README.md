# Reference
Defines a field type to let entities referencing other entites in Backdrop.

### Current status:

This module is currently under development and should not be used on any production sites.

### In progress:

* figure out why changing the field value doesn't make change on save

### Things to do:

* do we really want to use a value_callback to set the default value?
* what happens if you feed it a bad id number
* figure out proper way to set widget title so it isn't repeated with multiple values.
* make sure that a default value can be set in the field instance settings form
* check that entity exists in autocomplete validate
* update field validation to test that the referenced entity meets current restrictions
* access check on autocomplete callback to check that user has ability to edit the field
* add access check to the entity info so user can only select content they can access
* hide the numeric id from the user after autocomplete select
* autocomplete to get entity labels that start with the typed text before just containing
* additional field formatter to display rendered entity
* views integration
* multilingual support for label autocomplete
* and many more...

### Test to write: ###

* create reference field
* select entity type on the settings form
* entity type select disabled after field has data
* autocomplete callback reads field settings properly
* autocomplete respects bundle
* autocomplete respects status
* feeding a bad id number
* changing an existing field value
* display value on entity view
* required field cannot be blank

### To merge into core:

* write tests
* patch core modules to provide their label and status properties in their entity info
* remove then-unneeded reference_entity_info_alter() function

### Possible core changes:
* add bundles for file entities
* add plural for bundle labels
* replace references provided by other core modules, such as taxonomy tags and other term refs
* upgrade path from taxonomy reference modules
* upgrade path from contrib references to reference
* standardized autocomplete widget
* file key for includes in hook_field_widget_info and hook_field_formatter_info
* file key for includes in hook_field_info

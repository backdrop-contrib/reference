# Reference
Defines a field type to let entities referencing other entites in Backdrop.

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

### In progress:

* function to turn the autocomplete value to a numberic target id on form submit

### Things to do:

* get autocomplete function to respect bundle and status restrictions
* permissions on autocomplete callback to check that user has ability to edit the field
* update field validation to test that the referenced entity meets restrictions
* switch from selecting one entity type in the settings, to limiting to one or more types
* hide or disable entity type selector if only one entity type can be selected
* add js file for widget
* switch autocomplete path for entity_id when entity_type changes
* autocomplete to get entity labels that start with the typed text before just containing
* update reference_entity_info_alter to provide info on all core entities
* error or watchdog when trying to reference an entity that hasn't defined its keys properly
* figure out which settings should be set for the field globally, for the instance, or both
* clear entity id field if the entity type is changed
* additional field formatter to display rendered entity
* views integration
* and many more...

### To merge into core:

* write tests
* patch core modules to provide their label and status properties in their entity info
* remove unneeded reference_entity_info_alter() function

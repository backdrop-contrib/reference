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

### Things to do:

* allow more than one entity to be referenced by the field
* switch from selecting one entity type in the settings, to limiting to one or more types
* hide or disable entity type selector if only one entity type can be selected
* update reference_entity_info_alter to provide info on all core entities
* figure out which settings should be set for the field globally, for the instance, or both
* define automcomplete function to make autocomplete widget actually autocomplete
* get autocomplete function to respect bundle and status restrictions
* update field validation to test that the referenced entity meets restrictions
* field formatter to display rendered entity
* and many more...

### To merge into core:

* patch core modules to provide their label and status properties in their entity info
* remove unneeded reference_entity_info_alter() function

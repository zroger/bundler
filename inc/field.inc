<?php

/**
 * Abstract factory class for creating fields and field_instances.  Extending
 * classes must implement the two abstract methods, field_config() and
 * instance_config().
 *
 * Usage:
 *   example_field::create_instance('node', 'page');
 */
abstract class bundler_field_factory {
  /**
   * Return the default field configuration array for the {field_config} table.
   *
   * @return
   *   A field definition array. The field_name and type properties are required.
   *   Other properties, if omitted, will be given the following default values:
   *   - cardinality: 1
   *   - locked: FALSE
   *   - indexes: the field-type indexes, specified by the field type's
   *     hook_field_schema(). The indexes specified in $field are added
   *     to those default indexes. It is possible to override the
   *     definition of a field-type index by providing an index with the
   *     same name, or to remove it by redefining it as an empty array
   *     of columns. Overriding field-type indexes should be done
   *     carefully, for it might seriously affect the site's performance.
   *   - settings: each omitted setting is given the default value defined in
   *     hook_field_info().
   *   - storage:
   *     - type: the storage backend specified in the 'field_storage_default'
   *       system variable.
   *     - settings: each omitted setting is given the default value specified in
   *       hook_field_storage_info().
   *
   * @see field_create_field()
   */
  abstract public function field_config();

  /**
   * Return the default field instance configuration array.
   *
   * @return
   *   A field instance definition array. The field_name, entity_type and
   *   bundle properties will be filled automatically. Other properties, if
   *   omitted, will be given the following default values:
   *   - label: the field name
   *   - description: empty string
   *   - required: FALSE
   *   - default_value_function: empty string
   *   - settings: each omitted setting is given the default value specified in
   *     hook_field_info().
   *   - widget:
   *     - type: the default widget specified in hook_field_info().
   *     - settings: each omitted setting is given the default value specified in
   *       hook_field_widget_info().
   *   - display:
   *     Settings for the 'default' view mode will be added if not present, and
   *     each view mode in the definition will be completed with the following
   *     default values:
   *     - label: 'above'
   *     - type: the default formatter specified in hook_field_info().
   *     - settings: each omitted setting is given the default value specified in
   *       hook_field_formatter_info().
   *     View modes not present in the definition are left empty, and the field
   *     will not be displayed in this mode.
   *
   * @see field_create_instance()
   */
  abstract public function instance_config();

  /**
   * Create the "profile_picture" image field.  This only creates the record in
   * {field_config}, not any instances.  Use create_instance to actually
   * create a field instance on an entity bundle.
   *
   * @return
   *   The $field array with the id property filled in.
   */
  public function create_field() {
    $field_config = $this->field_config();

    // Create the field if it doesn't already exist.
    $field = field_read_field($field_config['field_name'], array('include_inactive' => TRUE));
    if (empty($field)) {
      $field = field_create_field($field_config);
    }

    return $field;
  }

  /**
   * Create a field instance.
   *
   * @return
   *   The $instance array with the id property filled in.
   */
  public function create_instance($entity_type, $bundle, $overrides = array()) {
    $field = $this->create_field();

    // Create the field instance if it doesn't already exist.
    $instance = field_read_instance($entity_type, $field['field_name'], $bundle);
    if (empty($instance)) {
      $instance = array(
        // Required instance properties.
        'field_name' => $field['field_name'],
        'entity_type' => $entity_type,
        'bundle' => $bundle,
      ) + $overrides + $this->instance_config();

      $instance = field_create_instance($instance);
    }

    return $instance;
  }

}


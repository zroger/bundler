<?php
/**
 * $bundle = new bundler_bundle('node', 'article');
 * $bundle->add_field('body');
 * $bundle->set_display('default', 'body', 'hidden', $weight, $settings);
 * $bundle->save();
 */

interface bundler_bundle_interface {
  public function add_field($field_name, $overrides = array());
  public function set_display($display, $field_name, $formatter = 'default', $label = 'hidden', $weight = NULL, $settings = array());
  public function save();
}

class bundler_bundle implements bundler_bundle_interface {
  protected $fields = array();
  protected $entity_type;
  protected $bundle;

  public $module;
  public $optional_modules = array();

  public function __construct($entity_type, $bundle) {
    $this->entity_type = $entity_type;
    $this->bundle = $bundle;
    return $this;
  }

  public function add_field($field_name, $overrides = array()) {
    $field_factory = bundler_field_factory($field_name);
    if (!empty($field_factory)) {
      $instance = $field_factory->create_instance($this->entity_type, $this->bundle, $overrides);
      $this->fields[$field_name] = $instance;
    }
    return $this;
  }

  public function set_display($display, $field_name, $formatter = 'default', $label = 'hidden', $weight = NULL, $settings = array()) {
    if (!empty($this->fields[$field_name])) {
      $this->fields[$field_name]['display'][$display] = array(
        'label' => $label,
        'type' => $formatter,
        'weight' => isset($weight) ? $weight : $this->max_weight(),
        'settings' => $settings,
      );
    }

    return $this;
  }

  protected function max_weight($display) {
    $max = 0;
    foreach ($this->fields as $field_name => $info) {
      if (!empty($field['display'][$display]['weight'])) {
        $max = max($max, $field['display'][$display]['weight']);
      }
    }
    return $max;
  }

  public function save() {
    // Save fields.
    foreach ($this->fields as $field_name => $instance_config) {
      try {
        $instance_config += array(
          // Required instance properties.
          'field_name' => $field_name,
          'entity_type' => $this->entity_type,
          'bundle' => $this->bundle,
        );
        $instance = field_update_instance($instance_config);
      }
      catch (Exception $e) {
        function_exists('drush_log') && drush_log($e->getMessage(), 'error');
      }
    }
  }
}

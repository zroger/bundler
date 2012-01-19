<?php

class bundler_tests__bundle_factory extends bundler_bundle {
  protected $fields = array();
  protected $entity_type;
  protected $bundle;

  public function __construct() {
    parent::__construct('node', 'bundler_test');
    $this->initialize();
    return $this;
  }

  protected function initialize() {
    $this
      ->add_field('body');

    // Default display.
    $this
      ->set_display('default', 'body', 'text_default', 'hidden', 0);

    // Teaser display.
    $this
      ->set_display('teaser', 'body', 'text_summary_or_trimmed', 'hidden', 0, array('trim_length' => 600));
  }
}

/**
 * Field factory for the "body" field.
 */
class bundler_tests__body_field_factory extends bundler_field_factory {
  public function field_config() {
    return array(
      'field_name' => 'body',
      'type' => 'text_with_summary',
      'cardinality' => 1,
    );
  }

  public function instance_config() {
    return array(
      'label' => t('Body'),
      'required' => TRUE,
      'settings' => array(
        'display_summary' => TRUE,
        'text_processing' => 1,
      ),
      'widget' => array(
        'type' => 'text_textarea_with_summary',
        'settings' => array(
          'rows' => 20,
          'summary_rows' => 5,
        ),
      ),
    );
  }
}

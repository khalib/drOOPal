<?php

class Form {
  public $form;

  public function __construct($mixed) {
    $this->load($mixed);
  }

  /**
   * Load the Drupal form into this class object.
   *
   * @param object
   *   A Drupal form object.
   * @return this
   */
  public function load($object) {
    // Copy the raw entity object for any Drupal function that require it.
    $this->form = $object;

    foreach ((array) $object as $key => $value) {
      $key = trim(str_replace('*', '', $key));
      $this->$key = $value;
    }

    return $this;
  }

  /**
   * Set attributes to a form field.
   *
   * @param string
   *   The name of a field (without the 'field_' prefix).
   * @param string
   *   The attribute key name.
   * @param mixed (array or string)
   *   The values to be assigned to the attribute.
   * @return this
   */
  public function set_field_attributes($field, $key, $values) {
    $field_name = 'field_' . $field;

    // See what type of form field is being used.
    $field_type = (isset($this->form[$field_name][LANGUAGE_NONE]['#type']) ? $this->form[$field_name][LANGUAGE_NONE]['#type'] : NULL);
    switch ($field_type) {
      case 'select':
        $attributes = &$this->form[$field_name][LANGUAGE_NONE]['#attributes'];
        break;

      default:
        $attributes = &$this->form[$field_name][LANGUAGE_NONE][0]['value']['#attributes'];
        break;
    }

    if (isset($attributes[$key])) {
      // Add attribute values to existing attribute array.
      foreach ($values as $value) {
        $attributes[$key][] = $value;
      }
    }
    else {
      // Add new attribute and values.
      $attributes[$key] = $values;
    }

    return $this;
  }
}
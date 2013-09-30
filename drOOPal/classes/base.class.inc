<?php

/**
 * The base class that all node/user classes are to extend.
 */
abstract class BaseClass {
  protected $entity_type;
  protected $many_fields = array();

  /**
   * Default constructor that auto-loads a node or a user.
   *
   * @param mixed A user or node object to be loaded, or a uid or nid to load the object.
   * @param string The type of object to be loaded.
   * @return NULL
   */
  public function __construct($mixed = NULL, $type = 'node') {
    $this->entity_type = $type;

    if (!empty($mixed)) {
      if (is_numeric($mixed)) {
        if ($type == 'node') {
          $object = node_load($mixed);
        } elseif ($type == 'user') {
          $object = user_load($mixed);
        }
        $this->load($object);
      } elseif (is_object($mixed)) {
        $this->load($mixed);
      }
    }
  }

  /**
   * Load the Drupal object into this class object.
   *
   * @param object
   *   A Drupal node or user object.
   * @return this
   */
  public function load($object) {
    // Copy the raw entity object for any Drupal function that require it.
    $this->{$this->entity_type} = $object;

    foreach ((array) $object as $key => $value) {
      $key = trim(str_replace('*', '', $key));
      $this->$key = $value;
    }

    return $this;
  }

  /**
   * Get the url alias path.
   */
  public function get_url($absolute = FALSE) {
    $id = ($this->entity_type == 'node') ? $this->nid : $this->uid;
    return url($this->entity_type . '/' . $this->nid);
  }

  /**
   * Get the field level entity of a node.
   *
   * @param string
   *   The name of the field sans the 'field_' prefix.
   * @param string
   *   The entity type (node or user).
   * @return
   */
  public function get_field($field, $type = 'node') {
    $field_name = 'field_' . $field;

    if (property_exists($this, $field_name)) {
      $field = $this->{$field_name};
      $entity = entity_metadata_wrapper($type, $this);
      $property_value = $entity->{$field_name}->value();

      $is_many = in_array($field_name, $this->many_fields);
      if (!$is_many) {
        $property_value = $entity->{$field_name};
      }

      foreach ($this->field_exceptions as $value) {
        if (isset($field[0]) && $field[0]->{$value}) {
          if (!$is_many) {
            $property_value = $field[0];
          }
          else {
            $property_value = $field;
          }
        }
      }

      return $property_value;
    }
  }

  /**
   * Get the field level entity of a node.
   *
   * @param string
   *   The name of the field sans the 'field_' prefix.
   * @return EntityValueWrapper
   *   The data object of the entity field.
   */
  public function get_entity_field($field) {
    $field_name = 'field_' . $field;

    // Check to see what type of entity this is.
    $entity_type = 'node';
    if (property_exists($this, 'user')) {
      $entity_type = 'user';
    }

    $entity_wrapper = entity_metadata_wrapper('user', $this->user);
    return $entity_wrapper->{$field_name};
  }
}
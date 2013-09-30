<?php

class Node extends BaseClass {
  // Node fields.
  public $node;
  public $nid;
  public $uid;
  public $vid;
  public $type;
  public $language;
  public $title;
  public $status;
  public $created;
  public $changed;
  public $comment;
  public $promote;
  public $sticky;
  public $tnid;
  public $translate;

  // Object properties.
  protected $query_limit = 5;
  protected $query_offset = 0;
  protected $query_node_status = 1;
  public $results = array();

  // Field type cases.
  protected $field_exceptions = array(
    'timezone',  // Date/time
    'filemime',  // Image
  );

  public $entity;

  public function __construct($mixed = NULL) {
    parent::__construct($mixed, 'node');

    // Load the entity wrapper for use.
    if ($mixed) {
      $this->entity = entity_metadata_wrapper('node', $this);
    }
  }

  public function __get($name) {
    $field_name = 'field_' . $name;

    if (property_exists($this, $field_name)) {
      $field = $this->{$field_name};
      $entity = entity_metadata_wrapper('node', $this);
      $property_value = $entity->{$field_name}->value();

      $is_many = in_array($field_name, $this->many_fields);
      if (!$is_many) {
        $property_value = $entity->{$field_name}->value();
      }

      foreach ($this->field_exceptions as $value) {
        if (isset($field[0]) && property_exists($field[0], $value)) {
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
    else {
      if (property_exists($this, $name)) {
        return $this->{$name};
      }
    }
  }

  /**
   * Checks to see if the object is loaded.
   *
   * @return boolean
   *   TRUE|FALSE whether the node object is loaded with data.
   */
  public function is_loaded() {
    $loaded = FALSE;

    if ($this->nid) {
      $loaded = TRUE;
    }

    return $loaded;
  }

  /**
   * Get the field level entity of a node.
   *
   * @param string - The name of the field sans the 'field_' prefix.
   * @return
   */
  public function get_field($field) {
    $field_name = 'field_' . $field;

    if (property_exists($this, $field_name)) {
      $field = $this->{$field_name};
      $entity = entity_metadata_wrapper('node', $this);
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
   * Get the user data object of the node.
   *
   * @return object User
   *   A user object.
   */
  public function get_user() {
    if ($this->uid) {
      return new User($this->uid);
    }
  }

  /**
   * Get the relative URL of the node.
   *
   * @return string
   *   The URL of the node.
   */
  public function get_url() {
    return url('node/' . $this->nid);
  }

  /**
   * Simple node node retrieval.
   *
   * @return array
   *   A collection of objects of the given type.
   */
  public function get() {
    $nodes = array();
    $result = db_query("SELECT n.nid FROM {node} n INNER JOIN {users} u ON n.uid = u.uid WHERE u.status = :user_status AND type = :type AND n.status = :node_status ORDER BY n.changed DESC LIMIT " . $this->query_offset . ", " . $this->query_limit, array(':user_status' => 1, ':node_status' => $this->query_node_status, ':type' => $this->node_type));

    $nids = array();
    foreach ($result as $value) {
      $nids[] = $value->nid;
    }

    if (!empty($nids)) {
      $node_results = entity_load('node', $nids);

      // Load into node objects.
      foreach ($node_results as $key => $node) {
        if ($node->nid) {
          $class_name = TLRUtil::camelcaser($node->type);
          $nodes[] = new $class_name($node);
        }
      }
    }

    return $nodes;
  }

  /**
   * Set the limit of the get() query.
   *
   * @param int
   *   The limit number.
   * @return this
   */
  public function limit($count) {
    $this->query_limit = $count;
    return $this;
  }

  /**
   * Set the offset of the get() query.
   *
   * @param int
   *   The offset number.
   * @return this
   */
  public function offset($count) {
    $this->query_offset = $count;
    return $this;
  }
}
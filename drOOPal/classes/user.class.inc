<?php

class User extends BaseClass {
  public function __construct($mixed) {
    parent::__construct($mixed, 'user');
  }

  /**
   * Get the user picture.
   *
   * @return string The url of the user picture.
   */
  public function get_picture() {
    $picture_field = field_get_items('user', $this, 'field_user_picture');
    $picture = !empty($picture_field) ? $picture_field[0]['uri'] : file_build_uri('default_images/missing_teacher-blue_0.png');
    return $picture;
  }

  /**
   * Get the display name of the user.
   *
   * @return string The user's display name.
   */
  public function get_display_name() {
    return tlr_user_get_display_name($this);
  }

  /**
   * Get the school of the user if it exists.
   *
   * @return object The school data object.
   */
  public function get_school() {
    $user_wrapper = entity_metadata_wrapper('user', $this);

    $school = NULL;
    if (!$this->is_homeschooler()) {
      $school_id = csm_utility_get_field_value('user', $this, 'field_reference_school');
      if ($school_id) {
        return new School($school_id);
      }
    }
  }

  /**
   * Get the user's profile url.
   *
   * @return string The url of the user's profile.
   */
  public function get_profile_url($absolute = FALSE) {
    return url('user/' . $this->uid, array('absolute' => $absolute));
  }

  /**
   * Check if the user is a homeschooler.
   *
   * @return boolean TRUE|FALSE.
   */
  public function is_homeschooler() {
    $user_wrapper = entity_metadata_wrapper('user', $this);
    return $user_wrapper->field_homeschooler->value();
  }

  /**
   * Deprecated: Updates the promo code info.
   *
   * @param string A promo code.
   * @return this
   */
  public function update_promo_code($promo_code) {
    $promo_code = trim($promo_code);

    if (!empty($promo_code)) {
      $field_collection_item = entity_create('field_collection_item', array('field_name' => 'field_collection_promo_code'));
      $field_collection_item->setHostEntity('user', $this);
      $field_collection_item->field_promo_code[LANGUAGE_NONE][0]['value'] = $promo_code;
      $field_collection_item->field_promo_code_date[LANGUAGE_NONE][0]['value'] = date('Y-m-d H:i:s', mktime());
      $field_collection_item->save(TRUE);

      $user_edit = array(
        'field_collection_promo_code' => array(
          LANGUAGE_NONE => array(
            array(
              'value' => $field_collection_item->item_id,
              'revision_id' => $field_collection_item->revision_id,
            ),
          ),
        ),
      );
      $this->load(user_save($this, $user_edit));
    }
    return $this;
  }
}